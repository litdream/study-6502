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

void CPU::pushStack(uint8_t value) {
    mem.write(0x0100 + S, value);
    S--;
}

uint8_t CPU::popStack() {
    S++;
    return mem.read(0x0100 + S);
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
        // STA - Store Accumulator
        // Zero Page: STA $NN (85)
        case 0x85: {
            uint8_t zeroPageAddr = mem.read(PC++);
            mem.write(zeroPageAddr, A);
            currentCycles += 3;
            break;
        }
        // Absolute: STA $NNNN (8D)
        case 0x8D: {
            uint8_t lowByte = mem.read(PC++);
            uint8_t highByte = mem.read(PC++);
            uint16_t absoluteAddr = (highByte << 8) | lowByte;
            mem.write(absoluteAddr, A);
            currentCycles += 4;
            break;
        }
        // Zero Page, X: STA $NN,X (95)
        case 0x95: {
            uint8_t zeroPageAddr = mem.read(PC++);
            uint8_t address = (zeroPageAddr + X) & 0xFF; // Wrap around for zero page
            mem.write(address, A);
            currentCycles += 4;
            break;
        }
        // Absolute, X: STA $NNNN,X (9D)
        case 0x9D: {
            uint8_t lowByte = mem.read(PC++);
            uint8_t highByte = mem.read(PC++);
            uint16_t baseAddr = (highByte << 8) | lowByte;
            uint16_t absoluteAddr = baseAddr + X;
            mem.write(absoluteAddr, A);
            currentCycles += 5; // Cycles for Absolute,X
            break;
        }
        // Absolute, Y: STA $NNNN,Y (99)
        case 0x99: {
            uint8_t lowByte = mem.read(PC++);
            uint8_t highByte = mem.read(PC++);
            uint16_t baseAddr = (highByte << 8) | lowByte;
            uint16_t absoluteAddr = baseAddr + Y;
            mem.write(absoluteAddr, A);
            currentCycles += 5; // Cycles for Absolute,Y
            break;
        }
        // Indirect, X: STA ($NN,X) (81)
        case 0x81: {
            uint8_t zeroPageAddr = mem.read(PC++);
            uint8_t indirectAddrBase = (zeroPageAddr + X) & 0xFF; // Wrap around for zero page
            uint8_t lowByte = mem.read(indirectAddrBase);
            uint8_t highByte = mem.read((indirectAddrBase + 1) & 0xFF); // Wrap around for zero page
            uint16_t effectiveAddr = (highByte << 8) | lowByte;
            mem.write(effectiveAddr, A);
            currentCycles += 6;
            break;
        }
        // Indirect, Y: STA ($NN),Y (91)
        case 0x91: {
            uint8_t zeroPageAddr = mem.read(PC++);
            uint8_t lowByte = mem.read(zeroPageAddr);
            uint8_t highByte = mem.read((zeroPageAddr + 1) & 0xFF); // Wrap around for zero page
            uint16_t baseAddr = (highByte << 8) | lowByte;
            uint16_t effectiveAddr = baseAddr + Y;
            mem.write(effectiveAddr, A);
            currentCycles += 6;
            break;
        }
        // LDX - Load X Register
        // Immediate: LDX #$NN (A2)
        case 0xA2: {
            uint8_t value = mem.read(PC++);
            X = value;
            setZNFlags(X);
            currentCycles += 2;
            break;
        }
        // Zero Page: LDX $NN (A6)
        case 0xA6: {
            uint8_t zeroPageAddr = mem.read(PC++);
            X = mem.read(zeroPageAddr);
            setZNFlags(X);
            currentCycles += 3;
            break;
        }
        // Absolute: LDX $NNNN (AE)
        case 0xAE: {
            uint8_t lowByte = mem.read(PC++);
            uint8_t highByte = mem.read(PC++);
            uint16_t absoluteAddr = (highByte << 8) | lowByte;
            X = mem.read(absoluteAddr);
            setZNFlags(X);
            currentCycles += 4;
            break;
        }
        // Zero Page, Y: LDX $NN,Y (B6)
        case 0xB6: {
            uint8_t zeroPageAddr = mem.read(PC++);
            uint8_t address = (zeroPageAddr + Y) & 0xFF; // Wrap around for zero page
            X = mem.read(address);
            setZNFlags(X);
            currentCycles += 4;
            break;
        }
        // Absolute, Y: LDX $NNNN,Y (BE)
        case 0xBE: {
            uint8_t lowByte = mem.read(PC++);
            uint8_t highByte = mem.read(PC++);
            uint16_t baseAddr = (highByte << 8) | lowByte;
            uint16_t absoluteAddr = baseAddr + Y;
            X = mem.read(absoluteAddr);
            setZNFlags(X);
            currentCycles += 4; // Add 1 if page crossed, not handled yet
            break;
        }
        // LDY - Load Y Register
        // Immediate: LDY #$NN (A0)
        case 0xA0: {
            uint8_t value = mem.read(PC++);
            Y = value;
            setZNFlags(Y);
            currentCycles += 2;
            break;
        }
        // Zero Page: LDY $NN (A4)
        case 0xA4: {
            uint8_t zeroPageAddr = mem.read(PC++);
            Y = mem.read(zeroPageAddr);
            setZNFlags(Y);
            currentCycles += 3;
            break;
        }
        // Absolute: LDY $NNNN (AC)
        case 0xAC: {
            uint8_t lowByte = mem.read(PC++);
            uint8_t highByte = mem.read(PC++);
            uint16_t absoluteAddr = (highByte << 8) | lowByte;
            Y = mem.read(absoluteAddr);
            setZNFlags(Y);
            currentCycles += 4;
            break;
        }
        // Zero Page, X: LDY $NN,X (B4)
        case 0xB4: {
            uint8_t zeroPageAddr = mem.read(PC++);
            uint8_t address = (zeroPageAddr + X) & 0xFF; // Wrap around for zero page
            Y = mem.read(address);
            setZNFlags(Y);
            currentCycles += 4;
            break;
        }
        // Absolute, X: LDY $NNNN,X (BC)
        case 0xBC: {
            uint8_t lowByte = mem.read(PC++);
            uint8_t highByte = mem.read(PC++);
            uint16_t baseAddr = (highByte << 8) | lowByte;
            uint16_t absoluteAddr = baseAddr + X;
            Y = mem.read(absoluteAddr);
            setZNFlags(Y);
            currentCycles += 4; // Add 1 if page crossed, not handled yet
            break;
        }
        // STX - Store X Register
        // Zero Page: STX $NN (86)
        case 0x86: {
            uint8_t zeroPageAddr = mem.read(PC++);
            mem.write(zeroPageAddr, X);
            currentCycles += 3;
            break;
        }
        // Absolute: STX $NNNN (8E)
        case 0x8E: {
            uint8_t lowByte = mem.read(PC++);
            uint8_t highByte = mem.read(PC++);
            uint16_t absoluteAddr = (highByte << 8) | lowByte;
            mem.write(absoluteAddr, X);
            currentCycles += 4;
            break;
        }
        // Zero Page, Y: STX $NN,Y (96)
        case 0x96: {
            uint8_t zeroPageAddr = mem.read(PC++);
            uint8_t address = (zeroPageAddr + Y) & 0xFF; // Wrap around for zero page
            mem.write(address, X);
            currentCycles += 4;
            break;
        }
        // STY - Store Y Register
        // Zero Page: STY $NN (84)
        case 0x84: {
            uint8_t zeroPageAddr = mem.read(PC++);
            mem.write(zeroPageAddr, Y);
            currentCycles += 3;
            break;
        }
        // Absolute: STY $NNNN (8C)
        case 0x8C: {
            uint8_t lowByte = mem.read(PC++);
            uint8_t highByte = mem.read(PC++);
            uint16_t absoluteAddr = (highByte << 8) | lowByte;
            mem.write(absoluteAddr, Y);
            currentCycles += 4;
            break;
        }
        // Zero Page, X: STY $NN,X (94)
        case 0x94: {
            uint8_t zeroPageAddr = mem.read(PC++);
            uint8_t address = (zeroPageAddr + X) & 0xFF; // Wrap around for zero page
            mem.write(address, Y);
            currentCycles += 4;
            break;
        }
        // Register Transfers
        // TAX - Transfer Accumulator to X (AA)
        case 0xAA: {
            X = A;
            setZNFlags(X);
            currentCycles += 2;
            break;
        }
        // TAY - Transfer Accumulator to Y (A8)
        case 0xA8: {
            Y = A;
            setZNFlags(Y);
            currentCycles += 2;
            break;
        }
        // TSX - Transfer Stack Pointer to X (BA)
        case 0xBA: {
            X = S;
            setZNFlags(X);
            currentCycles += 2;
            break;
        }
        // TXA - Transfer X to Accumulator (8A)
        case 0x8A: {
            A = X;
            setZNFlags(A);
            currentCycles += 2;
            break;
        }
        // TXS - Transfer X to Stack Pointer (9A)
        case 0x9A: {
            S = X;
            currentCycles += 2;
            break;
        }
        // TYA - Transfer Y to Accumulator (98)
        case 0x98: {
            A = Y;
            setZNFlags(A);
            currentCycles += 2;
            break;
        }
        // PHA - Push Accumulator (48)
        case 0x48: {
            pushStack(A);
            currentCycles += 3;
            break;
        }
        // PHP - Push Processor Status (08)
        case 0x08: {
            // Set B and U flags to 1 before pushing, as per 6502 behavior
            uint8_t status = P | CPU::B | CPU::U;
            pushStack(status);
            currentCycles += 3;
            break;
        }
        // PLA - Pull Accumulator (68)
        case 0x68: {
            A = popStack();
            setZNFlags(A);
            currentCycles += 4;
            break;
        }
        // PLP - Pull Processor Status (28)
        case 0x28: {
            uint8_t pulled_status = popStack();
            // The B and U flags are not affected by a PLP instruction.
            P = (pulled_status & ~CPU::B & ~CPU::U) | (P & (CPU::B | CPU::U));
            currentCycles += 4;
            break;
        }
        // AND - Logical AND
        // Immediate: AND #$NN (29)
        case 0x29: {
            uint8_t value = mem.read(PC++);
            A &= value;
            setZNFlags(A);
            currentCycles += 2;
            break;
        }
        // Zero Page: AND $NN (25)
        case 0x25: {
            uint8_t zeroPageAddr = mem.read(PC++);
            A &= mem.read(zeroPageAddr);
            setZNFlags(A);
            currentCycles += 3;
            break;
        }
        // Absolute: AND $NNNN (2D)
        case 0x2D: {
            uint8_t lowByte = mem.read(PC++);
            uint8_t highByte = mem.read(PC++);
            uint16_t absoluteAddr = (highByte << 8) | lowByte;
            A &= mem.read(absoluteAddr);
            setZNFlags(A);
            currentCycles += 4;
            break;
        }
        // EOR - Exclusive OR
        // Immediate: EOR #$NN (49)
        case 0x49: {
            uint8_t value = mem.read(PC++);
            A ^= value;
            setZNFlags(A);
            currentCycles += 2;
            break;
        }
        // Zero Page: EOR $NN (45)
        case 0x45: {
            uint8_t zeroPageAddr = mem.read(PC++);
            A ^= mem.read(zeroPageAddr);
            setZNFlags(A);
            currentCycles += 3;
            break;
        }
        // Absolute: EOR $NNNN (4D)
        case 0x4D: {
            uint8_t lowByte = mem.read(PC++);
            uint8_t highByte = mem.read(PC++);
            uint16_t absoluteAddr = (highByte << 8) | lowByte;
            A ^= mem.read(absoluteAddr);
            setZNFlags(A);
            currentCycles += 4;
            break;
        }
        // ORA - Logical Inclusive OR
        // Immediate: ORA #$NN (09)
        case 0x09: {
            uint8_t value = mem.read(PC++);
            A |= value;
            setZNFlags(A);
            currentCycles += 2;
            break;
        }
        // Zero Page: ORA $NN (05)
        case 0x05: {
            uint8_t zeroPageAddr = mem.read(PC++);
            A |= mem.read(zeroPageAddr);
            setZNFlags(A);
            currentCycles += 3;
            break;
        }
        // Absolute: ORA $NNNN (0D)
        case 0x0D: {
            uint8_t lowByte = mem.read(PC++);
            uint8_t highByte = mem.read(PC++);
            uint16_t absoluteAddr = (highByte << 8) | lowByte;
            A |= mem.read(absoluteAddr);
            setZNFlags(A);
            currentCycles += 4;
            break;
        }
        // ADC - Add with Carry
        case 0x69: // Immediate
        case 0x65: // Zero Page
        case 0x6D: // Absolute
        {
            uint8_t value;
            if (opcode == 0x69) {
                value = mem.read(PC++);
                currentCycles += 2;
            } else if (opcode == 0x65) {
                uint8_t zeroPageAddr = mem.read(PC++);
                value = mem.read(zeroPageAddr);
                currentCycles += 3;
            } else { // 0x6D
                uint8_t lowByte = mem.read(PC++);
                uint8_t highByte = mem.read(PC++);
                uint16_t absoluteAddr = (highByte << 8) | lowByte;
                value = mem.read(absoluteAddr);
                currentCycles += 4;
            }

            uint16_t sum = A + value + (getFlag(C) ? 1 : 0);
            
            // Set Carry flag
            if (sum > 0xFF) {
                setFlag(C);
            } else {
                clearFlag(C);
            }

            // Set Overflow flag
            // Overflow occurs if the sign of the result is different from the sign of both operands
            if ((~(A ^ value) & (A ^ sum)) & 0x80) {
                setFlag(V);
            } else {
                clearFlag(V);
            }
            
            A = sum & 0xFF;
            setZNFlags(A);
            break;
        }
        // SBC - Subtract with Carry
        case 0xE9: // Immediate
        case 0xE5: // Zero Page
        case 0xED: // Absolute
        {
            uint8_t value;
            if (opcode == 0xE9) {
                value = mem.read(PC++);
                currentCycles += 2;
            } else if (opcode == 0xE5) {
                uint8_t zeroPageAddr = mem.read(PC++);
                value = mem.read(zeroPageAddr);
                currentCycles += 3;
            } else { // 0xED
                uint8_t lowByte = mem.read(PC++);
                uint8_t highByte = mem.read(PC++);
                uint16_t absoluteAddr = (highByte << 8) | lowByte;
                value = mem.read(absoluteAddr);
                currentCycles += 4;
            }

            // SBC is equivalent to ADC with the operand's bits inverted.
            value = ~value;
            uint16_t sum = A + value + (getFlag(C) ? 1 : 0);

            // Set Carry flag (opposite of ADC)
            if (sum > 0xFF) {
                setFlag(C);
            } else {
                clearFlag(C);
            }

            // Set Overflow flag
            if ((~(A ^ value) & (A ^ sum)) & 0x80) {
                setFlag(V);
            } else {
                clearFlag(V);
            }

            A = sum & 0xFF;
            setZNFlags(A);
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
