#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "cpu_state.h"
#include "instructions.h"


static void set_status_flag(CpuState* cpu, CpuStatusFlag f, bool v);

CpuState ADC_(CpuState cpu, const uint16_t operand) {
  uint16_t result = cpu.a + cpu.bus[operand];
    if (result > 0xFF) {
      // Overflow - set carry bit
      set_status_flag(&cpu, C, true);
      cpu.a = result & 0xFF;
    } else {
      cpu.a = result;
    }
    return cpu;
}

CpuState AND_(const CpuState, const uint16_t operand) {
    
};

CpuState ASL_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState BCC_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState BCS_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState BEQ_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState BIT_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState BMI_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState BNE_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState BPL_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState BRK_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState BVC_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState BVS_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState CLC_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState CLD_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState CLI_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState CLV_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState CMP_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState CPX_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState CPY_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState DEC_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState DEX_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState DEY_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState EOR_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState INC_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState INX_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState INY_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState JMP_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState JSR_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState LDA_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState LDX_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState LDY_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState LSR_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState NOP_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState ORA_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState PHA_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState PHP_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState PLA_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState PLP_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState ROL_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState ROR_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState RTI_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState RTS_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState TRS_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState SBC_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState SEC_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState SED_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState SEI_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState STA_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState STX_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState STY_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState TAX_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState TAY_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState TSX_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState TXA_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState TXS_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState TYA_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};
CpuState ILLEGAL_INSTRUCTION_(const CpuState, const uint16_t operand){assert(false); /* not implemented */};


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

