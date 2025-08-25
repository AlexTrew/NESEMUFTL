# include "cpu.h"
#include "cpu_state.h"
# include "ppu.h"

# include <stdbool.h>

/*
  nes emulator for the lols
 */

static unsigned long master_clock_cycle_count = 0;

int main(){
  // fake RAM since its not important now
  uint8_t bus[0xFFFF];
  for(uint16_t i=0;i<sizeof(bus)/sizeof(bus[0]);i++){
    bus[i] = 0x69;
  }

  CpuState* cpu = init_cpu(bus);
  
  while(true){
    ++master_clock_cycle_count;
    cpu_cycle(cpu);
    }

  delete_cpu(cpu);
}
