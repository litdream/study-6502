#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class LDXTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    LDXTest() : cpu(mem) {}

    void SetUp() override {
        mem = Memory();
        cpu.reset();
        cpu.PC = 0x0000;
        cpu.setFlag(CPU::U); // Unused flag always 1
    }

    void testLDX(uint8_t opcode, uint16_t address, uint8_t value, int expected_cycles) {
        // Given
        uint16_t currentTestAddress = 0x0000;
        mem.write(currentTestAddress++, opcode); // LDX instruction

        // Set up memory and operands based on addressing mode
        switch(opcode) {
            case 0xA2: // Immediate
                mem.write(currentTestAddress++, value);
                break;
            case 0xA6: // Zero Page
                mem.write(currentTestAddress++, address & 0xFF);
                mem.write(address, value);
                break;
            case 0xAE: // Absolute
                mem.write(currentTestAddress++, address & 0xFF);
                mem.write(currentTestAddress++, (address >> 8) & 0xFF);
                mem.write(address, value);
                break;
            case 0xB6: // Zero Page, Y
                cpu.Y = 0x05; // Set Y for indexed addressing
                mem.write(currentTestAddress++, address & 0xFF);
                mem.write((address + cpu.Y) & 0xFF, value);
                break;
            case 0xBE: // Absolute, Y
                cpu.Y = 0x05; // Set Y for indexed addressing
                mem.write(currentTestAddress++, address & 0xFF);
                mem.write(currentTestAddress++, (address >> 8) & 0xFF);
                mem.write(address + cpu.Y, value);
                break;
        }

        cpu.PC = 0x0000; // Reset PC to the start of the instruction
        uint16_t expectedPC = currentTestAddress; // PC should point after the instruction and its operand(s) after execution

        // When
        int cycles = cpu.execute(opcode);

        // Then
        ASSERT_EQ(cpu.X, value);
        ASSERT_EQ(cpu.PC, expectedPC); // PC should advance correctly
        ASSERT_EQ(cpu.getFlag(CPU::Z), (value == 0));
        ASSERT_EQ(cpu.getFlag(CPU::N), ((value & 0x80) != 0));
        ASSERT_EQ(cycles, expected_cycles);
    }
};

// Test LDX Immediate Addressing Mode
TEST_F(LDXTest, LDX_IMM) {
    testLDX(0xA2, 0, 0x12, 2); // LDX #$12
    testLDX(0xA2, 0, 0x00, 2); // LDX #$00 (sets Z flag)
    testLDX(0xA2, 0, 0x80, 2); // LDX #$80 (sets N flag)
}

// Test LDX Zero Page Addressing Mode
TEST_F(LDXTest, LDX_ZP) {
    testLDX(0xA6, 0x10, 0x34, 3); // LDX $10
}

// Test LDX Absolute Addressing Mode
TEST_F(LDXTest, LDX_ABS) {
    testLDX(0xAE, 0x1234, 0x56, 4); // LDX $1234
}

// Test LDX Zero Page, Y Addressing Mode
TEST_F(LDXTest, LDX_ZP_Y) {
    testLDX(0xB6, 0x10, 0x78, 4); // LDX $10,Y with Y=5, address becomes 0x15
}

// Test LDX Absolute, Y Addressing Mode
TEST_F(LDXTest, LDX_ABS_Y) {
    testLDX(0xBE, 0x1230, 0x9A, 4); // LDX $1230,Y with Y=5, address becomes 0x1235
}
