#include <assert.h>
#include <stdbool.h>
#include "instructions.h"


CpuState ADC_(CpuState cpu, const AddrModeFptr addr_mode_func) {
  const CpuAddressingModeResult address_mode = addr_mode_func(&cpu);
  const uint16_t result = cpu.a + cpu.bus[address_mode.operand];
    if (result > 0xFF) {
      cpu.a = 0xFF;
      // Overflow - set carry bit
      cpu.p = cpu.p | (1 << 0);
    } else {
      cpu.a = result;
    }
    return cpu;
}

CpuState AND_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState ASL_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState BCC_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState BCS_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState BEQ_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState BIT_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState BMI_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState BNE_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState BPL_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState BRK_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState BVC_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState BVS_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState CLC_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState CLD_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState CLI_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState CLV_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState CMP_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState CPX_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState CPY_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState DEC_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState DEX_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState DEY_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState EOR_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState INC_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState INX_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState INY_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState JMP_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState JSR_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState LDA_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState LDX_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState LDY_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState LSR_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState NOP_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState ORA_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState PHA_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState PHP_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState PLA_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState PLP_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState ROL_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState ROR_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState RTI_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState RTS_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState TRS_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState SBC_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState SEC_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState SED_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState SEI_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState STA_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState STX_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState STY_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState TAX_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState TAY_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState TSX_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState TXA_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState TXS_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState TYA_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};
CpuState ILLEGAL_INSTRUCTION_(const CpuState, AddrModeFptr addr_mode_func){assert(false); /* not implemented */};


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
