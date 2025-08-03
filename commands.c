#include "commands.h"

CpuState ADC_(CpuState cpu, const AddrModeFptr addr_mode_func) {
    const CpuAddressingModeResult res = addr_mode_func(&cpu);
    // todo: ignoring carry for now
    cpu.a += cpu.bus[res.operand];
    return cpu;
}