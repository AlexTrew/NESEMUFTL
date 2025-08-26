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


typedef struct {
  // pointless for now but might be useful for logging
  CpuState updated_cpu_state;
  uint8_t additional_cpu_cycles;
} CpuInstructionResult;


CpuInstructionResult ADC_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult AND_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult ASL_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult BCC_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult BCS_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult BEQ_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult BIT_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult BMI_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult BNE_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult BPL_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult BRK_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult BVC_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult BVS_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult CLC_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult CLD_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult CLI_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult CLV_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult CMP_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult CPX_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult CPY_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult DEC_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult DEX_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult DEY_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult EOR_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult INC_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult INX_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult INY_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult JMP_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult JSR_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult LDA_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult LDX_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult LDY_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult LSR_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult NOP_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult ORA_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult PHA_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult PHP_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult PLA_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult PLP_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult ROL_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult ROR_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult RTI_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult RTS_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult TRS_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult SBC_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult SEC_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult SED_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult SEI_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult STA_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult STX_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult STY_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult TAX_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult TAY_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult TSX_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult TXA_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult TXS_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult TYA_(CpuState* cpu, CpuAddrMode addr_mode);
CpuInstructionResult ILLEGAL_INSTRUCTION_(CpuState* cpu, CpuAddrMode addr_mode);


typedef CpuInstructionResult(*CpuInstructionFPtr)(CpuState* cpu, CpuAddrMode addr_mode);
extern const CpuInstructionFPtr cpu_instruction_lookup[];

#endif
