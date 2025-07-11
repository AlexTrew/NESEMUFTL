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

  Cpu* cpu = (Cpu*)malloc(sizeof(Cpu));
  
  while(true){
    ++master_clock_cycle_count;
    cpu_cycle(cpu, master_clock_cycle_count);
    printf("master clock: %ld, cpu clock %ld\n", master_clock_cycle_count, cpu->cpu_cycle_count);
    }

  free(cpu);
}
