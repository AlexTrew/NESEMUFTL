
#include "assembler.h"
#include "shared.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <glib-2.0/glib.h>

int MAX_LINE_LEN = 256;

static const uint8_t* get_opcodes() {
  uint8_t *opcodes = (uint8_t*)malloc(sizeof(uint8_t) * NUMBER_OF_INSTRUCTIONS);
  for (uint16_t i = 0; i < NUMBER_OF_INSTRUCTIONS; ++i) {
    opcodes[i] = i & 0xFF;
  }    
  
  return opcodes;
}  

static GHashTable *build_opcode_hash_table(const uint8_t* opcodes) {
  GHashTable* opcode_lookup = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, NULL);

  for (uint16_t i = 0; i < NUMBER_OF_INSTRUCTIONS; ++i) {
    g_hash_table_insert(opcode_lookup, (gpointer)&opcode_x_cpu_instruction_lookup[i].name_str, (gpointer)&opcodes[i]);
  }    

  return opcode_lookup;
}


void assemble(CpuState* cpu, const char* filename) {

  const uint8_t* opcodes = get_opcodes();
  GHashTable* opcode_lookup = build_opcode_hash_table(opcodes);

  FILE* file = fopen(filename, "r");

  if (file == NULL) {
    printf("File %s not found\n", filename);
    return;
  }    
  
  char line_buf[MAX_LINE_LEN];

  int line_no = 1;
  while (fgets(line_buf, MAX_LINE_LEN, file)) {
    //parse the first 3 chars and assert that they are a valid instruction name
    char instruction[3];
    strncpy(instruction, line_buf, 3);
    printf("line %d\n", line_no);

    // get the addressing mode from the structure of the second part of the line

    ++line_no;
  }

  g_hash_table_destroy(opcode_lookup);
  free(opcodes);

  fclose(file);
}
