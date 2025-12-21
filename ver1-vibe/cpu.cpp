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
        // CMP - Compare Accumulator
        case 0xC9: // Immediate
        case 0xC5: // Zero Page
        case 0xCD: // Absolute
        {
            uint8_t value;
            if (opcode == 0xC9) {
                value = mem.read(PC++);
                currentCycles += 2;
            } else if (opcode == 0xC5) {
                uint8_t zeroPageAddr = mem.read(PC++);
                value = mem.read(zeroPageAddr);
                currentCycles += 3;
            } else { // 0xCD
                uint8_t lowByte = mem.read(PC++);
                uint8_t highByte = mem.read(PC++);
                uint16_t absoluteAddr = (highByte << 8) | lowByte;
                value = mem.read(absoluteAddr);
                currentCycles += 4;
            }

            uint8_t result = A - value;
            
            if (A >= value) {
                setFlag(C);
            } else {
                clearFlag(C);
            }
            
            setZNFlags(result);
            break;
        }
        // CPX - Compare X Register
        case 0xE0: // Immediate
        case 0xE4: // Zero Page
        case 0xEC: // Absolute
        {
            uint8_t value;
            if (opcode == 0xE0) {
                value = mem.read(PC++);
                currentCycles += 2;
            } else if (opcode == 0xE4) {
                uint8_t zeroPageAddr = mem.read(PC++);
                value = mem.read(zeroPageAddr);
                currentCycles += 3;
            } else { // 0xEC
                uint8_t lowByte = mem.read(PC++);
                uint8_t highByte = mem.read(PC++);
                uint16_t absoluteAddr = (highByte << 8) | lowByte;
                value = mem.read(absoluteAddr);
                currentCycles += 4;
            }

            uint8_t result = X - value;
            if (X >= value) setFlag(C); else clearFlag(C);
            setZNFlags(result);
            break;
        }
        // CPY - Compare Y Register
        case 0xC0: // Immediate
        case 0xC4: // Zero Page
        case 0xCC: // Absolute
        {
            uint8_t value;
            if (opcode == 0xC0) {
                value = mem.read(PC++);
                currentCycles += 2;
            } else if (opcode == 0xC4) {
                uint8_t zeroPageAddr = mem.read(PC++);
                value = mem.read(zeroPageAddr);
                currentCycles += 3;
            } else { // 0xCC
                uint8_t lowByte = mem.read(PC++);
                uint8_t highByte = mem.read(PC++);
                uint16_t absoluteAddr = (highByte << 8) | lowByte;
                value = mem.read(absoluteAddr);
                currentCycles += 4;
            }

            uint8_t result = Y - value;
            if (Y >= value) setFlag(C); else clearFlag(C);
            setZNFlags(result);
            break;
        }
        // DEC - Decrement Memory
        case 0xC6: // Zero Page
        case 0xD6: // Zero Page,X
        case 0xCE: // Absolute
        case 0xDE: // Absolute,X
        {
            uint16_t addr;
            uint8_t cycles;

            if (opcode == 0xC6) { // Zero Page
                addr = mem.read(PC++);
                cycles = 5;
            } else if (opcode == 0xD6) { // Zero Page,X
                uint8_t zeroPageAddr = mem.read(PC++);
                addr = (zeroPageAddr + X) & 0xFF; // Wrap around
                cycles = 6;
            } else if (opcode == 0xCE) { // Absolute
                uint8_t lowByte = mem.read(PC++);
                uint8_t highByte = mem.read(PC++);
                addr = (highByte << 8) | lowByte;
                cycles = 6;
            } else { // 0xDE - Absolute,X
                uint8_t lowByte = mem.read(PC++);
                uint8_t highByte = mem.read(PC++);
                uint16_t baseAddr = (highByte << 8) | lowByte;
                addr = baseAddr + X;
                cycles = 7;
            }
            
            uint8_t value = mem.read(addr);
            value--;
            mem.write(addr, value);
            setZNFlags(value);
            currentCycles += cycles;
            break;
        }
        // DEX - Decrement X Register (CA)
        case 0xCA: {
            X--;
            setZNFlags(X);
            currentCycles += 2;
            break;
        }
        // DEY - Decrement Y Register (88)
        case 0x88: {
            Y--;
            setZNFlags(Y);
            currentCycles += 2;
            break;
        }
        // INC - Increment Memory
        case 0xE6: // Zero Page
        case 0xF6: // Zero Page,X
        case 0xEE: // Absolute
        case 0xFE: // Absolute,X
        {
            uint16_t addr;
            uint8_t cycles;

            if (opcode == 0xE6) { // Zero Page
                addr = mem.read(PC++);
                cycles = 5;
            } else if (opcode == 0xF6) { // Zero Page,X
                uint8_t zeroPageAddr = mem.read(PC++);
                addr = (zeroPageAddr + X) & 0xFF; // Wrap around
                cycles = 6;
            } else if (opcode == 0xEE) { // Absolute
                uint8_t lowByte = mem.read(PC++);
                uint8_t highByte = mem.read(PC++);
                addr = (highByte << 8) | lowByte;
                cycles = 6;
            } else { // 0xFE - Absolute,X
                uint8_t lowByte = mem.read(PC++);
                uint8_t highByte = mem.read(PC++);
                uint16_t baseAddr = (highByte << 8) | lowByte;
                addr = baseAddr + X;
                cycles = 7;
            }
            
            uint8_t value = mem.read(addr);
            value++;
            mem.write(addr, value);
            setZNFlags(value);
            currentCycles += cycles;
            break;
        }
        // INX - Increment X Register (E8)
        case 0xE8: {
            X++;
            setZNFlags(X);
            currentCycles += 2;
            break;
        }
        // INY - Increment Y Register (C8)
        case 0xC8: {
            Y++;
            setZNFlags(Y);
            currentCycles += 2;
            break;
        }
        // ASL - Arithmetic Shift Left
        case 0x0A: // Accumulator
        case 0x06: // Zero Page
        case 0x16: // Zero Page,X
        case 0x0E: // Absolute
        case 0x1E: // Absolute,X
        {
            uint8_t value;
            uint16_t addr = 0;
            uint8_t cycles;

            if (opcode == 0x0A) { // Accumulator
                value = A;
                cycles = 2;
            } else {
                if (opcode == 0x06) { // Zero Page
                    addr = mem.read(PC++);
                    cycles = 5;
                } else if (opcode == 0x16) { // Zero Page,X
                    uint8_t zeroPageAddr = mem.read(PC++);
                    addr = (zeroPageAddr + X) & 0xFF;
                    cycles = 6;
                } else if (opcode == 0x0E) { // Absolute
                    uint8_t lowByte = mem.read(PC++);
                    uint8_t highByte = mem.read(PC++);
                    addr = (highByte << 8) | lowByte;
                    cycles = 6;
                } else { // 0x1E - Absolute,X
                    uint8_t lowByte = mem.read(PC++);
                    uint8_t highByte = mem.read(PC++);
                    uint16_t baseAddr = (highByte << 8) | lowByte;
                    addr = baseAddr + X;
                    cycles = 7;
                }
                value = mem.read(addr);
            }

            if (value & 0x80) setFlag(C); else clearFlag(C);
            value <<= 1;
            setZNFlags(value);

            if (opcode == 0x0A) {
                A = value;
            } else {
                mem.write(addr, value);
            }
            currentCycles += cycles;
            break;
        }
        // LSR - Logical Shift Right
        case 0x4A: // Accumulator
        case 0x46: // Zero Page
        case 0x56: // Zero Page,X
        case 0x4E: // Absolute
        case 0x5E: // Absolute,X
        {
            uint8_t value;
            uint16_t addr = 0;
            uint8_t cycles;

            if (opcode == 0x4A) { // Accumulator
                value = A;
                cycles = 2;
            } else {
                if (opcode == 0x46) { // Zero Page
                    addr = mem.read(PC++);
                    cycles = 5;
                } else if (opcode == 0x56) { // Zero Page,X
                    uint8_t zeroPageAddr = mem.read(PC++);
                    addr = (zeroPageAddr + X) & 0xFF;
                    cycles = 6;
                } else if (opcode == 0x4E) { // Absolute
                    uint8_t lowByte = mem.read(PC++);
                    uint8_t highByte = mem.read(PC++);
                    addr = (highByte << 8) | lowByte;
                    cycles = 6;
                } else { // 0x5E - Absolute,X
                    uint8_t lowByte = mem.read(PC++);
                    uint8_t highByte = mem.read(PC++);
                    uint16_t baseAddr = (highByte << 8) | lowByte;
                    addr = baseAddr + X;
                    cycles = 7;
                }
                value = mem.read(addr);
            }

            if (value & 0x01) setFlag(C); else clearFlag(C);
            value >>= 1;
            setZNFlags(value);

            if (opcode == 0x4A) {
                A = value;
            } else {
                mem.write(addr, value);
            }
            currentCycles += cycles;
            break;
        }
        // ROL - Rotate Left
        case 0x2A: // Accumulator
        case 0x26: // Zero Page
        case 0x36: // Zero Page,X
        case 0x2E: // Absolute
        case 0x3E: // Absolute,X
        {
            uint8_t value;
            uint16_t addr = 0;
            uint8_t cycles;

            if (opcode == 0x2A) { // Accumulator
                value = A;
                cycles = 2;
            } else {
                if (opcode == 0x26) { // Zero Page
                    addr = mem.read(PC++);
                    cycles = 5;
                } else if (opcode == 0x36) { // Zero Page,X
                    uint8_t zeroPageAddr = mem.read(PC++);
                    addr = (zeroPageAddr + X) & 0xFF;
                    cycles = 6;
                } else if (opcode == 0x2E) { // Absolute
                    uint8_t lowByte = mem.read(PC++);
                    uint8_t highByte = mem.read(PC++);
                    addr = (highByte << 8) | lowByte;
                    cycles = 6;
                } else { // 0x3E - Absolute,X
                    uint8_t lowByte = mem.read(PC++);
                    uint8_t highByte = mem.read(PC++);
                    uint16_t baseAddr = (highByte << 8) | lowByte;
                    addr = baseAddr + X;
                    cycles = 7;
                }
                value = mem.read(addr);
            }

            uint8_t old_carry = getFlag(C) ? 1 : 0;
            if (value & 0x80) setFlag(C); else clearFlag(C);
            value <<= 1;
            value |= old_carry;
            setZNFlags(value);

            if (opcode == 0x2A) {
                A = value;
            } else {
                mem.write(addr, value);
            }
            currentCycles += cycles;
            break;
        }
        // ROR - Rotate Right
        case 0x6A: // Accumulator
        case 0x66: // Zero Page
        case 0x76: // Zero Page,X
        case 0x6E: // Absolute
        case 0x7E: // Absolute,X
        {
            uint8_t value;
            uint16_t addr = 0;
            uint8_t cycles;

            if (opcode == 0x6A) { // Accumulator
                value = A;
                cycles = 2;
            } else {
                if (opcode == 0x66) { // Zero Page
                    addr = mem.read(PC++);
                    cycles = 5;
                } else if (opcode == 0x76) { // Zero Page,X
                    uint8_t zeroPageAddr = mem.read(PC++);
                    addr = (zeroPageAddr + X) & 0xFF;
                    cycles = 6;
                } else if (opcode == 0x6E) { // Absolute
                    uint8_t lowByte = mem.read(PC++);
                    uint8_t highByte = mem.read(PC++);
                    addr = (highByte << 8) | lowByte;
                    cycles = 6;
                } else { // 0x7E - Absolute,X
                    uint8_t lowByte = mem.read(PC++);
                    uint8_t highByte = mem.read(PC++);
                    uint16_t baseAddr = (highByte << 8) | lowByte;
                    addr = baseAddr + X;
                    cycles = 7;
                }
                value = mem.read(addr);
            }

            uint8_t old_carry = getFlag(C) ? 0x80 : 0x00;
            if (value & 0x01) setFlag(C); else clearFlag(C);
            value >>= 1;
            value |= old_carry;
            setZNFlags(value);

            if (opcode == 0x6A) {
                A = value;
            } else {
                mem.write(addr, value);
            }
            currentCycles += cycles;
            break;
        }
        // JMP - Jump
        case 0x4C: // Absolute
        {
            uint8_t lowByte = mem.read(PC++);
            uint8_t highByte = mem.read(PC++);
            PC = (highByte << 8) | lowByte;
            currentCycles += 3;
            break;
        }
        case 0x6C: // Indirect
        {
            uint8_t lowBytePtr = mem.read(PC++);
            uint8_t highBytePtr = mem.read(PC++);
            uint16_t ptr = (highBytePtr << 8) | lowBytePtr;

            uint8_t lowByte = mem.read(ptr);
            uint8_t highByte;

            // Emulate the 6502 bug where the high byte of the address is not read from the next page
            if ((ptr & 0x00FF) == 0x00FF) {
                highByte = mem.read(ptr & 0xFF00);
            } else {
                highByte = mem.read(ptr + 1);
            }
            
            PC = (highByte << 8) | lowByte;
            currentCycles += 5;
            break;
        }
        // JSR - Jump to Subroutine
        case 0x20: {
            uint8_t lowByte = mem.read(PC++);
            uint8_t highByte = mem.read(PC++);
            uint16_t targetAddr = (highByte << 8) | lowByte;
            uint16_t returnAddr = PC - 1;
            pushStack((returnAddr >> 8) & 0xFF); // Push high byte
            pushStack(returnAddr & 0xFF);       // Push low byte
            PC = targetAddr;
            currentCycles += 6;
            break;
        }
        // RTS - Return from Subroutine
        case 0x60: {
            uint8_t lowByte = popStack();
            uint8_t highByte = popStack();
            uint16_t returnAddr = (highByte << 8) | lowByte;
            PC = returnAddr + 1;
            currentCycles += 6;
            break;
        }
        // Branch Instructions
        case 0x90: branchIf(!getFlag(C)); break; // BCC
        case 0xB0: branchIf(getFlag(C)); break;  // BCS
        case 0xF0: branchIf(getFlag(Z)); break;  // BEQ
        case 0x30: branchIf(getFlag(N)); break;  // BMI
        case 0xD0: branchIf(!getFlag(Z)); break; // BNE
        case 0x10: branchIf(!getFlag(N)); break; // BPL
        case 0x50: branchIf(!getFlag(V)); break; // BVC
        case 0x70: branchIf(getFlag(V)); break;  // BVS
        default:
            std::cout << "Unknown opcode: 0x" << std::hex << (int)opcode << std::endl;
            currentCycles += 0; // Unknown opcode, assume 0 cycles or handle as an error
            break;
    }
    return currentCycles;
}

void CPU::branchIf(bool condition) {
    currentCycles += 2; // Base cycles for a branch instruction
    int8_t offset = mem.read(PC++);
    if (condition) {
        currentCycles++; // Add a cycle for a successful branch
        uint16_t oldPC = PC;
        PC += offset;
        if ((oldPC & 0xFF00) != (PC & 0xFF00)) {
            currentCycles++; // Add another cycle for crossing a page boundary
        }
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
