#include "instructions.h"

CpuState ADC_(CpuState cpu, const AddrModeFptr addr_mode_func) {
    const CpuAddressingModeResult address_mode = addr_mode_func(&cpu);
    const uint16_t result = cpu.a + cpu.bus[address_mode.operand];
    if (result > 0xFF) {
        cpu.a = 0xFF;
        // Overflow - set carry bit
        cpu.p = cpu.p | (1 << 0);
    } else {
        cpu.a = result;
    }
    return cpu;
}