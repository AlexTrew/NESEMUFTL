#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "cpu_addr_mode.h"
#include "cpu_state.h"
#include "instructions.h"

/* some functions to make reading and writing memory slightly more readable */

static uint8_t read_memory(const CpuState* cpu, uint16_t address);
static void write_memory(const CpuState* cpu, uint16_t address, uint8_t value);
static void stack_push(CpuState* cpu, uint8_t value);
static uint8_t stack_pop(CpuState* cpu);


/* some utility functions and shared behaviours */

static void set_status_flag(CpuState* cpu, CpuStatusFlag f, bool v);
static uint8_t get_status_flag(const CpuState* cpu, CpuStatusFlag f);
static bool mem_addresses_on_same_page(uint16_t a, uint16_t b); 
static CpuInstructionResult branch_instruction(CpuState* cpu, bool follow_branch, CpuAddrMode addr_mode);
static CpuInstructionResult compare_instruction(CpuState* cpu, CpuAddrMode addr_mode, uint8_t value_to_compare);

static uint8_t read_memory(const CpuState* cpu, uint16_t address){
  return cpu->bus[address] & 0xFF;
}

static void write_memory(const CpuState *cpu, uint16_t address, uint8_t value) {
  cpu->bus[address] = value;
}

static bool mem_addresses_on_same_page(uint16_t a, uint16_t b) {
    if ((a & 0xF0) != (b & 0xF0)) {
	return false;
    }          
    return true;
}  

static void stack_push(CpuState* cpu, uint8_t value){
    uint16_t actual_stack_ptr = cpu->stkptr + 0x0100;
    write_memory(cpu, actual_stack_ptr, value);
    ++cpu->stkptr; 
};

static uint8_t stack_pop(CpuState* cpu){
    uint16_t actual_stack_ptr = cpu->stkptr + 0x0100;
    uint8_t res = read_memory(cpu, actual_stack_ptr);
    --cpu->stkptr;

    return res;
};

