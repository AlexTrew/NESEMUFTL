#ifndef CPU
#define CPU

#include <stdint.h>
#include <stdbool.h>

#include "cpu_addr_mode.h"
#include "cpu_state.h"

/*
  This is the cpu. we are emulating the MOS 6502. 

  see https://www.nesdev.org/wiki/CPU_ALL for details
  see the datasheets:
  https://www.princeton.edu/~mae412/HANDOUTS/Datasheets/6502.pdf
  https://web.archive.org/web/20221112231348if_/http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf

  notes:
  - instructions recieved by the cpu are different sizes (1,2, or 3), and can take different
  numbers of clock cycles to execute!
  - The first byte of the instruction contains this information
 */

typedef enum {
  C = (1 << 0),
  Z = (1 << 1),
  I = (1 << 2),
  D = (1 << 3),
  B = (1 << 4),
  U = (1 << 5),
  O = (1 << 6),
  N = (1 << 7),
} CpuStatusFlag;


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

typedef struct CpuInstruction{
  CpuInstructionName name;
  uint8_t mem_needed;
  uint8_t cycles_left;
  CpuAddrMode addressing_mode;
} CpuInstruction;


CpuState *init_cpu(uint8_t* bus);

void delete_cpu(CpuState* cpu);

void cpu_cycle(CpuState* cpu);

#endif
