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


void ADC_(CpuState* cpu, const uint16_t operand);
void AND_(CpuState* cpu, const uint16_t operand);
void ASL_(CpuState* cpu, const uint16_t operand);
void BCC_(CpuState* cpu, const uint16_t operand);
void BCS_(CpuState* cpu, const uint16_t operand);
void BEQ_(CpuState* cpu, const uint16_t operand);
void BIT_(CpuState* cpu, const uint16_t operand);
void BMI_(CpuState* cpu, const uint16_t operand);
void BNE_(CpuState* cpu, const uint16_t operand);
void BPL_(CpuState* cpu, const uint16_t operand);
void BRK_(CpuState* cpu, const uint16_t operand);
void BVC_(CpuState* cpu, const uint16_t operand);
void BVS_(CpuState* cpu, const uint16_t operand);
void CLC_(CpuState* cpu, const uint16_t operand);
void CLD_(CpuState* cpu, const uint16_t operand);
void CLI_(CpuState* cpu, const uint16_t operand);
void CLV_(CpuState* cpu, const uint16_t operand);
void CMP_(CpuState* cpu, const uint16_t operand);
void CPX_(CpuState* cpu, const uint16_t operand);
void CPY_(CpuState* cpu, const uint16_t operand);
void DEC_(CpuState* cpu, const uint16_t operand);
void DEX_(CpuState* cpu, const uint16_t operand);
void DEY_(CpuState* cpu, const uint16_t operand);
void EOR_(CpuState* cpu, const uint16_t operand);
void INC_(CpuState* cpu, const uint16_t operand);
void INX_(CpuState* cpu, const uint16_t operand);
void INY_(CpuState* cpu, const uint16_t operand);
void JMP_(CpuState* cpu, const uint16_t operand);
void JSR_(CpuState* cpu, const uint16_t operand);
void LDA_(CpuState* cpu, const uint16_t operand);
void LDX_(CpuState* cpu, const uint16_t operand);
void LDY_(CpuState* cpu, const uint16_t operand);
void LSR_(CpuState* cpu, const uint16_t operand);
void NOP_(CpuState* cpu, const uint16_t operand);
void ORA_(CpuState* cpu, const uint16_t operand);
void PHA_(CpuState* cpu, const uint16_t operand);
void PHP_(CpuState* cpu, const uint16_t operand);
void PLA_(CpuState* cpu, const uint16_t operand);
void PLP_(CpuState* cpu, const uint16_t operand);
void ROL_(CpuState* cpu, const uint16_t operand);
void ROR_(CpuState* cpu, const uint16_t operand);
void RTI_(CpuState* cpu, const uint16_t operand);
void RTS_(CpuState* cpu, const uint16_t operand);
void TRS_(CpuState* cpu, const uint16_t operand);
void SBC_(CpuState* cpu, const uint16_t operand);
void SEC_(CpuState* cpu, const uint16_t operand);
void SED_(CpuState* cpu, const uint16_t operand);
void SEI_(CpuState* cpu, const uint16_t operand);
void STA_(CpuState* cpu, const uint16_t operand);
void STX_(CpuState* cpu, const uint16_t operand);
void STY_(CpuState* cpu, const uint16_t operand);
void TAX_(CpuState* cpu, const uint16_t operand);
void TAY_(CpuState* cpu, const uint16_t operand);
void TSX_(CpuState* cpu, const uint16_t operand);
void TXA_(CpuState* cpu, const uint16_t operand);
void TXS_(CpuState* cpu, const uint16_t operand);
void TYA_(CpuState* cpu, const uint16_t operand);
void ILLEGAL_INSTRUCTION_(CpuState* cpu, const uint16_t operand);


typedef void(*CpuInstructionFPtr)(CpuState* cpu, const uint16_t operand);
extern const CpuInstructionFPtr cpu_instruction_lookup[];

#endif
