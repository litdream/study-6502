#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>
#include "memory.hpp"

class CPU {
public:
    // 6502 Registers
    uint8_t A;  // Accumulator
    uint8_t X;  // Index Register X
    uint8_t Y;  // Index Register Y
    uint16_t PC; // Program Counter
    uint8_t S;  // Stack Pointer
    uint8_t P;  // Status Register

    // Status Flags (P register bits)
    enum Flag {
        C = (1 << 0), // Carry Flag
        Z = (1 << 1), // Zero Flag
        I = (1 << 2), // Interrupt Disable
        D = (1 << 3), // Decimal Mode Flag
        B = (1 << 4), // Break Command Flag
        U = (1 << 5), // Unused Flag (always 1)
        V = (1 << 6), // Overflow Flag
        N = (1 << 7)  // Negative Flag
    };

    Memory& mem;

    CPU(Memory& memory);

    // Helper functions for status flags
    void setFlag(Flag flag) { P |= flag; }
    void clearFlag(Flag flag) {
        if (flag != U) { // Unused flag (U) should always be 1, cannot be cleared
            P &= ~flag;
        }
    }
    bool getFlag(Flag flag) const { return (P & flag) != 0; }
    void setZNFlags(uint8_t value);

    // Stack operations
    void pushStack(uint8_t value);
    uint8_t popStack();

    // CPU execution
    void step();
    void reset();
        int execute(uint8_t opcode);
    
    private:
        void branchIf(bool condition);
        uint32_t currentCycles; // Clock cycles for the current instruction
    };

#endif // CPU_HPP