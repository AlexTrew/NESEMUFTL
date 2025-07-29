#include "cpu.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
  See page 10 of archive.6502.org/datasheets/rockwell_r650x_r651x.pdf for a
  complete opcode -> instruction reference.
 */  
const CpuInstruction cpu_instruction_lookup[] = {
  [0x00] = {.name=BRK, .cycles_left=7, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x01] = {.name=ORA, .cycles_left=5, .mem_needed=2, .addressing_mode=IND_X},
  [0x02] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x03] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x04] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x05] = {.name=ORA, .cycles_left=3, .mem_needed=2, .addressing_mode=ZP},
  [0x06] = {.name=ASL, .cycles_left=5, .mem_needed=2, .addressing_mode=ZP},
  [0x07] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x08] = {.name=PHP, .cycles_left=3, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x09] = {.name=ORA, .cycles_left=2, .mem_needed=2, .addressing_mode=IMM},
  [0x0A] = {.name=ASL, .cycles_left=2, .mem_needed=1, .addressing_mode=ACCUM},
  [0x0B] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x0C] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x0D] = {.name=ORA, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0x0E] = {.name=ASL, .cycles_left=6, .mem_needed=3, .addressing_mode=ABS},
  [0x0F] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x10] = {.name=BPL, .cycles_left=2, .mem_needed=2, .addressing_mode=RELATIVE},
  [0x11] = {.name=ORA, .cycles_left=5, .mem_needed=2, .addressing_mode=IND_X},
  [0x12] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x13] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x14] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x15] = {.name=ORA, .cycles_left=5, .mem_needed=2, .addressing_mode=ZP},
  [0x16] = {.name=ASL, .cycles_left=6, .mem_needed=2, .addressing_mode=ZP},
  [0x17] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x18] = {.name=CLC, .cycles_left=2, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x19] = {.name=ORA, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0x1A] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x1B] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x1C] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x1D] = {.name=ORA, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS_X},
  [0x1E] = {.name=ASL, .cycles_left=6, .mem_needed=4, .addressing_mode=ABS_X},
  [0x1F] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x20] = {.name=JSR, .cycles_left=6, .mem_needed=3, .addressing_mode=ABSOLUTE},
  [0x21] = {.name=AND, .cycles_left=6, .mem_needed=2, .addressing_mode=IND_X},
  [0x22] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x23] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x24] = {.name=BIT, .cycles_left=3, .mem_needed=2, .addressing_mode=ZP},
  [0x25] = {.name=AND, .cycles_left=3, .mem_needed=2, .addressing_mode=ZP},
  [0x26] = {.name=ROL, .cycles_left=5, .mem_needed=2, .addressing_mode=ZP},
  [0x27] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x28] = {.name=PLP, .cycles_left=4, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x29] = {.name=AND, .cycles_left=2, .mem_needed=2, .addressing_mode=IMM},
  [0x2A] = {.name=ROL, .cycles_left=2, .mem_needed=1, .addressing_mode=ACCUM},
  [0x2B] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x2C] = {.name=BIT, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0x2D] = {.name=AND, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0x2E] = {.name=ROL, .cycles_left=6, .mem_needed=3, .addressing_mode=ABS},
  [0x2F] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x30] = {.name=BMI, .cycles_left=2, .mem_needed=2, .addressing_mode=RELATIVE},
  [0x31] = {.name=AND, .cycles_left=6, .mem_needed=2, .addressing_mode=IND_Y},
  [0x32] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x33] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x34] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x35] = {.name=AND, .cycles_left=4, .mem_needed=2, .addressing_mode=ZP_X},
  [0x36] = {.name=ROL, .cycles_left=6, .mem_needed=2, .addressing_mode=ZP_X},
  [0x37] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x38] = {.name=SEC, .cycles_left=2, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x39] = {.name=AND, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS_Y},
  [0x3A] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x3B] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x3C] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x3D] = {.name=AND, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS_X},
  [0x3E] = {.name=ROL, .cycles_left=7, .mem_needed=3, .addressing_mode=ABS_X},
  [0x3F] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x40] = {.name=RTI, .cycles_left=6, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x41] = {.name=EOR, .cycles_left=6, .mem_needed=2, .addressing_mode=IND_Y},
  [0x42] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x43] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x44] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x45] = {.name=EOR, .cycles_left=3, .mem_needed=2, .addressing_mode=ZP},
  [0x46] = {.name=LSR, .cycles_left=5, .mem_needed=2, .addressing_mode=ZP},
  [0x47] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x48] = {.name=PHA, .cycles_left=3, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x49] = {.name=EOR, .cycles_left=2, .mem_needed=2, .addressing_mode=IMM},
  [0x4A] = {.name=LSR, .cycles_left=2, .mem_needed=1, .addressing_mode=ACCUM},
  [0x4B] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x4C] = {.name=JMP, .cycles_left=3, .mem_needed=3, .addressing_mode=ABS},
  [0x4D] = {.name=EOR, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0x4E] = {.name=LSR, .cycles_left=6, .mem_needed=3, .addressing_mode=ABS},
  [0x4F] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x50] = {.name=BVC, .cycles_left=2, .mem_needed=2, .addressing_mode=RELATIVE},
  [0x51] = {.name=EOR, .cycles_left=5, .mem_needed=2, .addressing_mode=IND_Y},
  [0x52] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x53] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x54] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x55] = {.name=EOR, .cycles_left=4, .mem_needed=2, .addressing_mode=ZP_X},
  [0x56] = {.name=LSR, .cycles_left=6, .mem_needed=2, .addressing_mode=ZP_X},
  [0x57] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x58] = {.name=CLI, .cycles_left=2, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x59] = {.name=EOR, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS_Y},
  [0x5A] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x5B] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x5C] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x5D] = {.name=EOR, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS_X},
  [0x5E] = {.name=LSR, .cycles_left=7, .mem_needed=3, .addressing_mode=ABS_X},
  [0x5F] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x60] = {.name=RTS, .cycles_left=6, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x62] = {.name=ADC, .cycles_left=6, .mem_needed=2, .addressing_mode=IND_X},
  [0x63] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x64] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x65] = {.name=ADC, .cycles_left=3, .mem_needed=2, .addressing_mode=ZP},
  [0x66] = {.name=ROR, .cycles_left=5, .mem_needed=2, .addressing_mode=ZP},
  [0x67] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x68] = {.name=PLA, .cycles_left=4, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x69] = {.name=ADC, .cycles_left=2, .mem_needed=2, .addressing_mode=ABSOLUTE},
  [0x6A] = {.name=ROR, .cycles_left=2, .mem_needed=1, .addressing_mode=ACCUM},
  [0x6B] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x6C] = {.name=JMP, .cycles_left=5, .mem_needed=3, .addressing_mode=INDIRECT},
  [0x6D] = {.name=ADC, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0x6E] = {.name=ROR, .cycles_left=6, .mem_needed=3, .addressing_mode=ABS},
  [0x6F] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x70] = {.name=BVS, .cycles_left=2, .mem_needed=2, .addressing_mode=RELATIVE},
  [0x71] = {.name=ADC, .cycles_left=5, .mem_needed=2, .addressing_mode=IND_Y},
  [0x72] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x73] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x74] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x75] = {.name=ADC, .cycles_left=4, .mem_needed=2, .addressing_mode=ZP_X},
  [0x76] = {.name=ROR, .cycles_left=6, .mem_needed=2, .addressing_mode=ZP_X},
  [0x77] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x78] = {.name=SEI, .cycles_left=2, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x79] = {.name=ADC, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS_Y},
  [0x7A] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x7B] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x7C] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x7D] = {.name=ADC, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS_X},
  [0x7E] = {.name=ROR, .cycles_left=7, .mem_needed=3, .addressing_mode=ABS_X},
  [0x7F] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x80] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x81] = {.name=STA, .cycles_left=6, .mem_needed=2, .addressing_mode=IND_X},
  [0x82] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x83] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x84] = {.name=STY, .cycles_left=3, .mem_needed=2, .addressing_mode=ZP},
  [0x85] = {.name=STA, .cycles_left=3, .mem_needed=2, .addressing_mode=ZP},
  [0x86] = {.name=STX, .cycles_left=3, .mem_needed=2, .addressing_mode=ZP},
  [0x87] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x88] = {.name=DEY, .cycles_left=2, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x89] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x8A] = {.name=DEY, .cycles_left=2, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x8B] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x8C] = {.name=STY, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0x8D] = {.name=STA, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0x8E] = {.name=STX, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0x8F] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x90] = {.name=BCC, .cycles_left=2, .mem_needed=2, .addressing_mode=RELATIVE},
  [0x91] = {.name=STA, .cycles_left=6, .mem_needed=2, .addressing_mode=IND_Y},
  [0x92] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x93] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x94] = {.name=STY, .cycles_left=4, .mem_needed=2, .addressing_mode=ZP_X},
  [0x95] = {.name=STA, .cycles_left=4, .mem_needed=2, .addressing_mode=NONE},
  [0x96] = {.name=STX, .cycles_left=4, .mem_needed=2, .addressing_mode=NONE},
  [0x97] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x98] = {.name=TYA, .cycles_left=2, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x99] = {.name=STA, .cycles_left=5, .mem_needed=3, .addressing_mode=ABS_Y},
  [0x9A] = {.name=TXS, .cycles_left=2, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x9B] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x9C] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x9D] = {.name=STA, .cycles_left=5, .mem_needed=3, .addressing_mode=ABS_X},
  [0x9E] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x9F] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xA0] = {.name=LDY, .cycles_left=2, .mem_needed=2, .addressing_mode=NONE},
  [0xA1] = {.name=LDA, .cycles_left=6, .mem_needed=2, .addressing_mode=NONE},
  [0xA2] = {.name=LDX, .cycles_left=2, .mem_needed=2, .addressing_mode=NONE},
  [0xA3] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xA4] = {.name=LDA, .cycles_left=3, .mem_needed=2, .addressing_mode=IMM},
  [0xA5] = {.name=LDA, .cycles_left=3, .mem_needed=2, .addressing_mode=IND_X},
  [0xA6] = {.name=LDX, .cycles_left=3, .mem_needed=2, .addressing_mode=IMM},
  [0xA7] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xA8] = {.name=TAY, .cycles_left=2, .mem_needed=1, .addressing_mode=ZP},
  [0xA9] = {.name=LDA, .cycles_left=2, .mem_needed=2, .addressing_mode=ZP},
  [0xAA] = {.name=TAX, .cycles_left=2, .mem_needed=1, .addressing_mode=IMPLIED},
  [0xAB] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xAC] = {.name=LDY, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0xAD] = {.name=LDA, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0xAE] = {.name=LDX, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0xAF] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xB0] = {.name=BCS, .cycles_left=2, .mem_needed=2, .addressing_mode=RELATIVE},
  [0xB1] = {.name=LDA, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xB2] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xB3] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xB4] = {.name=LDY, .cycles_left=2, .mem_needed=2, .addressing_mode=ZP_X},
  [0xB5] = {.name=LDA, .cycles_left=4, .mem_needed=2, .addressing_mode=ZP_X},
  [0xB6] = {.name=LDX, .cycles_left=2, .mem_needed=2, .addressing_mode=ZP_Y},
  [0xB7] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xB8] = {.name=CLV, .cycles_left=4, .mem_needed=1, .addressing_mode=IMPLIED},
  [0xB9] = {.name=LDA, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS_Y},
  [0xBA] = {.name=TSX, .cycles_left=4, .mem_needed=1, .addressing_mode=IMPLIED},
  [0xBB] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xBC] = {.name=LDY, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS_X},
  [0xBD] = {.name=LDA, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS_X},
  [0xBE] = {.name=LDX, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS_Y},
  [0xBF] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xC0] = {.name=CPY, .cycles_left=2, .mem_needed=2, .addressing_mode=IMM},
  [0xC1] = {.name=CMP, .cycles_left=6, .mem_needed=2, .addressing_mode=IND_X},
  [0xC2] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xC3] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xC4] = {.name=CPY, .cycles_left=3, .mem_needed=2, .addressing_mode=ZP},
  [0xC5] = {.name=CMP, .cycles_left=3, .mem_needed=2, .addressing_mode=ZP},
  [0xC6] = {.name=DEC, .cycles_left=5, .mem_needed=2, .addressing_mode=ZP},
  [0xC7] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xC8] = {.name=INY, .cycles_left=2, .mem_needed=1, .addressing_mode=IMPLIED},
  [0xC9] = {.name=CMP, .cycles_left=2, .mem_needed=2, .addressing_mode=IMM},
  [0xCA] = {.name=DEX, .cycles_left=2, .mem_needed=1, .addressing_mode=IMPLIED},
  [0xCB] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xCC] = {.name=CPY, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0xCD] = {.name=CMP, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0xCE] = {.name=DEC, .cycles_left=6, .mem_needed=3, .addressing_mode=ABS},
  [0xCF] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xD0] = {.name=BNE, .cycles_left=2, .mem_needed=2, .addressing_mode=IMM},
  [0xD1] = {.name=CMP, .cycles_left=6, .mem_needed=2, .addressing_mode=IND_X},
  [0xD2] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xD3] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xD4] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xD5] = {.name=CMP, .cycles_left=4, .mem_needed=2, .addressing_mode=ZP_X},
  [0xD6] = {.name=DEC, .cycles_left=6, .mem_needed=2, .addressing_mode=ZP_X},
  [0xD7] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xD8] = {.name=CLD, .cycles_left=2, .mem_needed=1, .addressing_mode=IMPLIED},
  [0xD9] = {.name=CMP, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS_Y},
  [0xDA] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xDB] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xDC] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xDD] = {.name=CMP, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS_X},
  [0xDE] = {.name=DEC, .cycles_left=7, .mem_needed=3, .addressing_mode=ABS},
  [0xDF] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xE0] = {.name=CPX, .cycles_left=2, .mem_needed=2, .addressing_mode=IMM},
  [0xE1] = {.name=SBC, .cycles_left=6, .mem_needed=2, .addressing_mode=IND_X},
  [0xE2] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xE3] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xE4] = {.name=CPX, .cycles_left=3, .mem_needed=2, .addressing_mode=ZP},
  [0xE5] = {.name=SBC, .cycles_left=3, .mem_needed=2, .addressing_mode=ZP},
  [0xE6] = {.name=INC, .cycles_left=5, .mem_needed=2, .addressing_mode=ZP},
  [0xE7] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xE8] = {.name=INX, .cycles_left=2, .mem_needed=1, .addressing_mode=IMPLIED},
  [0xE9] = {.name=SBC, .cycles_left=2, .mem_needed=2, .addressing_mode=IMM},
  [0xEA] = {.name=NOP, .cycles_left=2, .mem_needed=1, .addressing_mode=IMPLIED},
  [0xEB] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xEC] = {.name=CPX, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0xED] = {.name=SBC, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0xEE] = {.name=INC, .cycles_left=6, .mem_needed=3, .addressing_mode=ABS},
  [0xEF] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xF0] = {.name=BEQ, .cycles_left=2, .mem_needed=2, .addressing_mode=RELATIVE},
  [0xF1] = {.name=SBC, .cycles_left=5, .mem_needed=2, .addressing_mode=IND_Y},
  [0xF2] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xF3] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xF4] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xF5] = {.name=SBC, .cycles_left=4, .mem_needed=2, .addressing_mode=ZP_X},
  [0xF6] = {.name=INC, .cycles_left=6, .mem_needed=2, .addressing_mode=ZP_X},
  [0xF7] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xF8] = {.name=SED, .cycles_left=2, .mem_needed=1, .addressing_mode=IMPLIED},
  [0xF9] = {.name=SBC, .cycles_left=2, .mem_needed=1, .addressing_mode=ABS_Y},
  [0xFA] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xFB] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xFC] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xFD] = {.name=SBC, .cycles_left=4, .mem_needed=3, .addressing_mode=ABS_X},
  [0xFE] = {.name=INC, .cycles_left=7, .mem_needed=3, .addressing_mode=ABS_X},
  [0xFF] = {.name=ILLEGAL_INSTRUCTION, .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
};


