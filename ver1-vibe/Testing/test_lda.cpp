#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class LDATest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    LDATest() : cpu(mem) {}

    void SetUp() override {
        mem = Memory();
        cpu.reset();
        cpu.PC = 0x0000;
        cpu.setFlag(CPU::U); // Unused flag always 1
    }

    void testLDA(uint8_t opcode, uint16_t address, uint8_t value, int expected_cycles) {
        // Given
        uint16_t currentTestAddress = 0x0000;
        mem.write(currentTestAddress++, opcode); // LDA instruction
        if (opcode == 0xA9) { // Immediate
            mem.write(currentTestAddress++, value);
        } else if (opcode == 0xA5) { // Zero Page
            mem.write(currentTestAddress++, address & 0xFF);
            mem.write(address, value);
        } else if (opcode == 0xAD) { // Absolute
            mem.write(currentTestAddress++, address & 0xFF);
            mem.write(currentTestAddress++, (address >> 8) & 0xFF);
            mem.write(address, value);
        }

        cpu.PC = 0x0000; // Reset PC to the start of the instruction
        uint16_t expectedPC = currentTestAddress; // PC should point after the instruction and its operand(s) after execution

        // When
        int cycles = cpu.execute(opcode);

        // Then
        ASSERT_EQ(cpu.A, value);
        ASSERT_EQ(cpu.PC, expectedPC); // PC should advance correctly by execute
        ASSERT_EQ(cpu.getFlag(CPU::Z), (value == 0));
        ASSERT_EQ(cpu.getFlag(CPU::N), ((value & 0x80) != 0));
        ASSERT_EQ(cycles, expected_cycles);
    }
};

// Test LDA Immediate Addressing Mode
TEST_F(LDATest, LDA_IMM) {
    testLDA(0xA9, 0, 0x12, 2); // LDA #$12
    testLDA(0xA9, 0, 0x00, 2); // LDA #$00 (sets Z flag)
    testLDA(0xA9, 0, 0x80, 2); // LDA #$80 (sets N flag)
}

// Test LDA Zero Page Addressing Mode
TEST_F(LDATest, LDA_ZP) {
    testLDA(0xA5, 0x10, 0x34, 3); // LDA $10
    testLDA(0xA5, 0x20, 0x00, 3); // LDA $20 (sets Z flag)
    testLDA(0xA5, 0x30, 0xA0, 3); // LDA $30 (sets N flag)
}

// Test LDA Absolute Addressing Mode
TEST_F(LDATest, LDA_ABS) {
    testLDA(0xAD, 0x1234, 0x56, 4); // LDA $1234
    testLDA(0xAD, 0xABCD, 0x00, 4); // LDA $ABCD (sets Z flag)
    testLDA(0xAD, 0xEF00, 0xC0, 4); // LDA $EF00 (sets N flag)
}