CpuInstructionResult ADC_(CpuState* cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    uint16_t m = read_memory(cpu, addr_mode_data.operand_address);
    uint16_t result = cpu->a + m + get_status_flag(cpu, C);

    // set status flags
    set_status_flag(cpu, V, ~(((uint16_t)cpu->a ^ m) & ((uint16_t)cpu->a ^ result)) & 0x80); // still not sure why this works and the gcc builtin doesnt, but it does.
    set_status_flag(cpu, C, (result > 0xFF));
    set_status_flag(cpu, Z, (cpu->a == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    cpu->a = result & 0xFF;

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
}

CpuInstructionResult AND_(CpuState* cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->a = (cpu->a & (read_memory(cpu, addr_mode_data.operand_address) & 0xFF));

    // set status flags
    set_status_flag(cpu, Z, (cpu->a == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult ASL_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    uint16_t abs_addr = addr_mode_data.operand_address;
    uint16_t temp = read_memory(cpu, abs_addr) << 1;

    set_status_flag(cpu, C, (temp > 0xFF));
    set_status_flag(cpu, Z, (temp == 0));
    set_status_flag(cpu, N, 0);

    if(addr_mode == IMPLIED){
	cpu->a = temp & 0xFF;
    } else {
	write_memory(cpu, abs_addr, temp);
    }

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

// Branch instructions

static CpuInstructionResult branch_instruction(CpuState* cpu, bool follow_branch, CpuAddrMode addr_mode){
    // branch instructions always use RELATIVE addressing
    assert(addr_mode == RELATIVE);
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    uint8_t additional_cycles_from_instruction = 0;

    int8_t pc_offset = 0;

    if(follow_branch){

        /* we read the operand as the target address, and get the offset from
           the beginning of the next instruction. This takes the form of
           offset = M - pc + 2, where 2 is the size of the current instruction,
	*/

	uint16_t target_addr = addr_mode_data.operand_address;
	// add an additional cycle due to branching
	++additional_cycles_from_instruction;

	// if the new address is on a different page, add another additional cycle
	// 2 is the size of the branch instruction
        if (!mem_addresses_on_same_page(target_addr, cpu->pc + 2)) {
	    ++additional_cycles_from_instruction;
	};

	// set the pc offset
	if(target_addr > cpu->pc +2){
	  pc_offset = (target_addr - (cpu->pc +2));
	}
	else{
	  pc_offset = ((cpu->pc +2) - target_addr);
	}
    }        

    CpuInstructionResult res = {.pc_offset=pc_offset, .additional_cpu_cycles=addr_mode_data.additional_cycles+additional_cycles_from_instruction};
    return res;
}

CpuInstructionResult BCC_(CpuState *cpu, CpuAddrMode addr_mode) {
    bool follow_branch = (get_status_flag(cpu, C) == 0);
    return branch_instruction(cpu, follow_branch, addr_mode);
};

CpuInstructionResult BCS_(CpuState* cpu, CpuAddrMode addr_mode){
    bool follow_branch = (get_status_flag(cpu, C) == 1);
    return branch_instruction(cpu, follow_branch, addr_mode);
};

CpuInstructionResult BEQ_(CpuState *cpu, CpuAddrMode addr_mode) {
    bool follow_branch = (get_status_flag(cpu, Z) == 1);
    return branch_instruction(cpu, follow_branch, addr_mode);
};

CpuInstructionResult BMI_(CpuState *cpu, CpuAddrMode addr_mode) {
    bool follow_branch = (get_status_flag(cpu, N) == 1);
    return branch_instruction(cpu, follow_branch, addr_mode);
};

CpuInstructionResult BNE_(CpuState *cpu, CpuAddrMode addr_mode) {
    bool follow_branch = (get_status_flag(cpu, Z) == 0);
    return branch_instruction(cpu, follow_branch, addr_mode);
};

CpuInstructionResult BPL_(CpuState *cpu, CpuAddrMode addr_mode) {
    bool follow_branch = (get_status_flag(cpu, N) == 0);
    return branch_instruction(cpu, follow_branch, addr_mode);
};

CpuInstructionResult BVC_(CpuState *cpu, CpuAddrMode addr_mode) {
    bool follow_branch = (get_status_flag(cpu, V) == 0);
    return branch_instruction(cpu, follow_branch, addr_mode);
};

CpuInstructionResult BVS_(CpuState *cpu, CpuAddrMode addr_mode) {
    bool follow_branch = (get_status_flag(cpu, V) == 1);
    return branch_instruction(cpu, follow_branch, addr_mode);
};

CpuInstructionResult BRK_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    stack_push(cpu, cpu->pc+2);
    set_status_flag(cpu, I, 1);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult BIT_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult CLC_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    set_status_flag(cpu, C, 0);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult CLD_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    set_status_flag(cpu, D, 0);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult CLI_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    set_status_flag(cpu, I, 0);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult CLV_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    set_status_flag(cpu, V, 0);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

// comparison instructions

static CpuInstructionResult compare_instruction(CpuState* cpu, CpuAddrMode addr_mode, uint8_t value_to_compare){
  /* Compare a value with the operand of the current instruction and set the status flags accordingly. */

    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    uint16_t diff = value_to_compare - addr_mode_data.operand_address;

    set_status_flag(cpu, C, diff > 0xFF);
    set_status_flag(cpu, N, diff & 0x80);
    set_status_flag(cpu, Z, diff == 0);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
}


CpuInstructionResult CMP_(CpuState *cpu, CpuAddrMode addr_mode) {
    // compare memory with accumulator
    // A - M
    return compare_instruction(cpu, addr_mode, cpu->a);
};

CpuInstructionResult CPX_(CpuState *cpu, CpuAddrMode addr_mode) {
    // compare memory with X 
    // X - M
    return compare_instruction(cpu, addr_mode, cpu->x);
};

CpuInstructionResult CPY_(CpuState *cpu, CpuAddrMode addr_mode) {
    // compare memory with Y
    // Y - M
    return compare_instruction(cpu, addr_mode, cpu->x);
};

CpuInstructionResult DEC_(CpuState *cpu, CpuAddrMode addr_mode) {
    // decrement the value at the memory location
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    uint8_t value = read_memory(cpu, addr_mode_data.operand_address) - 1;
    write_memory(cpu, addr_mode_data.operand_address, value);

    set_status_flag(cpu, Z, (value & 0xFF) == 0);
    set_status_flag(cpu, N, (value & 0x80));

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult DEX_(CpuState *cpu, CpuAddrMode addr_mode) {
    cpu->x = cpu->x-1;
    
    set_status_flag(cpu, Z, (cpu->x & 0xFF) == 0);
    set_status_flag(cpu, N, (cpu->x & 0x80));

    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    CpuInstructionResult res = {
      .pc_offset=0, .additional_cpu_cycles = addr_mode_data.additional_cycles
          };
    return res;
};

CpuInstructionResult DEY_(CpuState *cpu, CpuAddrMode addr_mode) {
    cpu->y = cpu->y-1;
    
    set_status_flag(cpu, Z, (cpu->y & 0xFF) == 0);
    set_status_flag(cpu, N, (cpu->y & 0x80));

    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    CpuInstructionResult res = {
      .pc_offset=0, .additional_cpu_cycles = addr_mode_data.additional_cycles
          };
    return res;
};

CpuInstructionResult EOR_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    cpu->a = read_memory(cpu, addr_mode_data.operand_address) ^ cpu->a;

    set_status_flag(cpu, Z, (cpu->a & 0xFF) == 0);
    set_status_flag(cpu, N, (cpu->a & 0x80));

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult INC_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    
    uint16_t value = read_memory(cpu, addr_mode_data.operand_address) + 1;
    write_memory(cpu, addr_mode_data.operand_address, value);

    set_status_flag(cpu, Z, (cpu->y & 0xFF) == 0);
    set_status_flag(cpu, N, (cpu->y & 0x80));

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult INX_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    
    set_status_flag(cpu, Z, (cpu->y & 0xFF) == 0);
    set_status_flag(cpu, N, (cpu->y & 0x80));

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult INY_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    
    set_status_flag(cpu, Z, (cpu->y & 0xFF) == 0);
    set_status_flag(cpu, N, (cpu->y & 0x80));

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult JMP_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    uint16_t target_addr = addr_mode_data.operand_address;

    // set the pc offset
    cpu->pc = target_addr;
    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult JSR_(CpuState* cpu, CpuAddrMode addr_mode){
    stack_push(cpu, cpu->pc+2);
    return JMP_(cpu, addr_mode);
};

CpuInstructionResult LDA_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->a = read_memory(cpu, addr_mode_data.operand_address);

    // set zero status flag
    set_status_flag(cpu, Z, (cpu->a == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult LDX_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->x = read_memory(cpu, addr_mode_data.operand_address);

    // set zero status flag
    set_status_flag(cpu, Z, (cpu->x == 0));
    set_status_flag(cpu, N, cpu->x & 0x80);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult LDY_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->y = read_memory(cpu, addr_mode_data.operand_address);

    // set zero status flag
    set_status_flag(cpu, Z, (cpu->y == 0));
    set_status_flag(cpu, N, cpu->y & 0x80);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult LSR_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    uint16_t abs_addr = addr_mode_data.operand_address;
    uint16_t temp = 1 >> read_memory(cpu, abs_addr);

    set_status_flag(cpu, C, (temp > 0xFF));
    set_status_flag(cpu, Z, (temp == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    if(addr_mode == IMPLIED){
	cpu->a = temp & 0xFF;
    } else {
	write_memory(cpu, abs_addr, temp);
    }

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult NOP_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};

    return res;
};

CpuInstructionResult ORA_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    cpu->a = read_memory(cpu, addr_mode_data.operand_address) | cpu->a;

    set_status_flag(cpu, Z, (cpu->a & 0xFF) == 0);
    set_status_flag(cpu, N, (cpu->a & 0x80));

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult PHA_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    stack_push(cpu, cpu->a);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult PHP_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    stack_push(cpu, cpu->p);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult PLA_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    cpu->a = stack_pop(cpu);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult PLP_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    cpu->p = stack_pop(cpu);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult ROL_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    uint16_t abs_addr = addr_mode_data.operand_address;
    bool tmp_carry = (abs_addr << 7) == 0;

    uint16_t temp = read_memory(cpu, abs_addr) << get_status_flag(cpu, C);

    set_status_flag(cpu, C, tmp_carry);
    set_status_flag(cpu, Z, (temp == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    if(addr_mode == IMPLIED){
	cpu->a = temp & 0xFF;
    } else {
	write_memory(cpu, abs_addr, temp);
    }

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult ROR_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    uint16_t abs_addr = addr_mode_data.operand_address;
    bool tmp_carry = (7 >> abs_addr) == 0;

    uint16_t temp = get_status_flag(cpu, C) >> read_memory(cpu, abs_addr);

    set_status_flag(cpu, C, tmp_carry);
    set_status_flag(cpu, Z, (temp == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    if(addr_mode == IMPLIED){
	cpu->a = temp & 0xFF;
    } else {
	write_memory(cpu, abs_addr, temp);
    }

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult RTI_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult RTS_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult TRS_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult SBC_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult SEC_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    set_status_flag(cpu, C, 1);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult SED_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    set_status_flag(cpu, D, 1);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult SEI_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    set_status_flag(cpu, I, 1);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};


CpuInstructionResult STA_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    write_memory(cpu, cpu->pc, cpu->a);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult STX_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    write_memory(cpu, cpu->pc, cpu->x);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult STY_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    write_memory(cpu, cpu->pc, cpu->y);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
}

CpuInstructionResult TAX_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->x = cpu->a;

    set_status_flag(cpu, Z, (cpu->x == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult TAY_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->y = cpu->a;

    set_status_flag(cpu, Z, (cpu->y == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult TSX_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->x = cpu->stkptr;

    set_status_flag(cpu, Z, (cpu->x == 0));
    set_status_flag(cpu, N, cpu->x & 0x80);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult TXA_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->a = cpu->stkptr;

    set_status_flag(cpu, Z, (cpu->a == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult TXS_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->stkptr = cpu->x;

    set_status_flag(cpu, Z, (cpu->stkptr == 0));
    set_status_flag(cpu, N, cpu->stkptr & 0x80);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult TYA_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->a= cpu->y;

    set_status_flag(cpu, Z, (cpu->a == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    CpuInstructionResult res = {.pc_offset=0, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult ILLEGAL_INSTRUCTION_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

const CpuInstructionFPtr cpu_instruction_lookup[] = {
  [ADC] = ADC_,
  [AND] = AND_,
  [ASL] = ASL_,
  [BCC] = BCC_,
  [BCS] = BCS_,
  [BEQ] = BEQ_,
  [BIT] = BIT_,
  [BMI] = BMI_,
  [BNE] = BNE_,
  [BPL] = BPL_,
  [BRK] = BRK_,
  [BVC] = BVC_,
  [BVS] = BVS_,
  [CLC] = CLC_,
  [CLD] = CLD_,
  [CLI] = CLI_,
  [CLV] = CLV_,
  [CMP] = CMP_,
  [CPX] = CPX_,
  [CPY] = CPY_,
  [DEC] = DEC_,
  [DEX] = DEX_,
  [DEY] = DEY_,
  [EOR] = EOR_,
  [INC] = INC_,
  [INX] = INX_,
  [INY] = INY_,
  [JMP] = JMP_,
  [JSR] = JSR_,
  [LDA] = LDA_,
  [LDX] = LDX_,
  [LDY] = LDY_,
  [LSR] = LSR_,
  [NOP] = NOP_,
  [ORA] = ORA_,
  [PHA] = PHA_,
  [PHP] = PHP_,
  [PLA] = PLA_,
  [PLP] = PLP_,
  [ROL] = ROL_,
  [ROR] = ROR_,
  [RTI] = RTI_,
  [RTS] = RTS_,
  [TRS] = TRS_,
  [SBC] = SBC_,
  [SEC] = SEC_,
  [SED] = SED_,
  [SEI] = SEI_,
  [STA] = STA_,
  [STX] = STX_,
  [STY] = STY_,
  [TAX] = TAX_,
  [TAY] = TAY_,
  [TSX] = TSX_,
  [TXA] = TXA_,
  [TXS] = TXS_,
  [TYA] = TYA_,
  [ILLEGAL_INSTRUCTION] = ILLEGAL_INSTRUCTION_
};

static uint8_t get_status_flag(const CpuState* cpu, CpuStatusFlag f){
  return cpu->p & f;
}

static void set_status_flag(CpuState* cpu, CpuStatusFlag f, bool v){
  if(v){
    cpu->p |= f;
  }
  else{
    cpu->p &= ~f;
  }
}