static uint16_t get_operand_with_immediate_addressing(const Cpu* cpu);
static uint16_t get_operand_with_absolute_addressing(const Cpu* cpu);
static uint16_t get_operand_with_absolute_x_addressing(const Cpu* cpu);
static uint16_t get_operand_with_absolute_y_addressing(const Cpu* cpu);
static uint16_t get_operand_with_zero_page_addressing(const Cpu* cpu);
static uint16_t get_operand_with_zero_page_x_offset_addressing(const Cpu* cpu);
static uint16_t get_operand_with_zero_page_y_offset_addressing(const Cpu* cpu);

static void process_instruction(Cpu* cpu, const CpuInstruction* instruction);
static void set_status_flag(Cpu* cpu, CpuStatusFlag f, bool v);
static uint8_t cpu_read_from_bus(Cpu* cpu, const uint16_t addr);
static void cpu_write(Cpu* cpu, const uint16_t addr, uint8_t data);

static uint8_t cpu_read_from_bus(Cpu* cpu, const uint16_t addr){
  return cpu->bus[addr];
}

static void cpu_write(Cpu* cpu, uint16_t addr, uint8_t data){
  cpu->bus[addr] = data;
}

static void set_status_flag(Cpu* cpu, CpuStatusFlag f, bool v){
  if(v){
    cpu->status |= f;
  }
  else{
    cpu->status &= ~f;
  }
}

