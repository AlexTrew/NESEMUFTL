#ifndef CPU_STATE
#define CPU_STATE

#include <stdint.h>

typedef struct {
  unsigned long cpu_cycle_count;
  uint16_t pc; 
  uint8_t a;
  uint8_t x;
  uint8_t y;
  uint8_t stkptr;
  uint8_t status;
  uint8_t* bus; 
} CpuState;

#endif
