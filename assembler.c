#include "assembler.h"
#include "cpu_state.h"
#include "shared.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <glib-2.0/glib.h>

int MAX_LINE_LEN = 256;


static void hash_table_destroy_value(gpointer data) {
  free(data);
}

static uint8_t convert_instruction_string_to_opcode(GHashTable* lookup, const char* instruction) {
    gpointer res = g_hash_table_lookup(lookup, instruction);
    if (res == NULL) {
      printf("error: no supported opcde for instruction/address mode combination %s\n", instruction);
      exit(1);
    }      
    return *(uint8_t*)res;
}  

static void write_single_byte_instruction(CpuState* cpu, GHashTable* opcode_lookup, const  char* instruction) {
  uint8_t opcode = convert_instruction_string_to_opcode(opcode_lookup, instruction);
  cpu->bus[cpu->pc] = opcode;
  printf("wrote %x to %04x\n", cpu->bus[cpu->pc],  cpu->pc);
  cpu->pc+=opcode_x_cpu_instruction_lookup[opcode].size_in_bytes;
}  

static void write_2_byte_instruction(CpuState* cpu, GHashTable* opcode_lookup, const  char* instruction, uint8_t operand) {
  uint8_t opcode = convert_instruction_string_to_opcode(opcode_lookup, instruction);
  cpu->bus[cpu->pc] = opcode;
  cpu->bus[cpu->pc+1] = operand;
  printf("wrote %x %x to %04x\n", cpu->bus[cpu->pc], cpu->bus[cpu->pc+1], cpu->pc);
  cpu->pc+=opcode_x_cpu_instruction_lookup[opcode].size_in_bytes;
}  

static void write_3_byte_instruction(CpuState* cpu, GHashTable* opcode_lookup, const  char* instruction, uint16_t operand) {
  uint8_t opcode = convert_instruction_string_to_opcode(opcode_lookup, instruction);

  /* Here we convert the 16 bit operand into two bytes in the little endian
format.
  We do this by shifting it 8 bits forward to get the high order btye, and bitwise anding it with 0x00FF to get the low order one */

  cpu->bus[cpu->pc] = opcode;
  cpu->bus[cpu->pc+1] = operand & 0xFF;
  cpu->bus[cpu->pc+2] = operand >> 8;
  printf("wrote %x %x %x to %04x\n", cpu->bus[cpu->pc], cpu->bus[cpu->pc+1], cpu->bus[cpu->pc+2], cpu->pc);
  cpu->pc+=opcode_x_cpu_instruction_lookup[opcode].size_in_bytes;
}  