static uint16_t get_operand_with_immediate_addressing(const Cpu* cpu){
  /*
    get operand using immediate mode addressing: simply get the next byte after the instruction as
    the operand
   */
  return cpu->bus[cpu->pc+1];
}

static uint16_t get_operand_with_absolute_addressing(const Cpu* cpu){
  uint16_t lo_order_bits = cpu->bus[cpu->pc+1];
  uint16_t hi_order_bits = cpu->bus[cpu->pc+2];
  return (hi_order_bits << 8) | lo_order_bits;
}

static uint16_t get_operand_with_absolute_x_addressing(const Cpu* cpu){
  return get_operand_with_absolute_addressing(cpu) + cpu->x;
}

static uint16_t get_operand_with_absolute_y_addressing(const Cpu* cpu){
  return get_operand_with_absolute_addressing(cpu) + cpu->y;
}

static uint16_t get_operand_with_zero_page_addressing(const Cpu* cpu){
  uint16_t addr = cpu->bus[cpu->pc+1];
  return (addr & 0x00FF);
}

static uint16_t get_operand_with_zero_page_x_offset_addressing(const Cpu* cpu){
  uint16_t addr = cpu->bus[cpu->pc+1];
  return ((addr + cpu->x) & 0x00FF);
}

static uint16_t get_operand_with_zero_page_y_offset_addressing(const Cpu* cpu){
  uint16_t addr = cpu->bus[cpu->pc+1];
  return ((addr + cpu->y) & 0x00FF);
}

