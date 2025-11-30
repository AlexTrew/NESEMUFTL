#include "cpu.h"
#include "cpu_state.h"
#include "assembler.h"
#include <stdbool.h>
#include <stdio.h>

/*
  nes emulator for the lols
 */


int main(int argc, char** argv){


  if (argc != 2) {
    printf("Wrong number of args.\nusage: nesemuftl <filename>\n");
    return -1;
  }    

  // fake RAM since its not important now
  uint8_t bus[0xFFFF];
  for(uint16_t i=0;i<sizeof(bus)/sizeof(bus[0]);i++){
    bus[i] = 0x69;
  }

  CpuState* cpu = init_cpu(bus);

  assemble(cpu, argv[1]);
  
  /*
  while(true){
    ++master_clock_cycle_count;
    cpu_cycle(cpu);
    }
  */

  delete_cpu(cpu);
}
