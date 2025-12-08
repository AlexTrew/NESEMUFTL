#include "cpu.h"
#include "cpu_addr_mode.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "instructions.h"
#include "cpu_state.h"

/*
  See page 10 of archive.6502.org/datasheets/rockwell_r650x_r651x.pdf for a
  complete opcode -> instruction reference.
 */  
CpuState* init_cpu(uint8_t* memory){
  CpuState* cpu_ptr = (CpuState*)malloc(sizeof(CpuState));
  cpu_ptr->a=0x00;
  cpu_ptr->x=0x00;
  cpu_ptr->y=0x00;
  cpu_ptr->p=0x00;
  cpu_ptr->pc=0x0600;
  cpu_ptr->stkptr=0x00;
  cpu_ptr->cpu_cycle_count = 0;
  cpu_ptr->bus=memory;
  return cpu_ptr;
};

void delete_cpu(CpuState*cpu){
  free(cpu);
}

CpuInstruction get_instruction(const CpuState* cpu){
  uint8_t opcode = cpu->bus[cpu->pc];

  CpuInstruction current_instruction = opcode_x_cpu_instruction_lookup[opcode];
  if(current_instruction.name == ILLEGAL_INSTRUCTION){
    printf("illegal instruction (opcode %x) at 0x%x\n", opcode, cpu->pc);
    exit(1);
  }

  return current_instruction;
}

void cpu_cycle(CpuState* cpu){
  /* Load an instruction from an opcode and execute it.
     In this emulator, at least for now, instructions are exectued in a single cpu iteration:
     we do nothing for the remaining cycles for the instruction so the timings are correct. */

    static uint8_t cycles_until_next_instruction = 0;
    if(cycles_until_next_instruction == 0){

	// look up the instruction and get the operand using the specified addressing mode.
	CpuInstruction current_instruction = get_instruction(cpu);

	// execute the instruction, updating the state of the cpu
	CpuInstructionResult instruction_result = cpu_instruction_lookup[current_instruction.name](cpu, current_instruction.addressing_mode);

	printf("Executed %s at %x\n", current_instruction.name_str, cpu->pc);
   
	// set the number of cycles until the next instruction is loaded
	cycles_until_next_instruction = current_instruction.cycles_left + instruction_result.additional_cpu_cycles;


	/* move the program counter to the next instruction for all non jump instructions.
	   Jump instructions are responsible for updating the program counter themselves
	   for the sake of simplicity.
	 */
	if(current_instruction.name != JMP && current_instruction.name != JSR){
	    cpu->pc += current_instruction.size_in_bytes + instruction_result.pc_offset;
	}
    }
    else{
	--cycles_until_next_instruction;
    }
}

