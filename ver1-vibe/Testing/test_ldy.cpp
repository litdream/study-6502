#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class LDYTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    LDYTest() : cpu(mem) {}

    void SetUp() override {
        mem = Memory();
        cpu.reset();
        cpu.PC = 0x0000;
        cpu.setFlag(CPU::U); // Unused flag always 1
    }

    void testLDY(uint8_t opcode, uint16_t address, uint8_t value, int expected_cycles) {
        // Given
        uint16_t currentTestAddress = 0x0000;
        mem.write(currentTestAddress++, opcode); // LDY instruction

        // Set up memory and operands based on addressing mode
        switch(opcode) {
            case 0xA0: // Immediate
                mem.write(currentTestAddress++, value);
                break;
            case 0xA4: // Zero Page
                mem.write(currentTestAddress++, address & 0xFF);
                mem.write(address, value);
                break;
            case 0xAC: // Absolute
                mem.write(currentTestAddress++, address & 0xFF);
                mem.write(currentTestAddress++, (address >> 8) & 0xFF);
                mem.write(address, value);
                break;
            case 0xB4: // Zero Page, X
                cpu.X = 0x05; // Set X for indexed addressing
                mem.write(currentTestAddress++, address & 0xFF);
                mem.write((address + cpu.X) & 0xFF, value);
                break;
            case 0xBC: // Absolute, X
                cpu.X = 0x05; // Set X for indexed addressing
                mem.write(currentTestAddress++, address & 0xFF);
                mem.write(currentTestAddress++, (address >> 8) & 0xFF);
                mem.write(address + cpu.X, value);
                break;
        }

        cpu.PC = 0x0000; // Reset PC to the start of the instruction
        uint16_t expectedPC = currentTestAddress; // PC should point after the instruction and its operand(s) after execution

        // When
        int cycles = cpu.execute(opcode);

        // Then
        ASSERT_EQ(cpu.Y, value);
        ASSERT_EQ(cpu.PC, expectedPC); // PC should advance correctly
        ASSERT_EQ(cpu.getFlag(CPU::Z), (value == 0));
        ASSERT_EQ(cpu.getFlag(CPU::N), ((value & 0x80) != 0));
        ASSERT_EQ(cycles, expected_cycles);
    }
};

// Test LDY Immediate Addressing Mode
TEST_F(LDYTest, LDY_IMM) {
    testLDY(0xA0, 0, 0x12, 2); // LDY #$12
    testLDY(0xA0, 0, 0x00, 2); // LDY #$00 (sets Z flag)
    testLDY(0xA0, 0, 0x80, 2); // LDY #$80 (sets N flag)
}

// Test LDY Zero Page Addressing Mode
TEST_F(LDYTest, LDY_ZP) {
    testLDY(0xA4, 0x10, 0x34, 3); // LDY $10
}

// Test LDY Absolute Addressing Mode
TEST_F(LDYTest, LDY_ABS) {
    testLDY(0xAC, 0x1234, 0x56, 4); // LDY $1234
}

// Test LDY Zero Page, X Addressing Mode
TEST_F(LDYTest, LDY_ZP_X) {
    testLDY(0xB4, 0x10, 0x78, 4); // LDY $10,X with X=5, address becomes 0x15
}

// Test LDY Absolute, X Addressing Mode
TEST_F(LDYTest, LDY_ABS_X) {
    testLDY(0xBC, 0x1230, 0x9A, 4); // LDY $1230,X with X=5, address becomes 0x1235
}
