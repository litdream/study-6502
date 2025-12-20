#include "cpu.hpp"
#include <iostream>

CPU::CPU(Memory& memory) : A(0), X(0), Y(0), PC(0), S(0xFF), P(CPU::I | CPU::U), mem(memory) {
    // Initial state of 6502 registers after reset
    // P is typically initialized with Interrupt Disable (I) and Unused (U) flags set.
}

void CPU::step() {
    uint8_t opcode = mem.read(PC);
    PC++;
    std::cout << "Fetched opcode: 0x" << std::hex << (int)opcode << " at PC: 0x" << std::hex << (int)(PC -1) << std::endl;
    // Placeholder for decode and execute
}

void CPU::reset() {
    A = 0;
    X = 0;
    Y = 0;
    PC = 0;
    S = 0xFF; // Stack starts at 0xFF
    P = CPU::I | CPU::U; // P is typically initialized with Interrupt Disable (I) and Unused (U) flags set.
}
