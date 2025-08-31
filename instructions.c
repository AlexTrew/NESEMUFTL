#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "cpu_addr_mode.h"
#include "cpu_state.h"
#include "instructions.h"


static void set_status_flag(CpuState* cpu, CpuStatusFlag f, bool v);
static uint8_t get_status_flag(const CpuState* cpu, CpuStatusFlag f);

CpuInstructionResult ADC_(CpuState* cpu, CpuAddrMode addr_mode) {
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    uint16_t result = cpu->a + cpu->bus[addr_mode_data.operand] + get_status_flag(cpu, C);

    set_status_flag(cpu, C, (result > 0xFF));
    cpu->a = result & 0xFF;

    // set the ZERO status flag if the result is 0;
    set_status_flag(cpu, Z, (cpu->a == 0));

    CpuInstructionResult res = {.updated_cpu_state=*cpu, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
}

CpuInstructionResult AND_(CpuState* cpu, CpuAddrMode addr_mode) {

    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->a = cpu->a & (addr_mode_data.operand & 0xFF);

    // set the ZERO status flag if the result is 0;
    set_status_flag(cpu, Z, (cpu->a == 0));
    set_status_flag(cpu, N, false);

    // overflow flag... not sure how this works yet..
    set_status_flag(cpu, V, false);

    CpuInstructionResult res = {.updated_cpu_state=*cpu, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult ASL_(CpuState *cpu, CpuAddrMode addr_mode) {

    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);

    uint16_t abs_addr = addr_mode_data.operand;
    uint16_t temp = cpu->bus[abs_addr] << 1;

    set_status_flag(cpu, C, (temp > 0xFF));
    set_status_flag(cpu, Z, (temp == 0));

    if(addr_mode == IMPLIED){
	cpu->a = temp & 0xFF;
    } else {
	cpu->bus[abs_addr] = temp;
    }

    CpuInstructionResult res = {.updated_cpu_state= *cpu, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult BCC_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult BCS_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult BEQ_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult BIT_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult BMI_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult BNE_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult BPL_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult BRK_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult BVC_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult BVS_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult CLC_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult CLD_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult CLI_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult CLV_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult CMP_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult CPX_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult CPY_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

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
    cpu->a = addr_mode_data.operand;

    // set zero status flag
    set_status_flag(cpu, Z, (cpu->a == 0));

    CpuInstructionResult res = {.updated_cpu_state=*cpu, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult LDX_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->x = addr_mode_data.operand;

    // set zero status flag
    set_status_flag(cpu, Z, (cpu->x == 0));

    CpuInstructionResult res = {.updated_cpu_state=*cpu, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult LDY_(CpuState* cpu, CpuAddrMode addr_mode){
    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[addr_mode](cpu);
    cpu->y = addr_mode_data.operand;

    // set zero status flag
    set_status_flag(cpu, Z, (cpu->y == 0));

    CpuInstructionResult res = {.updated_cpu_state=*cpu, .additional_cpu_cycles=addr_mode_data.additional_cycles};
    return res;
};

CpuInstructionResult LSR_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult NOP_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

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

CpuInstructionResult STA_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult STX_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult STY_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult TAX_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult TAY_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult TSX_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult TXA_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult TXS_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

CpuInstructionResult TYA_(CpuState* cpu, CpuAddrMode addr_mode){assert(false); /* not implemented */};

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

