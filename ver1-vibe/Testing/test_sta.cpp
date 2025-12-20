#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class STATest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    STATest() : cpu(mem) {}

    void SetUp() override {
        mem = Memory();
        cpu.reset();
        cpu.PC = 0x0000;
        cpu.A = 0x00; // Reset accumulator for STA tests
        cpu.X = 0x00; // Reset X for indexed addressing tests
        cpu.Y = 0x00; // Reset Y for indexed addressing tests
        cpu.setFlag(CPU::U); // Unused flag always 1
    }

    // Helper to test STA operations
    void testSTA(uint8_t opcode, uint16_t operand_address, uint8_t value_to_store, int expected_cycles, uint16_t expected_stored_address) {
        // Given
        uint16_t currentTestAddress = 0x0000;
        cpu.A = value_to_store; // Set Accumulator to the value we want to store

        mem.write(currentTestAddress++, opcode); // STA instruction

        // Write operand(s) based on addressing mode
        switch (opcode) {
            case 0x85: // Zero Page
            case 0x95: // Zero Page, X
            case 0x81: // Indirect, X
            case 0x91: // Indirect, Y
                mem.write(currentTestAddress++, operand_address & 0xFF);
                break;
            case 0x8D: // Absolute
            case 0x9D: // Absolute, X
            case 0x99: // Absolute, Y
                mem.write(currentTestAddress++, operand_address & 0xFF);
                mem.write(currentTestAddress++, (operand_address >> 8) & 0xFF);
                break;
            default:
                FAIL() << "Unknown STA opcode for test setup: " << std::hex << (int)opcode;
        }

        // For Indirect,X and Indirect,Y, set up the indirect address
        if (opcode == 0x81) { // STA ($NN,X)
            // operand_address is the zero page address for the pointer
            uint8_t ptr_zp_addr = (operand_address + cpu.X) & 0xFF;
            mem.write(ptr_zp_addr, expected_stored_address & 0xFF);
            mem.write((ptr_zp_addr + 1) & 0xFF, (expected_stored_address >> 8) & 0xFF);
        } else if (opcode == 0x91) { // STA ($NN),Y
            // operand_address is the zero page address for the pointer
            uint16_t baseAddr = expected_stored_address - cpu.Y;
            mem.write(operand_address & 0xFF, baseAddr & 0xFF);
            mem.write((operand_address + 1) & 0xFF, (baseAddr >> 8) & 0xFF);
        }

        cpu.PC = 0x0000; // Reset PC to the start of the instruction
        uint16_t expectedPC = currentTestAddress; // PC should point after the instruction and its operand(s) after execution

        // When
        int cycles = cpu.execute(opcode);

        // Then
        ASSERT_EQ(mem.read(expected_stored_address), value_to_store); // Value should be stored correctly
        ASSERT_EQ(cpu.PC, expectedPC); // PC should advance correctly
        ASSERT_EQ(cycles, expected_cycles);
    }
};

// Test STA Zero Page Addressing Mode
TEST_F(STATest, STA_ZP) {
    testSTA(0x85, 0x10, 0xAA, 3, 0x10); // STA $10
}

// Test STA Absolute Addressing Mode
TEST_F(STATest, STA_ABS) {
    testSTA(0x8D, 0x1234, 0xBB, 4, 0x1234); // STA $1234
}

// Test STA Zero Page, X Addressing Mode
TEST_F(STATest, STA_ZP_X) {
    // With cpu.X = 0x05, 0x10 + 0x05 = 0x15
    cpu.X = 0x05;
    testSTA(0x95, 0x10, 0xCC, 4, 0x15); // STA $10,X

    // Test zero page wrap around
    cpu.X = 0xFF;
    testSTA(0x95, 0x01, 0xDD, 4, 0x00); // STA $01,X (0x01 + 0xFF = 0x00 (wrapped))
}

// Test STA Absolute, X Addressing Mode
TEST_F(STATest, STA_ABS_X) {
    // With cpu.X = 0x05, 0x1230 + 0x05 = 0x1235
    cpu.X = 0x05;
    testSTA(0x9D, 0x1230, 0xEE, 5, 0x1235); // STA $1230,X
}

// Test STA Absolute, Y Addressing Mode
TEST_F(STATest, STA_ABS_Y) {
    // With cpu.Y = 0x03, 0x1230 + 0x03 = 0x1233
    cpu.Y = 0x03;
    testSTA(0x99, 0x1230, 0xFF, 5, 0x1233); // STA $1230,Y
}

// Test STA Indirect, X Addressing Mode
TEST_F(STATest, STA_IND_X) {
    // operand_address = 0x02, cpu.X = 0x05. Indirect pointer at (0x02 + 0x05) & 0xFF = 0x07
    // Pointer at 0x07 contains 0xCDAB. So store to 0xABCD.
    cpu.X = 0x05;
    testSTA(0x81, 0x02, 0x1A, 6, 0xABCD); // STA ($02,X)
}

// Test STA Indirect, Y Addressing Mode
TEST_F(STATest, STA_IND_Y) {
    // operand_address = 0x02. Pointer at 0x02 contains 0xCDAB.
    // effective address = 0xCDAB + cpu.Y (0x03) = 0xCDBE
    cpu.Y = 0x03;
    testSTA(0x91, 0x02, 0x2B, 6, 0xCDBE); // STA ($02),Y
}
