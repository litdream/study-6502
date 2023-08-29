#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>

class CPU {
public:
    CPU();
    void reset();
    void executeInstruction();
    
private:
    // Registers
    uint8_t A;   // Accumulator
    uint8_t X;   // X index register
    uint8_t Y;   // Y index register
    uint16_t PC; // Program counter
    uint8_t SP;  // Stack pointer
    uint8_t SR;  // Status register
    
    // Memory
    uint8_t memory[65536]; // 64KB memory
    
    // Helper functions for addressing modes
    uint8_t immediate();
    uint8_t zeroPage();
    uint8_t zeroPageX();
    uint8_t zeroPageY();
    uint8_t absolute();
    uint8_t absoluteX();
    uint8_t absoluteY();
    uint8_t indirectX();
    uint8_t indirectY();
    
    // Instructions
    void ADC(uint8_t value);
    void AND(uint8_t value);
    // ... (implement other instructions)
};

#endif // CPU_HPP