static void process_instruction(Cpu* cpu, const CpuInstruction* instruction){
  //  uint8_t operand = instruction->addr_mode_func(cpu);
  //instruction->instruction_func(cpu, operand);
}

Cpu *init_cpu(uint16_t* memory){
  Cpu* cpu_ptr = (Cpu*)malloc(sizeof(Cpu));
  cpu_ptr->a=0x00;
  cpu_ptr->x=0x00;
  cpu_ptr->y=0x00;
  cpu_ptr->pc=0x0001;
  cpu_ptr->stkptr=0x00;
  cpu_ptr->status=0x00;
  cpu_ptr->cpu_cycle_count = 0;
  cpu_ptr->bus=memory;
  return cpu_ptr;
};

void delete_cpu(Cpu *cpu){
  free(cpu);
}

CpuInstruction* cpu_cycle(Cpu* cpu){
  static CpuInstruction* current_instruction_ptr = NULL;

  /* check if the current instruction is complete.
     if so, clean it up! */
  if(current_instruction_ptr && current_instruction_ptr->cycles_left == 0){
    free(current_instruction_ptr);
    current_instruction_ptr = NULL;
  }
  
  // If there is no active instruction, look one up using the op code pointed to by the
  // program counter.
  if(current_instruction_ptr == NULL){
    uint8_t opcode = cpu_read_from_bus(cpu, cpu->pc);
    current_instruction_ptr = (CpuInstruction*)malloc(sizeof(CpuInstruction));

    memcpy(current_instruction_ptr, &cpu_instruction_lookup[opcode], sizeof(CpuInstruction));
    if(current_instruction_ptr->name == ILLEGAL_INSTRUCTION){
      printf("illegal instruction (opcode %d)\n", opcode);
    }
    else {
      process_instruction(cpu, current_instruction_ptr);
    }
  }

  /* If there is, do nothing until the clock cycles go to zero. In this emulator, at least for now,
   instructions are exectued in a single cpu iteration: the remaining cycles for the instruction are
   simply so the timings are correct.*/
  else{
    --current_instruction_ptr->cycles_left;
  }

  // Increment the program counter.
  ++cpu->pc;

  // return the current instruction pointer for debugging purposes
  return current_instruction_ptr; 
}