void assemble_legacy(CpuState* cpu, const char* filename) {

  // build a lookup of opcodes
  GHashTable* opcode_lookup = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, hash_table_destroy_value);
  for (uint16_t i = 0; i < NUMBER_OF_INSTRUCTIONS; ++i) {
    uint8_t* opcode = malloc(sizeof(int));
    memcpy(opcode, &i, sizeof(int));
    g_hash_table_insert(opcode_lookup, opcode_x_cpu_instruction_lookup[i].name_str, GINT_TO_POINTER(opcode));
  }

  // process the input file
  FILE* file = fopen(filename, "r");

  if (file == NULL) {
    printf("File %s not found\n", filename);
    exit(1);
  }

  /* setup and compile regex matchers for different instruction types 

     Yes I know this is not how to interpret a programming language but I cbf doing it properly
     and the syntax of 6502 assembly is simple enough that this can work.
     This module is basically for test purposes anyway.
  */
  regex_t comment_regex;
  regex_t abs_regex;
  regex_t abs_xy_regex;
  regex_t zp_regex;
  regex_t zp_xy_regex;
  regex_t ind_regex;
  regex_t ind_x_regex;
  regex_t ind_y_regex;
  regex_t imm_regex;
  regex_t implied_regex;

  regcomp(&comment_regex, ";*$", REG_EXTENDED);
  regcomp(&abs_regex, "[A-Z]\{3\} [\$][[:xdigit:]]\{4\}\s*;*.*$", REG_EXTENDED);
  regcomp(&abs_xy_regex, "^[A-Z]\{3\} [\$][[:xdigit:]]\{4\},[XY]\s*;*.*$", REG_EXTENDED);
  regcomp(&zp_regex, "[A-Z]\{3\} [\$][[:xdigit:]]\{2\}\s*;*.*$", REG_EXTENDED);
  regcomp(&zp_xy_regex, "[A-Z]\{3\} [\$][[:xdigit:]]\{2\},[XY]\s*;*.*$", REG_EXTENDED);
  regcomp(&ind_regex, "[A-Z]\{3\} ([\$][[:xdigit:]]\{4\})\s*;*.*$", REG_EXTENDED);
  regcomp(&ind_x_regex, "[A-Z]\{3\} ([\$][[:xdigit:]]\{2\},X)\s*;*.*$", REG_EXTENDED);
  regcomp(&ind_y_regex, "[A-Z]\{3\} ([\$][[:xdigit:]]\{2\}),Y\s*;*.*$", REG_EXTENDED);
  regcomp(&imm_regex, "[A-Z]\{3\} #[\$][[:xdigit:]]\{2\}\s*;*.*$", REG_EXTENDED);
  regcomp(&implied_regex, "[A-Z]\{3\}\s*;*.*$", REG_EXTENDED);

  char line_buf[MAX_LINE_LEN];
  int line_no = 1;
  while(fgets(line_buf, MAX_LINE_LEN, file)){
    char instruction[256];
    uint16_t operand;


    if (!regexec(&comment_regex, line_buf, 0, NULL, 0)) {
      // it's a comment; do nothing
    }

    // write ABS instruction
    if (!regexec(&abs_regex, line_buf, 0, NULL, 0)) {
      sscanf(line_buf, "%s $%hx", instruction, &operand);
      strcat(instruction, "-ABS");

      write_3_byte_instruction(cpu, opcode_lookup, instruction, operand);
    } 

    // write ABS X and Y instruction
    else if (!regexec(&abs_xy_regex, line_buf, 0, NULL, 0)) {
      sscanf(line_buf, "%s $%hx", instruction, &operand);
      if (strchr(line_buf, 'Y') !=NULL) {
	strcat(instruction, "-ABS_Y");
      }        
      else if (strchr(line_buf, 'X')!=NULL) {
	strcat(instruction, "-ABS_X");
      }
      write_3_byte_instruction(cpu, opcode_lookup, instruction, operand);
    }

    // write zp instruction
    else if (!regexec(&zp_regex, line_buf, 0, NULL, 0)) {
      sscanf(line_buf, "%s $%hx", instruction, &operand);
      strcat(instruction, "-ZP");

      write_2_byte_instruction(cpu, opcode_lookup, instruction, operand & 0xFF);
    }
    
    // write zp X or Yinstruction
    else if (!regexec(&zp_xy_regex, line_buf, 0, NULL, 0)) {
      sscanf(line_buf, "%s $%hx", instruction, &operand);
      if (strchr(line_buf, 'Y') !=NULL) {
	strcat(instruction, "-ZP_Y");
      }        
      else if (strchr(line_buf, 'X')!=NULL) {
	strcat(instruction, "-ZP_X");
      }
      write_2_byte_instruction(cpu, opcode_lookup, instruction, operand & 0xFF);
    }

    // write immediate instruction
    else if (!regexec(&imm_regex, line_buf, 0, NULL, 0)) {
      sscanf(line_buf, "%s #$%hx", instruction, &operand);
      strcat(instruction, "-IMM");

      write_2_byte_instruction(cpu, opcode_lookup, instruction, operand & 0xFF);
    }

    // write implied instruction
    else if (!regexec(&implied_regex, line_buf, 0, NULL, 0)) {
      sscanf(line_buf, "%s", instruction);
      strcat(instruction, "-IMPLIED");

      write_single_byte_instruction(cpu, opcode_lookup, instruction);
    }

    // write indirect instruction
    else if (!regexec(&ind_regex, line_buf, 0, NULL, 0)) {
      sscanf(line_buf, "%s $%hx", instruction, &operand);
      strcat(instruction, "-IND");

      write_2_byte_instruction(cpu, opcode_lookup, instruction, operand & 0xFF);
    }

    // write indirect X instruction
    else if (!regexec(&ind_x_regex, line_buf, 0, NULL, 0)) {
      sscanf(line_buf, "%s $%hx", instruction, &operand);
      strcat(instruction, "-IND_X");

      write_2_byte_instruction(cpu, opcode_lookup, instruction, operand & 0xFF);
    }

    // write indirect Y instruction
    else if (!regexec(&ind_y_regex, line_buf, 0, NULL, 0)) {
      sscanf(line_buf, "%s $%hx", instruction, &operand);
      strcat(instruction, "-IND_Y");

      write_2_byte_instruction(cpu, opcode_lookup, instruction, operand & 0xFF);
    }
    else {
      printf("Syntax error in line %d\n", line_no);
      exit(1);
    }      

    ++line_no;
  }

  printf("Code assembled successfully, %d bytes\n", cpu->pc - 0x0600);
  
  // reset the stack pointer
  cpu->pc = 0x0600;

  // cleanup  
  g_hash_table_destroy(opcode_lookup);

  fclose(file);
}


char*** lex_file(const char * filename) {
  // process the input file
  FILE* file = fopen(filename, "r");

  if (file == NULL) {
    printf("File %s not found\n", filename);
    exit(1);
  }

  printf("file %s opened\n", filename);

  // determine the number of lines in the file to allocate the
  // correct amount of memory.  
  int no_of_lines = 0;
  char line_buf[MAX_LINE_LEN];
  while (fgets(line_buf, MAX_LINE_LEN, file) != NULL) {
    ++no_of_lines;
  }

  rewind(file);  

  char*** tokenised_lines = malloc(no_of_lines*sizeof(char**));

  int i = 0;
  while (fgets(line_buf, MAX_LINE_LEN, file) != NULL) {

    char** lexed_line = malloc(MAX_LINE_LEN*sizeof(char*));

    char* token = strtok(line_buf, " ");
    int j = 0;
    while (token != NULL) {
      token[strcspn(token, "\n")] = 0;
      char* tok = malloc(sizeof(token));
      strcpy(tok, token);
      lexed_line[j++] = tok;
      token = strtok(NULL, token);
    }
    tokenised_lines[i++] = lexed_line;
  }    

  fclose(file);

  return tokenised_lines;
}

void _parse_tokens(char ***tokens) {
  // check for a label and use it
  regex_t label_regex;
  regcomp(&label_regex, "", REG_EXTENDED);

  // determine instruction type

  // determine the addressing mode and get the operand

  // write things!
}  


void assemble(CpuState *cpu, const char* filename) {
  // build a lookup of opcodes
  GHashTable* opcode_lookup = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, hash_table_destroy_value);
  for (uint16_t i = 0; i < NUMBER_OF_INSTRUCTIONS; ++i) {
    uint8_t* opcode = malloc(sizeof(int));
    memcpy(opcode, &i, sizeof(int));
    g_hash_table_insert(opcode_lookup, opcode_x_cpu_instruction_lookup[i].name_str, GINT_TO_POINTER(opcode));
  }

  char*** tokens = lex_file(filename);

  // cleanup  
  g_hash_table_destroy(opcode_lookup);

}  
