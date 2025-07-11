
/*
  This is the cpu. we are emulating the NTSC(2CO2) cpu used in the ntsc version.
 */

typedef struct Cpu{
  unsigned long cpu_cycle_count;
} Cpu;


void cpu_cycle(Cpu* cpu, unsigned long master_clock_cycle_count);
void syncronise_with_master_clock(Cpu* cpu, unsigned long master_clock_cycle_count);

