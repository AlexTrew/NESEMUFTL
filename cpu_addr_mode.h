#ifndef CPU_ADDR_MODE
#define CPU_ADDR_MODE
#include <stdint.h>


typedef struct CpuAddressingModeResult{
  /* the result of an addressing mode.
     pc_pffset: the number of bytes by which using this addressing mode should increment the program counter.
     operand_addr: the memory address where the operand (if any) of the current instruction can be found.
     additional_cycles: sometimes, some addressing modes require the current instruction to take additional clock cycles;
     that is specified here. */
  
  int8_t pc_offset;
  uint16_t operand_addr;
  uint8_t additional_cycles;
} CpuAddressingModeResult;


CpuAddressingModeResult get_operand_with_immediate_addressing(const uint16_t pc, const uint16_t* bus);
CpuAddressingModeResult get_operand_with_absolute_addressing(const uint16_t pc, const uint16_t* bus);
CpuAddressingModeResult get_operand_with_absolute_x_addressing(const uint16_t pc, const uint8_t x_reg, const uint16_t* bus);
CpuAddressingModeResult get_operand_with_absolute_y_addressing(const uint16_t pc, const uint8_t y_reg, const uint16_t* bus);
CpuAddressingModeResult get_operand_with_zero_page_addressing(const uint16_t pc, const uint16_t* bus);
CpuAddressingModeResult get_operand_with_zero_page_x_offset_addressing(const uint16_t pc, const uint8_t x_reg, const uint16_t* bus);
CpuAddressingModeResult get_operand_with_zero_page_y_offset_addressing(const uint16_t pc, const uint8_t y_reg, const uint16_t* bus);
CpuAddressingModeResult get_operand_with_relative_addressing(const uint16_t pc, const uint16_t* bus);
CpuAddressingModeResult get_operand_with_indirect_x_addressing(const uint16_t pc, const uint8_t x_reg, const uint16_t* bus);

#endif
