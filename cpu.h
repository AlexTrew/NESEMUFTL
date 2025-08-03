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

CpuInstruction* cpu_cycle(CpuState* cpu);

uint8_t ADC_(CpuState* cpu, uint8_t operand);
uint8_t AND_(CpuState* cpu, uint8_t operand);
uint8_t ASL_(CpuState* cpu, uint8_t operand);
uint8_t BCC_(CpuState* cpu, uint8_t operand);
uint8_t BCS_(CpuState* cpu, uint8_t operand);
uint8_t BEQ_(CpuState* cpu, uint8_t operand);
uint8_t BIT_(CpuState* cpu, uint8_t operand);
uint8_t BMI_(CpuState* cpu, uint8_t operand);
uint8_t BNE_(CpuState* cpu, uint8_t operand);
uint8_t BPL_(CpuState* cpu, uint8_t operand);
uint8_t BRK_(CpuState* cpu, uint8_t operand);
uint8_t BVC_(CpuState* cpu, uint8_t operand);
uint8_t BVS_(CpuState* cpu, uint8_t operand);
uint8_t CLC_(CpuState* cpu, uint8_t operand);
uint8_t CLD_(CpuState* cpu, uint8_t operand);
uint8_t CLI_(CpuState* cpu, uint8_t operand);
uint8_t CLV_(CpuState* cpu, uint8_t operand);
uint8_t CMP_(CpuState* cpu, uint8_t operand);
uint8_t CPX_(CpuState* cpu, uint8_t operand);
uint8_t CPY_(CpuState* cpu, uint8_t operand);
uint8_t DEC_(CpuState* cpu, uint8_t operand);
uint8_t DEX_(CpuState* cpu, uint8_t operand);
uint8_t DEY_(CpuState* cpu, uint8_t operand);
uint8_t EOR_(CpuState* cpu, uint8_t operand);
uint8_t INC_(CpuState* cpu, uint8_t operand);
uint8_t INX_(CpuState* cpu, uint8_t operand);
uint8_t INY_(CpuState* cpu, uint8_t operand);
uint8_t JMP_(CpuState* cpu, uint8_t operand);
uint8_t JSR_(CpuState* cpu, uint8_t operand);
uint8_t LDA_(CpuState* cpu, uint8_t operand);
uint8_t LDX_(CpuState* cpu, uint8_t operand);
uint8_t LDY_(CpuState* cpu, uint8_t operand);
uint8_t LSR_(CpuState* cpu, uint8_t operand);
uint8_t NOP_(CpuState* cpu, uint8_t operand);
uint8_t ORA_(CpuState* cpu, uint8_t operand);
uint8_t PHA_(CpuState* cpu, uint8_t operand);
uint8_t PHP_(CpuState* cpu, uint8_t operand);
uint8_t PLA_(CpuState* cpu, uint8_t operand);
uint8_t PLP_(CpuState* cpu, uint8_t operand);
uint8_t ROL_(CpuState* cpu, uint8_t operand);
uint8_t ROR_(CpuState* cpu, uint8_t operand);
uint8_t RTI_(CpuState* cpu, uint8_t operand);
uint8_t RTS_(CpuState* cpu, uint8_t operand);
uint8_t TRS_(CpuState* cpu, uint8_t operand);
uint8_t SBC_(CpuState* cpu, uint8_t operand);
uint8_t SEC_(CpuState* cpu, uint8_t operand);
uint8_t SED_(CpuState* cpu, uint8_t operand);
uint8_t SEI_(CpuState* cpu, uint8_t operand);
uint8_t STA_(CpuState* cpu, uint8_t operand);
uint8_t STX_(CpuState* cpu, uint8_t operand);
uint8_t STY_(CpuState* cpu, uint8_t operand);
uint8_t TAX_(CpuState* cpu, uint8_t operand);
uint8_t TAY_(CpuState* cpu, uint8_t operand);
uint8_t TSX_(CpuState* cpu, uint8_t operand);
uint8_t TXA_(CpuState* cpu, uint8_t operand);
uint8_t TXS_(CpuState* cpu, uint8_t operand);
uint8_t TYA_(CpuState* cpu, uint8_t operand);

#endif
