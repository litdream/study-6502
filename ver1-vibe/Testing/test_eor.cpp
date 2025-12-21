#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class EORTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    EORTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

// Test EOR Immediate Addressing Mode
TEST_F(EORTest, EOR_IMM) {
    // Given
    cpu.A = 0b11110000;
    mem.write(0x0000, 0x49); // EOR #$10101010
    mem.write(0x0001, 0b10101010);
    
    // When
    int cycles = cpu.execute(0x49);

    // Then
    EXPECT_EQ(cpu.A, 0b01011010);
    EXPECT_EQ(cpu.PC, 0x0002);
    EXPECT_EQ(cycles, 2);
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
}

TEST_F(EORTest, EOR_IMM_ZeroFlag) {
    // Given
    cpu.A = 0b10101010;
    mem.write(0x0000, 0x49); // EOR #$10101010
    mem.write(0x0001, 0b10101010);
    
    // When
    int cycles = cpu.execute(0x49);

    // Then
    EXPECT_EQ(cpu.A, 0);
    EXPECT_EQ(cpu.PC, 0x0002);
    EXPECT_EQ(cycles, 2);
    EXPECT_TRUE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
}

TEST_F(EORTest, EOR_IMM_NegativeFlag) {
    // Given
    cpu.A = 0b01110000;
    mem.write(0x0000, 0x49); // EOR #$10101010
    mem.write(0x0001, 0b10000000);
    
    // When
    int cycles = cpu.execute(0x49);

    // Then
    EXPECT_EQ(cpu.A, 0b11110000);
    EXPECT_EQ(cpu.PC, 0x0002);
    EXPECT_EQ(cycles, 2);
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_TRUE(cpu.getFlag(CPU::N));
}

// Test EOR Zero Page Addressing Mode
TEST_F(EORTest, EOR_ZP) {
    // Given
    cpu.A = 0b11110000;
    mem.write(0x0010, 0b10101010);
    mem.write(0x0000, 0x45); // EOR $10
    mem.write(0x0001, 0x10);

    // When
    int cycles = cpu.execute(0x45);

    // Then
    EXPECT_EQ(cpu.A, 0b01011010);
    EXPECT_EQ(cpu.PC, 0x0002);
    EXPECT_EQ(cycles, 3);
}

// Test EOR Absolute Addressing Mode
TEST_F(EORTest, EOR_ABS) {
    // Given
    cpu.A = 0b11110000;
    mem.write(0x1234, 0b10101010);
    mem.write(0x0000, 0x4D); // EOR $1234
    mem.write(0x0001, 0x34);
    mem.write(0x0002, 0x12);

    // When
    int cycles = cpu.execute(0x4D);

    // Then
    EXPECT_EQ(cpu.A, 0b01011010);
    EXPECT_EQ(cpu.PC, 0x0003);
    EXPECT_EQ(cycles, 4);
}
