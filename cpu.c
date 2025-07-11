#include "cpu.h"

void syncronise_with_master_clock(Cpu* cpu, unsigned long master_clock_cycle_count){
  /* The cpu clock speed is approx 1/12 of that of the master clock.
     See https://www.nesdev.org/wiki/Cycle_reference_chart for details. */
  if(master_clock_cycle_count % 12 == 0){
    ++cpu->cpu_cycle_count;
  }
}

void cpu_cycle(Cpu* cpu, unsigned long master_clock_cycle_count){
  syncronise_with_master_clock(cpu, master_clock_cycle_count);
}
