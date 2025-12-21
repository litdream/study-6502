#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class ORATest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    ORATest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

// Test ORA Immediate Addressing Mode
TEST_F(ORATest, ORA_IMM) {
    // Given
    cpu.A = 0b10100000;
    mem.write(0x0000, 0x09); // ORA #$01010101
    mem.write(0x0001, 0b01010101);
    
    // When
    int cycles = cpu.execute(0x09);

    // Then
    EXPECT_EQ(cpu.A, 0b11110101);
    EXPECT_EQ(cpu.PC, 0x0002);
    EXPECT_EQ(cycles, 2);
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_TRUE(cpu.getFlag(CPU::N));
}

TEST_F(ORATest, ORA_IMM_ZeroFlag) {
    // Given
    cpu.A = 0x00;
    mem.write(0x0000, 0x09); // ORA #$00
    mem.write(0x0001, 0x00);
    
    // When
    int cycles = cpu.execute(0x09);

    // Then
    EXPECT_EQ(cpu.A, 0);
    EXPECT_EQ(cpu.PC, 0x0002);
    EXPECT_EQ(cycles, 2);
    EXPECT_TRUE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
}

// Test ORA Zero Page Addressing Mode
TEST_F(ORATest, ORA_ZP) {
    // Given
    cpu.A = 0b10100000;
    mem.write(0x0010, 0b01010101);
    mem.write(0x0000, 0x05); // ORA $10
    mem.write(0x0001, 0x10);

    // When
    int cycles = cpu.execute(0x05);

    // Then
    EXPECT_EQ(cpu.A, 0b11110101);
    EXPECT_EQ(cpu.PC, 0x0002);
    EXPECT_EQ(cycles, 3);
}

// Test ORA Absolute Addressing Mode
TEST_F(ORATest, ORA_ABS) {
    // Given
    cpu.A = 0b10100000;
    mem.write(0x1234, 0b01010101);
    mem.write(0x0000, 0x0D); // ORA $1234
    mem.write(0x0001, 0x34);
    mem.write(0x0002, 0x12);

    // When
    int cycles = cpu.execute(0x0D);

    // Then
    EXPECT_EQ(cpu.A, 0b11110101);
    EXPECT_EQ(cpu.PC, 0x0003);
    EXPECT_EQ(cycles, 4);
}
