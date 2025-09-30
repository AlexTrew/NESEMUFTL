#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "cpu.h"
#include "cpu_addr_mode.h"
#include "cpu_state.h"
#include "instructions.h"

/* some functions to make reading and writing memory slightly more readable */

static uint8_t read_memory(const CpuState* cpu, uint16_t address);
static void write_memory(const CpuState* cpu, uint16_t address, uint8_t value);

/* some utility functions and shared behaviours */

static void set_status_flag(CpuState* cpu, CpuStatusFlag f, bool v);
static uint8_t get_status_flag(const CpuState* cpu, CpuStatusFlag f);
static bool mem_addresses_on_same_page(uint16_t a, uint16_t b); 
static int8_t convert_16_bit_uint_to_8_bit_signed_int(uint16_t x);
static CpuInstructionResult branch_instruction(CpuState* cpu, bool follow_branch, CpuAddrMode addr_mode);

static uint8_t read_memory(const CpuState* cpu, uint16_t address){
  return cpu->bus[address] & 0xFF;
}

static void write_memory(const CpuState *cpu, uint16_t address, uint8_t value) {
  cpu->bus[address] = value;
}

static int8_t convert_16_bit_uint_to_8_bit_signed_int(uint16_t x) {
    int8_t res = (int16_t)x & 0xFF;
    return res;
}

static bool mem_addresses_on_same_page(uint16_t a, uint16_t b) {
    if ((a & 0xF0) != (b & 0xF0)) {
	return false;
    }          
    return true;
}  

