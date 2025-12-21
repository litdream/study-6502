#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class PLATest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    PLATest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

TEST_F(PLATest, PLA_PullsValueFromStack) {
    // Given
    mem.write(0x01FF, 0x42); // Value on stack
    cpu.S = 0xFE; // Stack pointer
    mem.write(0x0000, 0x68); // PLA

    // When
    int cycles = cpu.execute(0x68);

    // Then
    EXPECT_EQ(cpu.A, 0x42);
    EXPECT_EQ(cpu.S, 0xFF); // Stack pointer incremented
    EXPECT_EQ(cycles, 4);
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
}

TEST_F(PLATest, PLA_SetsZeroFlag) {
    // Given
    mem.write(0x01FF, 0x00); // Zero value on stack
    cpu.S = 0xFE;
    mem.write(0x0000, 0x68); // PLA

    // When
    int cycles = cpu.execute(0x68);

    // Then
    EXPECT_EQ(cpu.A, 0x00);
    EXPECT_TRUE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
}

TEST_F(PLATest, PLA_SetsNegativeFlag) {
    // Given
    mem.write(0x01FF, 0x80); // Negative value on stack
    cpu.S = 0xFE;
    mem.write(0x0000, 0x68); // PLA

    // When
    int cycles = cpu.execute(0x68);

    // Then
    EXPECT_EQ(cpu.A, 0x80);
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_TRUE(cpu.getFlag(CPU::N));
}
