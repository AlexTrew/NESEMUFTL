#include "cpu_addr_mode.h"
#include <stdint.h>

CpuAddressingModeData get_operand_with_immediate_addressing(const uint16_t pc, const uint16_t* bus){
  /*
    get operand using immediate mode addressing: simply get the next byte after the instruction as
    the operand
   */
  CpuAddressingModeData res = {.operand_addr=bus[pc+1], .pc_offset=1};
  return res;
}

CpuAddressingModeData get_operand_with_absolute_addressing(const uint16_t pc, const uint16_t* bus){
     uint16_t lo_order_bits = bus[pc+1];
     uint16_t hi_order_bits = bus[pc+2];
     CpuAddressingModeData res = {.operand_addr=(hi_order_bits << 8) | lo_order_bits, .pc_offset=2};
     return res;
}

CpuAddressingModeData get_operand_with_absolute_x_addressing(const uint16_t pc, const uint8_t x_reg, const uint16_t* bus){
  CpuAddressingModeData res = get_operand_with_absolute_addressing(pc, bus);
  res.operand_addr += x_reg;
  return res;
}

CpuAddressingModeData get_operand_with_absolute_y_addressing(const uint16_t pc, const uint8_t y_reg, const uint16_t* bus){
  CpuAddressingModeData res = get_operand_with_absolute_addressing(pc, bus);
  res.operand_addr += y_reg;
  return res;
}

CpuAddressingModeData get_operand_with_zero_page_addressing(const uint16_t pc, const uint16_t* bus){
  CpuAddressingModeData res = {.operand_addr=bus[pc+1], .pc_offset=1};
  return res;
}

CpuAddressingModeData get_operand_with_zero_page_x_offset_addressing(const uint16_t pc, const uint8_t x_reg, const uint16_t* bus){
  uint16_t addr = ((bus[pc] + x_reg) & 0x00FF);
  CpuAddressingModeData res = {.operand_addr=addr, .pc_offset=1};
  return res;
}

CpuAddressingModeData get_operand_with_zero_page_y_offset_addressing(const uint16_t pc, const uint8_t y_reg, const uint16_t* bus){
  uint16_t addr = ((bus[pc] + y_reg) & 0x00FF);
  CpuAddressingModeData res = {.operand_addr=addr, .pc_offset=1};
  return res;
}

CpuAddressingModeData get_operand_with_relative_addressing(const uint16_t pc, const uint16_t* bus){
  CpuAddressingModeData res = {.operand_addr=0, .pc_offset=bus[pc+1]};
  return res;
}