CpuInstructionResult ADC_(CpuState* cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    uint16_t m = read_memory(cpu, addr_mode_data.operand_address);
    uint16_t result = cpu->a + m + get_status_flag(cpu, C);


    // set status flags
    set_status_flag(cpu, V, ~(((uint16_t)cpu->a ^ m) & ((uint16_t)cpu->a ^ result)) & 0x80); // still not sure why this wors, but it does
    set_status_flag(cpu, C, (result > 0xFF));
    set_status_flag(cpu, Z, (cpu->a == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    cpu->a = result & 0xFF;

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;

    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
}

CpuInstructionResult AND_(CpuState* cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->a = (cpu->a & (read_memory(cpu, addr_mode_data.operand_address) & 0xFF));

    // set status flags
    set_status_flag(cpu, Z, (cpu->a == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;

    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult ASL_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    uint16_t abs_addr = addr_mode_data.operand_address;
    uint16_t temp = read_memory(cpu, abs_addr) << 1;

    set_status_flag(cpu, C, (temp > 0xFF));
    set_status_flag(cpu, Z, (temp == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    if(addr_mode == IMPLIED){
	cpu->a = temp & 0xFF;
    } else {
	write_memory(cpu, abs_addr, temp);
    }

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;

    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

// Branch instructions

static CpuInstructionResult branch_instruction(CpuState* cpu, bool follow_branch, CpuAddrMode addr_mode){
    // branch instructions always use RELATIVE addressing
    assert(addr_mode == RELATIVE);
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    uint8_t additional_cycles_from_instruction = addr_mode_data.additional_cycles;
    
    if(follow_branch){
	
	// read operand_address bytes as a signed 8 bit integer
        uint16_t new_addr = cpu->pc + convert_16_bit_uint_to_8_bit_signed_int(addr_mode_data.operand_address);

	// add an additional cycle due to branching
	++additional_cycles_from_instruction;

	// if the new address is on a different page, add another additional cycle
	// TODO test this logic 
        if (!mem_addresses_on_same_page(new_addr, cpu->pc)) {
	    ++additional_cycles_from_instruction;
	}          

	cpu->pc = new_addr;

    } else {
	//update program counter
	cpu->pc += addr_mode_data.pc_offset;
    }

    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles+additional_cycles_from_instruction};
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

    cpu->stkptr = cpu->pc+2;
    set_status_flag(cpu, I, 1);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;

    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult BIT_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult CLC_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    set_status_flag(cpu, C, 0);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;

    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult CLD_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    set_status_flag(cpu, D, 0);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;

    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult CLI_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    set_status_flag(cpu, I, 0);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;

    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult CLV_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    set_status_flag(cpu, V, 0);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;

    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult CMP_(CpuState *cpu, CpuAddrMode addr_mode) {
    // compare memory with accumulator
    // A - M
    uint16_t res = cpu->a - cpu->bus[cpu->pc + 1];

    set_status_flag(cpu, C, res > 0xFF);
    set_status_flag(cpu, N, res & 0x80);
    set_status_flag(cpu, Z, res == 0);
};

CpuInstructionResult CPX_(CpuState *cpu, CpuAddrMode addr_mode) {
    // compare memory with X 
    // X - M
    uint16_t res = cpu->x - cpu->bus[cpu->pc + 1];

    set_status_flag(cpu, C, res > 0xFF);
    set_status_flag(cpu, N, res & 0x80);
    set_status_flag(cpu, Z, res == 0);
};

CpuInstructionResult CPY_(CpuState *cpu, CpuAddrMode addr_mode) {
    // compare memory with Y
    // Y - M
    uint16_t res = cpu->y - cpu->bus[cpu->pc + 1];

    set_status_flag(cpu, C, res > 0xFF);
    set_status_flag(cpu, N, res & 0x80);
    set_status_flag(cpu, Z, res == 0);
};

CpuInstructionResult DEC_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult DEX_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult DEY_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult EOR_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult INC_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult INX_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult INY_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult JMP_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult JSR_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult LDA_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->a = read_memory(cpu, addr_mode_data.operand_address);

    // set zero status flag
    set_status_flag(cpu, Z, (cpu->a == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;

    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult LDX_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->x = read_memory(cpu, addr_mode_data.operand_address);

    // set zero status flag
    set_status_flag(cpu, Z, (cpu->x == 0));
    set_status_flag(cpu, N, cpu->x & 0x80);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;

    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult LDY_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->y = read_memory(cpu, addr_mode_data.operand_address);

    // set zero status flag
    set_status_flag(cpu, Z, (cpu->y == 0));
    set_status_flag(cpu, N, cpu->y & 0x80);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;

    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult LSR_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult NOP_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;

    return res;
};

CpuInstructionResult ORA_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult PHA_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult PHP_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult PLA_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};
 
CpuInstructionResult PLP_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult ROL_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult ROR_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult RTI_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult RTS_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult TRS_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult SBC_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult SEC_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult SED_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult SEI_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult STA_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    write_memory(cpu, cpu->pc, cpu->a);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;

    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult STX_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    write_memory(cpu, cpu->pc, cpu->x);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;

    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult STY_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    write_memory(cpu, cpu->pc, cpu->y);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;

    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
}

CpuInstructionResult TAX_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->x = cpu->a;

    set_status_flag(cpu, Z, (cpu->x == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;

    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult TAY_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->y = cpu->a;

    set_status_flag(cpu, Z, (cpu->y == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;
    
    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult TSX_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->x = cpu->stkptr;

    set_status_flag(cpu, Z, (cpu->x == 0));
    set_status_flag(cpu, N, cpu->x & 0x80);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;
    
    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult TXA_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->a = cpu->stkptr;

    set_status_flag(cpu, Z, (cpu->a == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;
    
    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult TXS_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->stkptr = cpu->x;

    set_status_flag(cpu, Z, (cpu->stkptr == 0));
    set_status_flag(cpu, N, cpu->stkptr & 0x80);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;
    
    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult TYA_(CpuState *cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->a= cpu->y;

    set_status_flag(cpu, Z, (cpu->a == 0));
    set_status_flag(cpu, N, cpu->a & 0x80);

    //update program counter
    cpu->pc += addr_mode_data.pc_offset;
    
    CpuInstructionResult res = {.additional_cpu_cycles=addr_mode_data.additional_cycles};
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

