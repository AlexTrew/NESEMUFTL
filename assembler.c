
#include "assembler.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <glib-2.0/glib.h>

int MAX_LINE_LEN = 256;


static GHashTable *build_opcode_hash_table() {
  // TODO add destroy functions for the hash table
  GHashTable* opcode_lookup = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, NULL);
  return opcode_lookup;
}


void assemble(CpuState* cpu, const char* filename) {

  GHashTable* opcode_lookup = build_opcode_hash_table();

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

    // doing this the bad way because i cbf implementing my own hashmap
    if(strcmp(instruction, "ADC") == 0){ 
    }      
    else if(strcmp(instruction, "LDA") == 0){ 
      printf("write LDA using the instruction lookup\n");
    } else {
      printf("syntax error on line %d: %s in not a valid instruction\n", line_no, instruction);
    }

    ++line_no;

  }

  g_hash_table_destroy(opcode_lookup);
  fclose(file);
}
