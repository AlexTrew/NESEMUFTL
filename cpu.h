# include <stdint.h>
#include <stdbool.h>

/*
  This is the cpu. we are emulating the 6502 cpu. 

  see https://www.nesdev.org/wiki/CPU_ALL for details
  see the datasheet: https://www.princeton.edu/~mae412/HANDOUTS/Datasheets/6502.pdf

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
  ACCUMULATOR,
  ABSOLUTE,
  ZERO_PAGE,
  INDEXED_ZERO_PAGE,
  INDEXED_ABSOLUTE,
  IMPLIED,
  RELATIVE,
  INDIRECT_X,
  INDIRECT_Y,
  ABSOLUTE_INDIRECT,
  NONE
} AddrMode;

typedef struct {
  char* name;
  uint8_t mem_needed;
  uint8_t cycles_left;
  AddrMode addressing_mode;
} CpuInstruction;

typedef struct {
  unsigned long cpu_cycle_count;
  uint8_t pc; 
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

