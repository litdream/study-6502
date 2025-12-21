#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class TransferTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    TransferTest() : cpu(mem) {}

    void SetUp() override {
        mem = Memory();
        cpu.reset();
        cpu.PC = 0x0000;
        cpu.setFlag(CPU::U); // Unused flag always 1
    }

    void testTransfer(uint8_t opcode, uint8_t& from_reg, uint8_t& to_reg, uint8_t initial_value, bool check_flags) {
        // Given
        from_reg = initial_value;
        mem.write(0x0000, opcode);
        cpu.PC = 0x0000;

        // When
        int cycles = cpu.execute(opcode);

        // Then
        ASSERT_EQ(to_reg, from_reg);
        ASSERT_EQ(cpu.PC, 0x0001);
        ASSERT_EQ(cycles, 2);

        if (check_flags) {
            ASSERT_EQ(cpu.getFlag(CPU::Z), (to_reg == 0));
            ASSERT_EQ(cpu.getFlag(CPU::N), ((to_reg & 0x80) != 0));
        }
    }
};

TEST_F(TransferTest, TAX) {
    testTransfer(0xAA, cpu.A, cpu.X, 0x42, true);
    testTransfer(0xAA, cpu.A, cpu.X, 0x00, true);
    testTransfer(0xAA, cpu.A, cpu.X, 0x80, true);
}

TEST_F(TransferTest, TAY) {
    testTransfer(0xA8, cpu.A, cpu.Y, 0x42, true);
}

TEST_F(TransferTest, TSX) {
    cpu.S = 0xFF;
    testTransfer(0xBA, cpu.S, cpu.X, 0xFF, true);
}

TEST_F(TransferTest, TXA) {
    testTransfer(0x8A, cpu.X, cpu.A, 0x42, true);
}

TEST_F(TransferTest, TXS) {
    // TXS does not affect flags
    cpu.X = 0x42;
    mem.write(0x0000, 0x9A);
    cpu.PC = 0x0000;
    int cycles = cpu.execute(0x9A);
    ASSERT_EQ(cpu.S, 0x42);
    ASSERT_EQ(cpu.PC, 0x0001);
    ASSERT_EQ(cycles, 2);
}

TEST_F(TransferTest, TYA) {
    testTransfer(0x98, cpu.Y, cpu.A, 0x42, true);
}
