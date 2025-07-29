# include <stdint.h>
#include <stdbool.h>

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

typedef struct CpuInstruction{
  CpuInstructionName name;
  uint8_t mem_needed;
  uint8_t cycles_left;
  // uint8_t (*instruction_func)(Cpu* cpu, const uint8_t operand);
  // uint8_t (*addr_mode_func)(Cpu* cpu);
  AddrMode addressing_mode;
} CpuInstruction;

Cpu *init_cpu(uint16_t* bus);
void delete_cpu(Cpu* cpu);

CpuInstruction* cpu_cycle(Cpu* cpu);

uint8_t ADC_(Cpu* cpu, uint8_t operand);
uint8_t AND_(Cpu* cpu, uint8_t operand);
uint8_t ASL_(Cpu* cpu, uint8_t operand);
uint8_t BCC_(Cpu* cpu, uint8_t operand);
uint8_t BCS_(Cpu* cpu, uint8_t operand);
uint8_t BEQ_(Cpu* cpu, uint8_t operand);
uint8_t BIT_(Cpu* cpu, uint8_t operand);
uint8_t BMI_(Cpu* cpu, uint8_t operand);
uint8_t BNE_(Cpu* cpu, uint8_t operand);
uint8_t BPL_(Cpu* cpu, uint8_t operand);
uint8_t BRK_(Cpu* cpu, uint8_t operand);
uint8_t BVC_(Cpu* cpu, uint8_t operand);
uint8_t BVS_(Cpu* cpu, uint8_t operand);
uint8_t CLC_(Cpu* cpu, uint8_t operand);
uint8_t CLD_(Cpu* cpu, uint8_t operand);
uint8_t CLI_(Cpu* cpu, uint8_t operand);
uint8_t CLV_(Cpu* cpu, uint8_t operand);
uint8_t CMP_(Cpu* cpu, uint8_t operand);
uint8_t CPX_(Cpu* cpu, uint8_t operand);
uint8_t CPY_(Cpu* cpu, uint8_t operand);
uint8_t DEC_(Cpu* cpu, uint8_t operand);
uint8_t DEX_(Cpu* cpu, uint8_t operand);
uint8_t DEY_(Cpu* cpu, uint8_t operand);
uint8_t EOR_(Cpu* cpu, uint8_t operand);
uint8_t INC_(Cpu* cpu, uint8_t operand);
uint8_t INX_(Cpu* cpu, uint8_t operand);
uint8_t INY_(Cpu* cpu, uint8_t operand);
uint8_t JMP_(Cpu* cpu, uint8_t operand);
uint8_t JSR_(Cpu* cpu, uint8_t operand);
uint8_t LDA_(Cpu* cpu, uint8_t operand);
uint8_t LDX_(Cpu* cpu, uint8_t operand);
uint8_t LDY_(Cpu* cpu, uint8_t operand);
uint8_t LSR_(Cpu* cpu, uint8_t operand);
uint8_t NOP_(Cpu* cpu, uint8_t operand);
uint8_t ORA_(Cpu* cpu, uint8_t operand);
uint8_t PHA_(Cpu* cpu, uint8_t operand);
uint8_t PHP_(Cpu* cpu, uint8_t operand);
uint8_t PLA_(Cpu* cpu, uint8_t operand);
uint8_t PLP_(Cpu* cpu, uint8_t operand);
uint8_t ROL_(Cpu* cpu, uint8_t operand);
uint8_t ROR_(Cpu* cpu, uint8_t operand);
uint8_t RTI_(Cpu* cpu, uint8_t operand);
uint8_t RTS_(Cpu* cpu, uint8_t operand);
uint8_t TRS_(Cpu* cpu, uint8_t operand);
uint8_t SBC_(Cpu* cpu, uint8_t operand);
uint8_t SEC_(Cpu* cpu, uint8_t operand);
uint8_t SED_(Cpu* cpu, uint8_t operand);
uint8_t SEI_(Cpu* cpu, uint8_t operand);
uint8_t STA_(Cpu* cpu, uint8_t operand);
uint8_t STX_(Cpu* cpu, uint8_t operand);
uint8_t STY_(Cpu* cpu, uint8_t operand);
uint8_t TAX_(Cpu* cpu, uint8_t operand);
uint8_t TAY_(Cpu* cpu, uint8_t operand);
uint8_t TSX_(Cpu* cpu, uint8_t operand);
uint8_t TXA_(Cpu* cpu, uint8_t operand);
uint8_t TXS_(Cpu* cpu, uint8_t operand);
uint8_t TYA_(Cpu* cpu, uint8_t operand);
