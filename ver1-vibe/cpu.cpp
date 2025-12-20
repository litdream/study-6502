#include "cpu.hpp"
#include <iostream>

CPU::CPU(Memory& memory) : A(0), X(0), Y(0), PC(0), S(0xFF), P(CPU::I | CPU::U), mem(memory), currentCycles(0) {
    // Initial state of 6502 registers after reset
    // P is typically initialized with Interrupt Disable (I) and Unused (U) flags set.
}

void CPU::setZNFlags(uint8_t value) {
    if (value == 0) {
        setFlag(Z);
    } else {
        clearFlag(Z);
    }
    if ((value & 0x80) != 0) { // Check bit 7 for negative
        setFlag(N);
    } else {
        clearFlag(N);
    }
}

void CPU::step() {
    uint8_t opcode = mem.read(PC);
    PC++;
    std::cout << "Fetched opcode: 0x" << std::hex << (int)opcode << " at PC: 0x" << std::hex << (int)(PC -1) << std::endl;
    execute(opcode);
}

void CPU::execute(uint8_t opcode) {
    switch (opcode) {
        // LDA - Load Accumulator
        // Immediate: LDA #$NN (A9)
        case 0xA9: {
            uint8_t value = mem.read(PC);
            PC++;
            A = value;
            setZNFlags(A);
            currentCycles += 2; // 2 cycles
            break;
        }
        // LDA - Zero Page: LDA $NN (A5)
        case 0xA5: {
            uint8_t zeroPageAddr = mem.read(PC);
            PC++;
            A = mem.read(zeroPageAddr);
            setZNFlags(A);
            currentCycles += 3; // 3 cycles
            break;
        }
        // LDA - Absolute: LDA $NNNN (AD)
        case 0xAD: {
            uint8_t lowByte = mem.read(PC);
            PC++;
            uint8_t highByte = mem.read(PC);
            PC++;
            uint16_t absoluteAddr = (highByte << 8) | lowByte;
            A = mem.read(absoluteAddr);
            setZNFlags(A);
            currentCycles += 4; // 4 cycles
            break;
        }
        default:
            std::cout << "Unknown opcode: 0x" << std::hex << (int)opcode << std::endl;
            break;
    }
}

void CPU::reset() {
    A = 0;
    X = 0;
    Y = 0;
    PC = 0;
    S = 0xFF; // Stack starts at 0xFF
    P = CPU::I | CPU::U; // P is typically initialized with Interrupt Disable (I) and Unused (U) flags set.
    currentCycles = 0;
}
