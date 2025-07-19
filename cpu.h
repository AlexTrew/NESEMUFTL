# include <stdint.h>
#include <stdbool.h>

/*
  This is the cpu. we are emulating the 6502 cpu. 

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

typedef enum{
  ACCUM,
  IMM,
  ABSOLUTE,
  ZP,
  ZP_X,
  ZP_Y,
  ABS,
  ABS_X,
  ABS_Y,
  IMPLIED,
  RELATIVE,
  IND_X,
  IND_Y,
  INDIRECT,
  NONE
} AddrMode;

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
  AddrMode addressing_mode;
} CpuInstruction;

typedef struct {
  unsigned long cpu_cycle_count;
  uint16_t pc; 
  uint8_t a;
  uint8_t x;
  uint8_t y;
  uint8_t stkptr;
  uint8_t status;
  uint16_t* bus; 
} Cpu;

Cpu *init_cpu(uint16_t* bus);
void delete_cpu(Cpu* cpu);
void cpu_cycle(Cpu* cpu);

