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


CpuState ADC_(const CpuState, const uint16_t operand);
CpuState AND_(const CpuState, const uint16_t operand);
CpuState ASL_(const CpuState, const uint16_t operand);
CpuState BCC_(const CpuState, const uint16_t operand);
CpuState BCS_(const CpuState, const uint16_t operand);
CpuState BEQ_(const CpuState, const uint16_t operand);
CpuState BIT_(const CpuState, const uint16_t operand);
CpuState BMI_(const CpuState, const uint16_t operand);
CpuState BNE_(const CpuState, const uint16_t operand);
CpuState BPL_(const CpuState, const uint16_t operand);
CpuState BRK_(const CpuState, const uint16_t operand);
CpuState BVC_(const CpuState, const uint16_t operand);
CpuState BVS_(const CpuState, const uint16_t operand);
CpuState CLC_(const CpuState, const uint16_t operand);
CpuState CLD_(const CpuState, const uint16_t operand);
CpuState CLI_(const CpuState, const uint16_t operand);
CpuState CLV_(const CpuState, const uint16_t operand);
CpuState CMP_(const CpuState, const uint16_t operand);
CpuState CPX_(const CpuState, const uint16_t operand);
CpuState CPY_(const CpuState, const uint16_t operand);
CpuState DEC_(const CpuState, const uint16_t operand);
CpuState DEX_(const CpuState, const uint16_t operand);
CpuState DEY_(const CpuState, const uint16_t operand);
CpuState EOR_(const CpuState, const uint16_t operand);
CpuState INC_(const CpuState, const uint16_t operand);
CpuState INX_(const CpuState, const uint16_t operand);
CpuState INY_(const CpuState, const uint16_t operand);
CpuState JMP_(const CpuState, const uint16_t operand);
CpuState JSR_(const CpuState, const uint16_t operand);
CpuState LDA_(const CpuState, const uint16_t operand);
CpuState LDX_(const CpuState, const uint16_t operand);
CpuState LDY_(const CpuState, const uint16_t operand);
CpuState LSR_(const CpuState, const uint16_t operand);
CpuState NOP_(const CpuState, const uint16_t operand);
CpuState ORA_(const CpuState, const uint16_t operand);
CpuState PHA_(const CpuState, const uint16_t operand);
CpuState PHP_(const CpuState, const uint16_t operand);
CpuState PLA_(const CpuState, const uint16_t operand);
CpuState PLP_(const CpuState, const uint16_t operand);
CpuState ROL_(const CpuState, const uint16_t operand);
CpuState ROR_(const CpuState, const uint16_t operand);
CpuState RTI_(const CpuState, const uint16_t operand);
CpuState RTS_(const CpuState, const uint16_t operand);
CpuState TRS_(const CpuState, const uint16_t operand);
CpuState SBC_(const CpuState, const uint16_t operand);
CpuState SEC_(const CpuState, const uint16_t operand);
CpuState SED_(const CpuState, const uint16_t operand);
CpuState SEI_(const CpuState, const uint16_t operand);
CpuState STA_(const CpuState, const uint16_t operand);
CpuState STX_(const CpuState, const uint16_t operand);
CpuState STY_(const CpuState, const uint16_t operand);
CpuState TAX_(const CpuState, const uint16_t operand);
CpuState TAY_(const CpuState, const uint16_t operand);
CpuState TSX_(const CpuState, const uint16_t operand);
CpuState TXA_(const CpuState, const uint16_t operand);
CpuState TXS_(const CpuState, const uint16_t operand);
CpuState TYA_(const CpuState, const uint16_t operand);
CpuState ILLEGAL_INSTRUCTION_(const CpuState, const uint16_t operand);


typedef CpuState (*CpuInstructionFPtr)(const CpuState cpu, const uint16_t operand);
extern const CpuInstructionFPtr cpu_instruction_lookup[];

#endif
