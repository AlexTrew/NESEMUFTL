#ifndef CPU_ADDR_MODE
#define CPU_ADDR_MODE

#include <stdint.h>
#include "cpu_state.h"

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
  IND_X,
  IND_Y,
  INDIRECT,
  NONE
} CpuAddrMode;

typedef struct CpuAddressingModeResult{
  /* the result of an addressing mode.
     pc_pffset: the number of bytes by which using this addressing mode should increment the program counter.
     operand: the memory address or value to be used as the operand (if any) of the current instruction.
     additional_cycles: sometimes, some addressing modes require the current instruction to take additional clock cycles;
     that is specified here. */
  
  int8_t pc_offset;
  uint16_t operand;
  uint8_t additional_cycles;
} CpuAddressingModeResult;

CpuAddressingModeResult immediate_addressing_mode(const CpuState* cpu);
CpuAddressingModeResult absolute_addressing_mode(const CpuState* cpu);
CpuAddressingModeResult absolute_x_addressing_mode(const CpuState* cpu);
CpuAddressingModeResult absolute_y_addressing_mode(const CpuState* cpu);
CpuAddressingModeResult zero_page_addressing_mode(const CpuState* cpu);
CpuAddressingModeResult zero_page_x_offset_addressing_mode(const CpuState* cpu);
CpuAddressingModeResult zero_page_y_offset_addressing_mode(const CpuState* cpu);
CpuAddressingModeResult relative_addressing_mode(const CpuState* cpu);
CpuAddressingModeResult indirect_addressing_mode(const CpuState* cpu);
CpuAddressingModeResult indirect_x_addressing_mode(const CpuState* cpu);
CpuAddressingModeResult indirect_y_addressing_mode(const CpuState* cpu);
CpuAddressingModeResult implied_addressing_mode(const CpuState* cpu);
CpuAddressingModeResult accumulator_addressing_mode(const CpuState* cpu);
CpuAddressingModeResult invalid_addressing_mode(const CpuState* cpu);

typedef CpuAddressingModeResult (*AddrModeFptr)(const CpuState* cpu);
extern const AddrModeFptr addr_mode_lookup[];

#endif
