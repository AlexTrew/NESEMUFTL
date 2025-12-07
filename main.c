#include "cpu.h"
#include "cpu_state.h"
#include "assembler.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
  nes emulator for the lols
 */


int main(int argc, char** argv){

  if (argc != 2) {
    printf("Wrong number of args.\nusage: nesemuftl <filename>\n");
    return -1;
  }    

  uint8_t* bus = malloc(sizeof(uint8_t) * 0xFFFF);
  for(uint16_t i=0;i<0xFFFF/sizeof(uint16_t);i++){
    bus[i] = 0x00;
  }

  CpuState* cpu = init_cpu(bus);

  assemble(cpu, argv[1]);
  
  for(int i=0;i<100;++i){
    cpu_cycle(cpu);
  }

  delete_cpu(cpu);
  free(bus);
}
