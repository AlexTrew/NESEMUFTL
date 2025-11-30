
#include "assembler.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <glib-2.0/glib.h>

int MAX_LINE_LEN = 256;


GHashTable opcode_lookup = g_hash_table_new();

void assemble(CpuState* cpu, const char* filename) {
  FILE* file = fopen(filename, "r");

  if (file == NULL) {
    printf("File %s not found\n", filename);
    retur;
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


    // parse the operand

    // determine the addressing mode

    // determine the operand

    //write this all to memory
  }

  fclose(file);

}
