#ifndef CPU
#define CPU

#include <stdint.h>
#include <stdbool.h>

#include "cpu_addr_mode.h"
#include "instructions.h"
#include "cpu_state.h"

/*
  This is the cpu. we are emulating the MOS 6502. 

  this is the best document: https://www.masswerk.at/6502/6502_instruction_set.html

  see https://www.nesdev.org/wiki/CPU_ALL for details
  see the datasheets:
  https://www.princeton.edu/~mae412/HANDOUTS/Datasheets/6502.pdf
  https://web.archive.org/web/20221112231348if_/http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf

  notes:
  - instructions recieved by the cpu are different sizes (1,2, or 3), and can take different
  numbers of clock cycles to execute!
  - The first byte of the instruction contains this information
 */




typedef struct CpuInstruction{
  CpuInstructionName name;
  uint8_t size_in_bytes;
  uint8_t cycles_left;
  CpuAddrMode addressing_mode;
} CpuInstruction;


CpuState *init_cpu(uint8_t* bus);

void delete_cpu(CpuState* cpu);

void cpu_cycle(CpuState* cpu);

#endif
