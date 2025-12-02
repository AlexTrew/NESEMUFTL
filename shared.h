#ifndef SHARED
#define SHARED

#include <stdint.h>

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

typedef enum{
  ACCUM,
  IMM,
  ZP,
  ZP_X,
  ZP_Y,
  ABS,
  ABS_X,
  ABS_Y,
  IMPLIED,
  RELATIVE,
  ABS_IND_X,
  IND_X,
  IND_Y,
  INDIRECT,
  NONE
} CpuAddrMode;


typedef struct CpuInstruction{
  CpuInstructionName name;
  const char* name_str;
  uint8_t size_in_bytes;
  uint8_t cycles_left;
  CpuAddrMode addressing_mode;
} CpuInstruction;


extern const int NUMBER_OF_INSTRUCTIONS;

extern const CpuInstruction opcode_x_cpu_instruction_lookup[];

#endif
