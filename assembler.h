#ifndef ASSEMBLER
#define ASSEMBLER

#include "cpu_state.h"
#include "shared.h"

void assemble(CpuState* cpu, const char* filename);
void assemble_legacy(CpuState* cpu, const char* filename);

#endif
