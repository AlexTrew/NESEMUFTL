# include "cpu.h"
# include "ppu.h"
# include <stdlib.h>

# include <stdio.h>
# include "SDL3/SDL.h"
# include <stdbool.h>

/*
  nes emulator for the lols
 */

static unsigned long master_clock_cycle_count = 0;

int main(){

  // fake RAM since its not important now
  uint16_t bus[0xFFFE];
  for(int i=0;i<sizeof(bus)/sizeof(bus[0]);i++){
    bus[i] = 0x16;
  }

  Cpu* cpu = init_cpu(bus);
  
  while(true){
    ++master_clock_cycle_count;
    cpu_cycle(cpu);
    }

  delete_cpu(cpu);
}
