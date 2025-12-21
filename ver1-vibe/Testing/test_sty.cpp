#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class STYTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    STYTest() : cpu(mem) {}

    void SetUp() override {
        mem = Memory();
        cpu.reset();
        cpu.PC = 0x0000;
        cpu.Y = 0x42; // Set a known value for Y to store
        cpu.setFlag(CPU::U); // Unused flag always 1
    }

    void testSTY(uint8_t opcode, uint16_t address, int expected_cycles) {
        // Given
        uint16_t currentTestAddress = 0x0000;
        mem.write(currentTestAddress++, opcode);

        uint16_t effectiveAddress = address;

        switch(opcode) {
            case 0x84: // Zero Page
                mem.write(currentTestAddress++, address & 0xFF);
                break;
            case 0x8C: // Absolute
                mem.write(currentTestAddress++, address & 0xFF);
                mem.write(currentTestAddress++, (address >> 8) & 0xFF);
                break;
            case 0x94: // Zero Page, X
                cpu.X = 0x05;
                mem.write(currentTestAddress++, address & 0xFF);
                effectiveAddress = (address + cpu.X) & 0xFF;
                break;
        }

        cpu.PC = 0x0000;
        uint16_t expectedPC = currentTestAddress;

        // When
        int cycles = cpu.execute(opcode);

        // Then
        ASSERT_EQ(mem.read(effectiveAddress), cpu.Y);
        ASSERT_EQ(cpu.PC, expectedPC);
        ASSERT_EQ(cycles, expected_cycles);
    }
};

TEST_F(STYTest, STY_ZP) {
    testSTY(0x84, 0x10, 3); // STY $10
}

TEST_F(STYTest, STY_ABS) {
    testSTY(0x8C, 0x1234, 4); // STY $1234
}

TEST_F(STYTest, STY_ZP_X) {
    testSTY(0x94, 0x10, 4); // STY $10,X with X=5, address becomes 0x15
}
