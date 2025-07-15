#include "cpu.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
  Instructions named "???" should be returned for illegal opcodes.
  See page 10 of archive.6502.org/datasheets/rockwell_r650x_r651x.pdf for a
  complete opcode -> instruction reference.
 */  
static const CpuInstruction cpu_instruction_lookup[] = {
  [0x00] = {.name="BRK", .cycles_left=7, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x01] = {.name="ORA", .cycles_left=5, .mem_needed=2, .addressing_mode=IND_X},
  [0x02] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x03] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x04] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x05] = {.name="ORA", .cycles_left=3, .mem_needed=2, .addressing_mode=ZP},
  [0x06] = {.name="ASL", .cycles_left=5, .mem_needed=2, .addressing_mode=ZP},
  [0x07] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x08] = {.name="PHP", .cycles_left=3, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x09] = {.name="ORA", .cycles_left=2, .mem_needed=2, .addressing_mode=IMM},
  [0x0A] = {.name="ASL", .cycles_left=2, .mem_needed=1, .addressing_mode=ACCUM},
  [0x0B] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x0C] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x0D] = {.name="ORA", .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0x0E] = {.name="ASL", .cycles_left=6, .mem_needed=3, .addressing_mode=ABS},
  [0x0F] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x10] = {.name="BPL", .cycles_left=2, .mem_needed=2, .addressing_mode=RELATIVE},
  [0x11] = {.name="ORA", .cycles_left=5, .mem_needed=2, .addressing_mode=IND_X},
  [0x12] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x13] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x14] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x15] = {.name="ORA", .cycles_left=5, .mem_needed=2, .addressing_mode=ZP},
  [0x16] = {.name="ASL", .cycles_left=6, .mem_needed=2, .addressing_mode=ZP},
  [0x17] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x18] = {.name="CLC", .cycles_left=2, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x19] = {.name="ORA", .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0x1A] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x1B] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x1C] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x1D] = {.name="ORA", .cycles_left=4, .mem_needed=3, .addressing_mode=ABS_X},
  [0x1E] = {.name="ASL", .cycles_left=6, .mem_needed=4, .addressing_mode=ABS_X},
  [0x1F] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x20] = {.name="JSR", .cycles_left=6, .mem_needed=3, .addressing_mode=ABSOLUTE},
  [0x21] = {.name="AND", .cycles_left=6, .mem_needed=2, .addressing_mode=IND_X},
  [0x22] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x23] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x24] = {.name="BIT", .cycles_left=3, .mem_needed=2, .addressing_mode=ZP},
  [0x25] = {.name="AND", .cycles_left=3, .mem_needed=2, .addressing_mode=ZP},
  [0x26] = {.name="ROL", .cycles_left=5, .mem_needed=2, .addressing_mode=ZP},
  [0x27] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x28] = {.name="PLP", .cycles_left=4, .mem_needed=1, .addressing_mode=IMPLIED},
  [0x29] = {.name="AND", .cycles_left=2, .mem_needed=2, .addressing_mode=IMM},
  [0x2A] = {.name="ROL", .cycles_left=2, .mem_needed=1, .addressing_mode=ACCUM},
  [0x2B] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x2C] = {.name="BIT", .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0x2D] = {.name="AND", .cycles_left=4, .mem_needed=3, .addressing_mode=ABS},
  [0x2E] = {.name="ROL", .cycles_left=6, .mem_needed=3, .addressing_mode=ABS},
  [0x2F] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x30] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x31] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x32] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x33] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x34] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x35] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x36] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x37] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x38] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x39] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x3A] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x3B] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x3C] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x3D] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x3E] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x3F] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x40] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x41] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x42] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x43] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x44] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x45] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x46] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x47] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x48] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x49] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x4A] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x4B] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x4C] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x4D] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x4E] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x4F] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x50] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x51] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x52] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x53] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x54] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x55] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x56] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x57] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x58] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x59] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x5A] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x5B] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x5C] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x5D] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x5E] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x5F] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x60] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x61] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x62] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x63] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x64] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x65] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x66] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x67] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x68] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x69] = {.name="ADC", .cycles_left=2, .mem_needed=2, .addressing_mode=ABSOLUTE},
  [0x6A] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x6B] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x6C] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x6D] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x6E] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x6F] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x70] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x71] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x72] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x73] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x74] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x75] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x76] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x77] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x78] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x79] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x7A] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x7B] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x7C] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x7D] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x7E] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x7F] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x80] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x81] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x82] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x83] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x84] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x85] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x86] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x87] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x88] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x89] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x8A] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x8B] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x8C] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x8D] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x8E] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x8F] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x90] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x91] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x92] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x93] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x94] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x95] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x96] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x97] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x98] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x99] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x9A] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x9B] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x9C] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x9D] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x9E] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0x9F] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xA0] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xA1] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xA2] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xA3] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xA4] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xA5] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xA6] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xA7] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xA8] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xA9] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xAA] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xAB] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xAC] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xAD] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xAE] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xAF] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xB0] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xB1] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xB2] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xB3] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xB4] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xB5] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xB6] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xB7] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xB8] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xB9] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xBA] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xBB] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xBC] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xBD] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xBE] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xBF] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xC0] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xC1] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xC2] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xC3] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xC4] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xC5] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xC6] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xC7] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xC8] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xC9] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xCA] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xCB] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xCC] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xCD] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xCE] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xCF] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xD0] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xD1] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xD2] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xD3] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xD4] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xD5] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xD6] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xD7] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xD8] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xD9] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xDA] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xDB] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xDC] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xDD] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xDE] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xDF] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xE0] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xE1] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xE2] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xE3] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xE4] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xE5] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xE6] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xE7] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xE8] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xE9] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xEA] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xEB] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xEC] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xED] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xEE] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xEF] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xF0] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xF1] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xF2] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xF3] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xF4] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xF5] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xF6] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xF7] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xF8] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xF9] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xFA] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xFB] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xFC] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xFD] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xFE] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
  [0xFF] = {.name="???", .cycles_left=0, .mem_needed=0, .addressing_mode=NONE},
};

static void process_instruction(CpuInstruction* instruction);
static void set_status_flag(Cpu* cpu, CpuStatusFlag f, bool v);
static uint8_t cpu_read_from_bus(Cpu* cpu, uint16_t addr);
static void cpu_write(Cpu* cpu, uint16_t addr, uint8_t data);

static uint8_t cpu_read_from_bus(Cpu* cpu, uint16_t addr){
  return cpu->bus[addr];
}

static void cpu_write(Cpu* cpu, uint16_t addr, uint8_t data){
  if(addr >= 0x0000 && addr <= 0xFFFF){
    cpu->bus[addr] = data;
  }
}

static void set_status_flag(Cpu* cpu, CpuStatusFlag f, bool v){
  if(v){
    cpu->status |= f;
  }
  else{
    cpu->status &= ~f;
  }
}

static void process_instruction(CpuInstruction* instruction){
    printf("instruction: %s, cycles_left: %d\n", instruction->name, instruction->cycles_left);
    --instruction->cycles_left;
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

void cpu_cycle(Cpu* cpu){
  static CpuInstruction* current_instruction_ptr = NULL;

  /* check if the current instruction is complete.
     if so, clean it up */
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
    if(strcmp(current_instruction_ptr->name, "???") == 0){
      printf("illegal opcode (%d) exiting...\n", opcode);
	exit(-1);
    }

    process_instruction(current_instruction_ptr);
  }
  // If there is, just keep executing it.
  else{
    process_instruction(current_instruction_ptr);
  }

  // Increment the program counter.
  ++cpu->pc;
}
