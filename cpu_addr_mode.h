#include <stdint.h>


typedef struct CpuAddressingModeData{
  int8_t pc_offset;
  uint16_t operand_addr;
} CpuAddressingModeData;


CpuAddressingModeData get_operand_with_immediate_addressing(const uint16_t pc, const uint16_t* bus);
CpuAddressingModeData get_operand_with_absolute_addressing(const uint16_t pc, const uint16_t* bus);
CpuAddressingModeData get_operand_with_absolute_x_addressing(const uint16_t pc, const uint8_t x_reg, const uint16_t* bus);
CpuAddressingModeData get_operand_with_absolute_y_addressing(const uint16_t pc, const uint8_t y_reg, const uint16_t* bus);
CpuAddressingModeData get_operand_with_zero_page_addressing(const uint16_t pc, const uint16_t* bus);
CpuAddressingModeData get_operand_with_zero_page_x_offset_addressing(const uint16_t pc, const uint8_t x_reg, const uint16_t* bus);
CpuAddressingModeData get_operand_with_zero_page_y_offset_addressing(const uint16_t pc, const uint8_t y_reg, const uint16_t* bus);
CpuAddressingModeData get_operand_with_relative_addressing(const uint16_t pc, const uint16_t* bus);


