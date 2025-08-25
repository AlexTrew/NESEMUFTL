#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "cpu_state.h"
#include "instructions.h"


static void set_status_flag(CpuState* cpu, CpuStatusFlag f, bool v);

void ADC_(CpuState* cpu, const uint16_t operand) {
  uint16_t result = cpu->a + cpu->bus[operand];
    if (result > 0xFF) {
      // Overflow - set carry bit
      set_status_flag(cpu, C, true);
      cpu->a = result & 0xFF;
    } else {
      cpu->a = result;
    }
}

void AND_(CpuState* cpu, const uint16_t operand) {
    cpu->a = cpu->a & (operand & 0xFF);
};

void ASL_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void BCC_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void BCS_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void BEQ_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void BIT_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void BMI_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void BNE_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void BPL_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void BRK_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void BVC_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void BVS_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void CLC_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void CLD_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void CLI_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void CLV_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void CMP_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void CPX_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void CPY_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void DEC_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void DEX_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void DEY_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void EOR_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void INC_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void INX_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void INY_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void JMP_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void JSR_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void LDA_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void LDX_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void LDY_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void LSR_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void NOP_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void ORA_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void PHA_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void PHP_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void PLA_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void PLP_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void ROL_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void ROR_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void RTI_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void RTS_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void TRS_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void SBC_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void SEC_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void SED_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void SEI_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void STA_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void STX_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void STY_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void TAX_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void TAY_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void TSX_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void TXA_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void TXS_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void TYA_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};
void ILLEGAL_INSTRUCTION_(CpuState* cpu, const uint16_t operand){assert(false); /* not implemented */};


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

static void set_status_flag(CpuState* cpu, CpuStatusFlag f, bool v){
  if(v){
    cpu->p |= f;
  }
  else{
    cpu->p &= ~f;
  }
}

