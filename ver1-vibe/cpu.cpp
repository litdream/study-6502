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
    std::cout << "Fetched opcode: 0x" << std::hex << (int)opcode << " at PC: 0x" << std::hex << (int)PC << std::endl;
    execute(opcode);
}

int CPU::execute(uint8_t opcode) {
    currentCycles = 0; // Reset cycles for the current instruction

    // Increment PC for the opcode itself
    PC++;

    switch (opcode) {
        // LDA - Load Accumulator
        // Immediate: LDA #$NN (A9)
        case 0xA9: {
            uint8_t value = mem.read(PC++); // Read value and then increment PC for the operand
            A = value;
            setZNFlags(A);
            currentCycles += 2; // 2 cycles
            break;
        }
        // LDA - Zero Page: LDA $NN (A5)
        case 0xA5: {
            uint8_t zeroPageAddr = mem.read(PC++); // Read zero page address and then increment PC for the operand
            A = mem.read(zeroPageAddr);
            setZNFlags(A);
            currentCycles += 3; // 3 cycles
            break;
        }
        // LDA - Absolute: LDA $NNNN (AD)
        case 0xAD: {
            uint8_t lowByte = mem.read(PC++); // Read low byte and then increment PC for the first operand
            uint8_t highByte = mem.read(PC++); // Read high byte and then increment PC for the second operand
            uint16_t absoluteAddr = (highByte << 8) | lowByte;
            A = mem.read(absoluteAddr);
            setZNFlags(A);
            currentCycles += 4; // 4 cycles
            break;
        }
        default:
            std::cout << "Unknown opcode: 0x" << std::hex << (int)opcode << std::endl;
            currentCycles += 0; // Unknown opcode, assume 0 cycles or handle as an error
            break;
    }
    return currentCycles;
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
