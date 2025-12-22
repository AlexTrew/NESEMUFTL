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

int main(){

  /* if (argc != 2) { */
  /*   printf("Wrong number of args.\nusage: nesemuftl <filename>\n"); */
  /*   return -1; */
  /* }     */

  uint8_t* bus = malloc(sizeof(uint8_t) * 0xFFFF);
  for(uint16_t i=0;i<0xFFFF/sizeof(uint16_t);i++){
    bus[i] = 0x00;
  }

  CpuState* cpu = init_cpu(bus);


  char input_buf[256];
  while (true) {
    printf(">: ");
    char* cmd = fgets(input_buf, 256, stdin);
    char* tok = strtok(cmd, " ");

    char v[256][256];
    int c=0;
    while (tok != NULL) {
      strcpy(v[c], tok);
      tok = strtok(NULL, " ");
      ++c;
    }

    if (c == 2 && strcmp(v[0], "load") == 0) {
      assemble(cpu, v[1]);
    }      
    if (c == 3 && strcmp(v[0], "show") == 0) {
      uint16_t start = (uint16_t)strtol(v[1], NULL, 16);
      uint16_t end = (uint16_t)strtol(v[2], NULL, 16);
      show_memory(cpu, start, end);
    }      

  }    
  
  while(true){
    cpu_cycle(cpu);
  }

  delete_cpu(cpu);
  free(bus);
}
