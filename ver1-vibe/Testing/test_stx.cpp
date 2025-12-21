#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class STXTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    STXTest() : cpu(mem) {}

    void SetUp() override {
        mem = Memory();
        cpu.reset();
        cpu.PC = 0x0000;
        cpu.X = 0x42; // Set a known value for X to store
        cpu.setFlag(CPU::U); // Unused flag always 1
    }

    void testSTX(uint8_t opcode, uint16_t address, int expected_cycles) {
        // Given
        uint16_t currentTestAddress = 0x0000;
        mem.write(currentTestAddress++, opcode);

        uint16_t effectiveAddress = address;

        switch(opcode) {
            case 0x86: // Zero Page
                mem.write(currentTestAddress++, address & 0xFF);
                break;
            case 0x8E: // Absolute
                mem.write(currentTestAddress++, address & 0xFF);
                mem.write(currentTestAddress++, (address >> 8) & 0xFF);
                break;
            case 0x96: // Zero Page, Y
                cpu.Y = 0x05;
                mem.write(currentTestAddress++, address & 0xFF);
                effectiveAddress = (address + cpu.Y) & 0xFF;
                break;
        }

        cpu.PC = 0x0000;
        uint16_t expectedPC = currentTestAddress;

        // When
        int cycles = cpu.execute(opcode);

        // Then
        ASSERT_EQ(mem.read(effectiveAddress), cpu.X);
        ASSERT_EQ(cpu.PC, expectedPC);
        ASSERT_EQ(cycles, expected_cycles);
    }
};

TEST_F(STXTest, STX_ZP) {
    testSTX(0x86, 0x10, 3); // STX $10
}

TEST_F(STXTest, STX_ABS) {
    testSTX(0x8E, 0x1234, 4); // STX $1234
}

TEST_F(STXTest, STX_ZP_Y) {
    testSTX(0x96, 0x10, 4); // STX $10,Y with Y=5, address becomes 0x15
}
