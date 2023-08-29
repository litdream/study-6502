#include "cpu.hpp"

CPU::CPU() {
    // Initialize CPU state here
}

void CPU::reset() {
    // Reset CPU state here
}

void CPU::executeInstruction() {
    // Fetch the opcode from memory at PC
    uint8_t opcode = memory[PC];
    
    // Decode and execute the instruction based on the opcode
    switch (opcode) {
        case 0x69: // ADC Immediate
            ADC(immediate());
            break;
        case 0x29: // AND Immediate
            AND(immediate());
            break;
        // ... (implement other instructions)
        default:
            // Handle unknown opcode
            break;
    }
}

uint8_t CPU::immediate() {
    // Fetch the immediate value from memory at PC+1
    uint8_t value = memory[PC + 1];
    PC += 2; // Increment PC by 2
    return value;
}

// Implement other addressing mode functions (zeroPage, absolute, etc.) similarly

void CPU::ADC(uint8_t value) {
    // Implement ADC instruction logic here
    // Update flags, A register, etc.
}

void CPU::AND(uint8_t value) {
    // Implement AND instruction logic here
    // Update flags, A register, etc.
}

// Implement other instruction functions (AND, ORA, etc.) similarly
