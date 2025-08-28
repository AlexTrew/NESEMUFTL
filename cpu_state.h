#ifndef CPU_STATE
#define CPU_STATE

#include <stdint.h>

typedef struct {
  unsigned long cpu_cycle_count;
  uint16_t pc; 
  uint8_t a;
  uint8_t x;
  uint8_t y;
  // Flags
  uint8_t p; // status register
  uint8_t stkptr;
  uint8_t* bus; 
} CpuState;

typedef enum {
  C = (1 << 0),
  Z = (1 << 1),
  I = (1 << 2),
  D = (1 << 3),
  B = (1 << 4),
  U = (1 << 5),
  V = (1 << 6),
  N = (1 << 7),
} CpuStatusFlag;

#endif
