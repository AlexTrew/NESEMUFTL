#include "cpu.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const CpuInstruction cpu_instruction_lookup[] = {
  [69] = {.name="ADC", .cycles_left=2, .mem_needed=2}
};

static void process_instruction(CpuInstruction* instruction);
static void set_status_flag(Cpu* cpu, CpuStatusFlag f, bool v);
static uint8_t cpu_read_from_bus(Cpu* cpu, uint16_t addr);
static void cpu_write(Cpu* cpu, uint16_t addr, uint8_t data);

static uint8_t cpu_read_from_bus(Cpu* cpu, uint16_t addr){

  printf("addr passed %d\n", addr);
  printf("bus val %d\n", cpu->bus[addr]);
  return cpu->bus[addr];
}

static void cpu_write(Cpu* cpu, uint16_t addr, uint8_t data){
  if(addr >= 0x0000 && addr <= 0xFFFF){
    cpu->bus[addr] = data;
  }
}

static void set_status_flag(Cpu* cpu, CpuStatusFlag f, bool v){
  if(v){
    cpu->status |= f;
  }
  else{
    cpu->status &= ~f;
  }
}

static void process_instruction(CpuInstruction* instruction){
    printf("instruction: %s, cycles_left: %d\n", instruction->name, instruction->cycles_left);
    --instruction->cycles_left;
}

Cpu *init_cpu(uint16_t* memory){
  Cpu* cpu_ptr = (Cpu*)malloc(sizeof(Cpu));
  cpu_ptr->a=0x00;
  cpu_ptr->x=0x00;
  cpu_ptr->y=0x00;
  cpu_ptr->pc=0x0001;
  cpu_ptr->stkptr=0x00;
  cpu_ptr->status=0x00;
  cpu_ptr->cpu_cycle_count = 0;
  cpu_ptr->bus=memory;
  return cpu_ptr;
};

void delete_cpu(Cpu *cpu){
  free(cpu);
}

void cpu_cycle(Cpu* cpu){
  static CpuInstruction* current_instruction_ptr = NULL;

  /* check if the current instruction is complete.
     if so, clean it up */
  if(current_instruction_ptr && current_instruction_ptr->cycles_left == 0){
    free(current_instruction_ptr);
    current_instruction_ptr = NULL;
  }

  if(current_instruction_ptr == NULL){
    /*
      get the opcode from the bus and look up the instruction
     */
    printf("pc addr: %d\n", cpu->pc);
    uint8_t opcode = cpu_read_from_bus(cpu, cpu->pc);
    printf("instruction opcode: %d\n", opcode);
    current_instruction_ptr = (CpuInstruction*)malloc(sizeof(CpuInstruction));
    memcpy(current_instruction_ptr, &cpu_instruction_lookup[opcode], sizeof(CpuInstruction));
    process_instruction(current_instruction_ptr);
  }
  else{
    process_instruction(current_instruction_ptr);
  }

  ++cpu->pc;
}
