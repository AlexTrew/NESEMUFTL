#include "cpu_addr_mode.h"
#include <stdbool.h>
#include <stdint.h>

CpuAddressingModeResult immediate_addressing_mode(const CpuState* cpu){
  /*
    get operand_address using immediate mode addressing: simply get the next byte after the instruction as
    the operand_address
   */
  CpuAddressingModeResult res = {.operand_address=cpu->pc+1, .additional_cycles=0};
  return res;
}

CpuAddressingModeResult absolute_addressing_mode(const CpuState* cpu){
  uint16_t lo_order_bits = cpu->bus[cpu->pc+1] & 0x00FF;
  uint16_t hi_order_bits = cpu->bus[cpu->pc+2] & 0x00FF;
  CpuAddressingModeResult res = {.operand_address=(hi_order_bits << 8) | lo_order_bits, .additional_cycles=0};
  return res;
}

CpuAddressingModeResult absolute_x_addressing_mode(const CpuState* cpu){
  CpuAddressingModeResult res = absolute_addressing_mode(cpu);
  res.operand_address += cpu->x;
  return res;
}

CpuAddressingModeResult absolute_y_addressing_mode(const CpuState* cpu){
  CpuAddressingModeResult res = absolute_addressing_mode(cpu);
  res.operand_address += cpu->y;
  return res;
}

CpuAddressingModeResult zero_page_addressing_mode(const CpuState* cpu){
  CpuAddressingModeResult res = {.operand_address=cpu->bus[cpu->pc+1], .additional_cycles=0};
  return res;
}

CpuAddressingModeResult zero_page_x_offset_addressing_mode(const CpuState* cpu){
  uint16_t addr = ((cpu->bus[cpu->pc+1] + cpu->x) & 0x00FF);
  CpuAddressingModeResult res = {.operand_address=addr, .additional_cycles=0};
  return res;
}

CpuAddressingModeResult zero_page_y_offset_addressing_mode(const CpuState* cpu){
  uint16_t addr = ((cpu->bus[cpu->pc+1] + cpu->y) & 0x00FF);
  CpuAddressingModeResult res = {.operand_address=addr, .additional_cycles=0};
  return res;
}

CpuAddressingModeResult relative_addressing_mode(const CpuState* cpu){
  CpuAddressingModeResult res = {.operand_address=cpu->bus[cpu->pc+1]};
  return res;
}

CpuAddressingModeResult indirect_addressing_mode(const CpuState* cpu){
  uint16_t lo = cpu->bus[cpu->pc+1] & 0x00FF;
  uint16_t hi = cpu->bus[cpu->pc+2] & 0x00FF;
  uint16_t lookup_addr = (hi << 8 | lo);

  uint16_t effective_addr_lo = cpu->bus[lookup_addr] & 0x00FF;
  uint16_t effective_addr_hi = cpu->bus[lookup_addr+1] & 0x00FF;

  uint16_t effective_addr = (effective_addr_hi << 8 | effective_addr_lo);

  CpuAddressingModeResult res = {.operand_address=effective_addr, .additional_cycles=2};
  return res;
}

CpuAddressingModeResult indirect_x_addressing_mode(const CpuState* cpu){
  uint16_t lookup_addr = (cpu->bus[cpu->pc+1]) + cpu->x;

  uint8_t additional_cycles = 0;
  if (lookup_addr > 0x00FF) {
    lookup_addr &= 0x00FF; 
    additional_cycles=1;
  }    
  
  uint16_t effective_addr_lo = cpu->bus[lookup_addr];
  uint16_t effective_addr_hi = cpu->bus[lookup_addr+1];
  uint16_t effective_addr = (effective_addr_hi << 8 | effective_addr_lo);

  CpuAddressingModeResult res = {.operand_address=effective_addr, .additional_cycles=2+additional_cycles};
  return res;
}

CpuAddressingModeResult indirect_y_addressing_mode(const CpuState* cpu){
  uint16_t lookup_addr = (cpu->bus[cpu->pc+1]);

  uint8_t additional_cycles = 0;
  if (lookup_addr > 0x00FF) {
    lookup_addr &= 0x00FF; 
    additional_cycles=1;
  }    
  
  uint16_t effective_addr_lo = cpu->bus[lookup_addr];
  uint16_t effective_addr_hi = cpu->bus[lookup_addr+1];
  uint16_t effective_addr = (effective_addr_hi << 8 | effective_addr_lo) + cpu->y;

  CpuAddressingModeResult res = {.operand_address=effective_addr, .additional_cycles=2+additional_cycles};
  return res;
}

CpuAddressingModeResult implied_addressing_mode(const CpuState* cpu){
  /* We do nothing, so we just return 0 for all values */
  CpuAddressingModeResult res = {.operand_address=0, .additional_cycles=0};
  return res;
}

CpuAddressingModeResult accumulator_addressing_mode(const CpuState* cpu){
  /* We do nothing, so we just return 0 for all values */
  CpuAddressingModeResult res = {.operand_address=0, .additional_cycles=0};
  return res;
}

CpuAddressingModeResult invalid_addressing_mode(const CpuState* cpu){
  /* since invalid instructions have no addressing mode, just return a result
   where all values are 0. This means that the cpu will just load the next instruction */
  CpuAddressingModeResult res = {.operand_address=0, .additional_cycles=0};
  return res;
}

const AddrModeFptr addr_mode_lookup[] = {
  [ACCUM] = accumulator_addressing_mode,
  [IMM] = immediate_addressing_mode,
  [ZP] = zero_page_addressing_mode,
  [ZP_X] = zero_page_x_offset_addressing_mode,
  [ZP_Y] = zero_page_y_offset_addressing_mode,
  [ABS] = absolute_addressing_mode,
  [ABS_X] = absolute_x_addressing_mode,
  [ABS_Y] = absolute_y_addressing_mode,
  [IMPLIED] = implied_addressing_mode,
  [RELATIVE] = relative_addressing_mode,
  [IND_X] = indirect_x_addressing_mode,
  [IND_Y] = indirect_y_addressing_mode,
  [NONE] = invalid_addressing_mode
};
