#include "cpu.h"
#include "cpu_state.h"
#include "assembler.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  nes emulator for the lols
 */

void show_memory(CpuState* cpu, uint16_t start, uint16_t end) {
  int line_len = 20;
  int count = 0;
  for (int i = start; i <= end; ++i) {
    printf("%x: %x ",i, cpu->bus[i]);
    if (count != 0 && count % line_len == 0) {
      printf("\n");
    }      
    ++count;
  }    
  printf("\n");
}

void run_program(CpuState* cpu) {
  while(true){
    cpu_cycle(cpu);
  }
}  

void run_program_max_cycles(CpuState* cpu, int max_cycles) {
  for(int i=0;i<max_cycles;++i){
    cpu_cycle(cpu);
  }
}  

int main(){

  uint8_t* bus = malloc(sizeof(uint8_t) * 0xFFFF);
  for(uint16_t i=0;i<0xFFFF/sizeof(uint16_t);i++){
    bus[i] = 0x00;
  }


  CpuState* cpu = init_cpu(bus);
  assemble(cpu, "/home/alex/Documents/test_6502");

  printf("NESEMUFTL: a NES emulator (currently just a 6502)\n\n");

  printf("usage:\n");
  printf("\"load <file>\" to load a program.\n");
  printf("\"run <n>\" to run a loaded program, with an optional max number of cpu steps\n");
  printf("\"mem <mem location> <max mem location>\" to print the contents of memory between 2 memory locations\n");
  printf("\"reg\" to print the contents of registers\n");

  char input_buf[256];
  while (true) {
    printf(">: ");

    char v[256][256];
    int c=0;

    char* cmd = fgets(input_buf, 256, stdin);
    char* tok = strtok(cmd, " ");


    while (tok != NULL) {

      // remove trailing newline char, if any
      tok[strcspn(tok, "\n")] = 0;
      strcpy(v[c++], tok);
      tok = strtok(NULL, " ");
    }

    if (c == 2 && strcmp(v[0], "load") == 0) {
      assemble(cpu, v[1]);
    }      
    else if (c == 3 && strcmp(v[0], "mem") == 0) {
      uint16_t start = (uint16_t)strtol(v[1], NULL, 16);
      uint16_t end = (uint16_t)strtol(v[2], NULL, 16);
      show_memory(cpu, start, end);
    }
    else if (c == 1 && strcmp(v[0], "reg") == 0) {
      printf("a: %x x: %x y: %x p: %x pc: %x stckptr: %x\n", cpu->a, cpu->x, cpu->y, cpu->p, cpu->pc, cpu->stkptr);
    }
    else if (c == 1 && strcmp(v[0], "run") == 0) {
      run_program(cpu);
    }      
    else if (c == 2 && strcmp(v[0], "run") == 0) {
      run_program_max_cycles(cpu, (int)strtol(v[1], NULL, 10));
    }      

  }    

  delete_cpu(cpu);
  free(bus);
}
