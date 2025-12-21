#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class ANDTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    ANDTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

// Test AND Immediate Addressing Mode
TEST_F(ANDTest, AND_IMM) {
    // Given
    cpu.A = 0b11110000;
    mem.write(0x0000, 0x29); // AND #$10101010
    mem.write(0x0001, 0b10101010);
    
    // When
    int cycles = cpu.execute(0x29);

    // Then
    EXPECT_EQ(cpu.A, 0b10100000);
    EXPECT_EQ(cpu.PC, 0x0002);
    EXPECT_EQ(cycles, 2);
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_TRUE(cpu.getFlag(CPU::N));
}

TEST_F(ANDTest, AND_IMM_ZeroFlag) {
    // Given
    cpu.A = 0b11110000;
    mem.write(0x0000, 0x29); // AND #$00001111
    mem.write(0x0001, 0b00001111);
    
    // When
    int cycles = cpu.execute(0x29);

    // Then
    EXPECT_EQ(cpu.A, 0);
    EXPECT_EQ(cpu.PC, 0x0002);
    EXPECT_EQ(cycles, 2);
    EXPECT_TRUE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
}


// Test AND Zero Page Addressing Mode
TEST_F(ANDTest, AND_ZP) {
    // Given
    cpu.A = 0b11110000;
    mem.write(0x0010, 0b10101010);
    mem.write(0x0000, 0x25); // AND $10
    mem.write(0x0001, 0x10);

    // When
    int cycles = cpu.execute(0x25);

    // Then
    EXPECT_EQ(cpu.A, 0b10100000);
    EXPECT_EQ(cpu.PC, 0x0002);
    EXPECT_EQ(cycles, 3);
}

// Test AND Absolute Addressing Mode
TEST_F(ANDTest, AND_ABS) {
    // Given
    cpu.A = 0b11110000;
    mem.write(0x1234, 0b10101010);
    mem.write(0x0000, 0x2D); // AND $1234
    mem.write(0x0001, 0x34);
    mem.write(0x0002, 0x12);

    // When
    int cycles = cpu.execute(0x2D);

    // Then
    EXPECT_EQ(cpu.A, 0b10100000);
    EXPECT_EQ(cpu.PC, 0x0003);
    EXPECT_EQ(cycles, 4);
}