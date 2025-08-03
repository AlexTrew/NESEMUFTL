#include "cpu_addr_mode.h"
#include <stdbool.h>
#include <stdint.h>

CpuAddressingModeResult immediate_addressing_mode(const CpuState* cpu){
  /*
    get operand using immediate mode addressing: simply get the next byte after the instruction as
    the operand
   */
  CpuAddressingModeResult res = {.operand=cpu->bus[cpu->pc+1], .pc_offset=1};
  return res;
}

CpuAddressingModeResult absolute_addressing_mode(const CpuState* cpu){
     uint16_t lo_order_bits = cpu->bus[cpu->pc+1];
     uint16_t hi_order_bits = cpu->bus[cpu->pc+2];
     CpuAddressingModeResult res = {.operand=(hi_order_bits << 8) | lo_order_bits, .pc_offset=2};
     return res;
}

CpuAddressingModeResult absolute_x_addressing_mode(const CpuState* cpu){
  CpuAddressingModeResult res = absolute_addressing_mode(cpu);
  res.operand += cpu->x;
  return res;
}

CpuAddressingModeResult absolute_y_addressing_mode(const CpuState* cpu){
  CpuAddressingModeResult res = absolute_addressing_mode(cpu);
  res.operand += cpu->y;
  return res;
}

CpuAddressingModeResult zero_page_addressing_mode(const CpuState* cpu){
  CpuAddressingModeResult res = {.operand=cpu->bus[cpu->pc+1], .pc_offset=1};
  return res;
}

CpuAddressingModeResult zero_page_x_offset_addressing_mode(const CpuState* cpu){
  uint16_t addr = ((cpu->bus[cpu->pc+1] + cpu->x) & 0x00FF);
  CpuAddressingModeResult res = {.operand=addr, .pc_offset=1};
  return res;
}

CpuAddressingModeResult zero_page_y_offset_addressing_mode(const CpuState* cpu){
  uint16_t addr = ((cpu->bus[cpu->pc+1] + cpu->y) & 0x00FF);
  CpuAddressingModeResult res = {.operand=addr, .pc_offset=1};
  return res;
}

CpuAddressingModeResult relative_addressing_mode(const CpuState* cpu){
  CpuAddressingModeResult res = {.operand=0, .pc_offset=cpu->bus[cpu->pc+1]};
  return res;
}

CpuAddressingModeResult indirect_addressing_mode(const CpuState* cpu){
}

CpuAddressingModeResult indirect_x_addressing_mode(const CpuState* cpu){
  uint16_t lo = (cpu->bus[cpu->pc+cpu->x+1] & 0x00FF);
  uint16_t hi = (cpu->bus[cpu->pc+cpu->x+2] & 0x00FF);
  uint16_t addr = (hi <<8 | lo);
  CpuAddressingModeResult res = {.operand=addr, .pc_offset=1};
  return res;
}

CpuAddressingModeResult indirect_y_addressing_mode(const CpuState* cpu){
}

CpuAddressingModeResult implied_addressing_mode(const CpuState* cpu){
}

CpuAddressingModeResult accumulator_addressing_mode(const CpuState* cpu){
}

CpuAddressingModeResult invalid_addressing_mode(const CpuState* cpu){
  CpuAddressingModeResult res = {.operand=0, .pc_offset=0};
  return res;
}

const AddrModeFptr addr_mode_lookup[] = {
  [ACCUM] = accumulator_addressing_mode,
  [IMM] = immediate_addressing_mode,
  [ZP] = zero_page_addressing_mode,
  [ZP_X] = zero_page_x_offset_addressing_mode,
  [ZP_Y] = zero_page_y_offset_addressing_mode,
  [ABSOLUTE] = absolute_addressing_mode,
  [ABS_X] = absolute_x_addressing_mode,
  [ABS_Y] = absolute_y_addressing_mode,
  [IMPLIED] = implied_addressing_mode,
  [RELATIVE] = relative_addressing_mode,
  [IND_X] = indirect_addressing_mode,
  [IND_Y] = indirect_y_addressing_mode,
  [INDIRECT] = indirect_addressing_mode,
  [NONE] = invalid_addressing_mode
};
