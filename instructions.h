#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>
#include "cpu_addr_mode.h"
#include "cpu_state.h"

typedef enum {
  ADC,
  AND,
  ASL,
  BCC,
  BCS,
  BEQ,
  BIT,
  BMI,
  BNE,
  BPL,
  BRK,
  BVC,
  BVS,
  CLC,
  CLD,
  CLI,
  CLV,
  CMP,
  CPX,
  CPY,
  DEC,
  DEX,
  DEY,
  EOR,
  INC,
  INX,
  INY,
  JMP,
  JSR,
  LDA,
  LDX,
  LDY,
  LSR,
  NOP,
  ORA,
  PHA,
  PHP,
  PLA,
  PLP,
  ROL,
  ROR,
  RTI,
  RTS,
  TRS,
  SBC,
  SEC,
  SED,
  SEI,
  STA,
  STX,
  STY,
  TAX,
  TAY,
  TSX,
  TXA,
  TXS,
  TYA,
  ILLEGAL_INSTRUCTION
} CpuInstructionName;


CpuState ADC_(const CpuState, AddrModeFptr addr_mode_func);
CpuState AND_(const CpuState, AddrModeFptr addr_mode_func);
CpuState ASL_(const CpuState, AddrModeFptr addr_mode_func);
CpuState BCC_(const CpuState, AddrModeFptr addr_mode_func);
CpuState BCS_(const CpuState, AddrModeFptr addr_mode_func);
CpuState BEQ_(const CpuState, AddrModeFptr addr_mode_func);
CpuState BIT_(const CpuState, AddrModeFptr addr_mode_func);
CpuState BMI_(const CpuState, AddrModeFptr addr_mode_func);
CpuState BNE_(const CpuState, AddrModeFptr addr_mode_func);
CpuState BPL_(const CpuState, AddrModeFptr addr_mode_func);
CpuState BRK_(const CpuState, AddrModeFptr addr_mode_func);
CpuState BVC_(const CpuState, AddrModeFptr addr_mode_func);
CpuState BVS_(const CpuState, AddrModeFptr addr_mode_func);
CpuState CLC_(const CpuState, AddrModeFptr addr_mode_func);
CpuState CLD_(const CpuState, AddrModeFptr addr_mode_func);
CpuState CLI_(const CpuState, AddrModeFptr addr_mode_func);
CpuState CLV_(const CpuState, AddrModeFptr addr_mode_func);
CpuState CMP_(const CpuState, AddrModeFptr addr_mode_func);
CpuState CPX_(const CpuState, AddrModeFptr addr_mode_func);
CpuState CPY_(const CpuState, AddrModeFptr addr_mode_func);
CpuState DEC_(const CpuState, AddrModeFptr addr_mode_func);
CpuState DEX_(const CpuState, AddrModeFptr addr_mode_func);
CpuState DEY_(const CpuState, AddrModeFptr addr_mode_func);
CpuState EOR_(const CpuState, AddrModeFptr addr_mode_func);
CpuState INC_(const CpuState, AddrModeFptr addr_mode_func);
CpuState INX_(const CpuState, AddrModeFptr addr_mode_func);
CpuState INY_(const CpuState, AddrModeFptr addr_mode_func);
CpuState JMP_(const CpuState, AddrModeFptr addr_mode_func);
CpuState JSR_(const CpuState, AddrModeFptr addr_mode_func);
CpuState LDA_(const CpuState, AddrModeFptr addr_mode_func);
CpuState LDX_(const CpuState, AddrModeFptr addr_mode_func);
CpuState LDY_(const CpuState, AddrModeFptr addr_mode_func);
CpuState LSR_(const CpuState, AddrModeFptr addr_mode_func);
CpuState NOP_(const CpuState, AddrModeFptr addr_mode_func);
CpuState ORA_(const CpuState, AddrModeFptr addr_mode_func);
CpuState PHA_(const CpuState, AddrModeFptr addr_mode_func);
CpuState PHP_(const CpuState, AddrModeFptr addr_mode_func);
CpuState PLA_(const CpuState, AddrModeFptr addr_mode_func);
CpuState PLP_(const CpuState, AddrModeFptr addr_mode_func);
CpuState ROL_(const CpuState, AddrModeFptr addr_mode_func);
CpuState ROR_(const CpuState, AddrModeFptr addr_mode_func);
CpuState RTI_(const CpuState, AddrModeFptr addr_mode_func);
CpuState RTS_(const CpuState, AddrModeFptr addr_mode_func);
CpuState TRS_(const CpuState, AddrModeFptr addr_mode_func);
CpuState SBC_(const CpuState, AddrModeFptr addr_mode_func);
CpuState SEC_(const CpuState, AddrModeFptr addr_mode_func);
CpuState SED_(const CpuState, AddrModeFptr addr_mode_func);
CpuState SEI_(const CpuState, AddrModeFptr addr_mode_func);
CpuState STA_(const CpuState, AddrModeFptr addr_mode_func);
CpuState STX_(const CpuState, AddrModeFptr addr_mode_func);
CpuState STY_(const CpuState, AddrModeFptr addr_mode_func);
CpuState TAX_(const CpuState, AddrModeFptr addr_mode_func);
CpuState TAY_(const CpuState, AddrModeFptr addr_mode_func);
CpuState TSX_(const CpuState, AddrModeFptr addr_mode_func);
CpuState TXA_(const CpuState, AddrModeFptr addr_mode_func);
CpuState TXS_(const CpuState, AddrModeFptr addr_mode_func);
CpuState TYA_(const CpuState, AddrModeFptr addr_mode_func);
CpuState ILLEGAL_INSTRUCTION_(const CpuState, AddrModeFptr addr_mode_func);


typedef CpuState (*CpuInstructionFPtr)(const CpuState cpu, const AddrModeFptr addr_mode_func);
extern const CpuInstructionFPtr cpu_instruction_lookup[];

#endif
