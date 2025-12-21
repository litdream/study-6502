#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class SBCTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    SBCTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
        cpu.setFlag(CPU::C); // SBC usually starts with carry set (no borrow)
    }
};

TEST_F(SBCTest, SBC_IMM_SimpleSubtraction) {
    cpu.A = 0x15;
    mem.write(0x0000, 0xE9); // SBC #$10
    mem.write(0x0001, 0x10);
    cpu.execute(0xE9);
    EXPECT_EQ(cpu.A, 0x05);
    EXPECT_TRUE(cpu.getFlag(CPU::C)); // No borrow
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
    EXPECT_FALSE(cpu.getFlag(CPU::V));
}

TEST_F(SBCTest, SBC_IMM_WithoutCarryIn_Borrow) {
    cpu.A = 0x15;
    cpu.clearFlag(CPU::C); // With borrow
    mem.write(0x0000, 0xE9); // SBC #$10
    mem.write(0x0001, 0x10);
    cpu.execute(0xE9);
    EXPECT_EQ(cpu.A, 0x04); // 0x15 - 0x10 - 1 = 4
    EXPECT_TRUE(cpu.getFlag(CPU::C)); // No borrow on 21-17
}

TEST_F(SBCTest, SBC_IMM_Borrow) {
    cpu.A = 0x00;
    mem.write(0x0000, 0xE9); // SBC #$01
    mem.write(0x0001, 0x01);
    cpu.execute(0xE9);
    EXPECT_EQ(cpu.A, 0xFF); // 0 - 1 = -1
    EXPECT_FALSE(cpu.getFlag(CPU::C)); // Carry clear indicates borrow
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_TRUE(cpu.getFlag(CPU::N));
    EXPECT_FALSE(cpu.getFlag(CPU::V));
}

TEST_F(SBCTest, SBC_IMM_OverflowPositiveToNegative) {
    cpu.A = 0x80; // -128
    mem.write(0x0000, 0xE9); // SBC #$01
    mem.write(0x0001, 0x01); // 1
    cpu.execute(0xE9);
    EXPECT_EQ(cpu.A, 0x7F); // -128 - 1 = -129 -> wraps to 127
    EXPECT_TRUE(cpu.getFlag(CPU::C)); // 0x80 > 0x01, no borrow
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
    EXPECT_TRUE(cpu.getFlag(CPU::V));
}

TEST_F(SBCTest, SBC_IMM_OverflowNegativeToPositive) {
    cpu.A = 0x7F; // 127
    mem.write(0x0000, 0xE9); // SBC #$FF
    mem.write(0x0001, 0xFF); // -1
    cpu.execute(0xE9);
    EXPECT_EQ(cpu.A, 0x80); // 127 - (-1) = 128 -> wraps to -128
    EXPECT_FALSE(cpu.getFlag(CPU::C)); // 0x7F < 0xFF, borrow
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_TRUE(cpu.getFlag(CPU::N));
    EXPECT_TRUE(cpu.getFlag(CPU::V));
}

TEST_F(SBCTest, SBC_ZP) {
    cpu.A = 0x15;
    mem.write(0x0020, 0x10);
    mem.write(0x0000, 0xE5); // SBC $20
    mem.write(0x0001, 0x20);
    cpu.execute(0xE5);
    EXPECT_EQ(cpu.A, 0x05);
}

TEST_F(SBCTest, SBC_ABS) {
    cpu.A = 0x15;
    mem.write(0x1234, 0x10);
    mem.write(0x0000, 0xED); // SBC $1234
    mem.write(0x0001, 0x34);
    mem.write(0x0002, 0x12);
    cpu.execute(0xED);
    EXPECT_EQ(cpu.A, 0x05);
}