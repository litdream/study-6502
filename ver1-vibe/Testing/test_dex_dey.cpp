#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class DEXDEYTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    DEXDEYTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

TEST_F(DEXDEYTest, DEX_Simple) {
    cpu.X = 0x05;
    mem.write(0x0000, 0xCA); // DEX
    cpu.execute(0xCA);
    EXPECT_EQ(cpu.X, 0x04);
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
    EXPECT_EQ(cpu.PC, 0x0001);
}

TEST_F(DEXDEYTest, DEX_ZeroResult) {
    cpu.X = 0x01;
    mem.write(0x0000, 0xCA);
    cpu.execute(0xCA);
    EXPECT_EQ(cpu.X, 0x00);
    EXPECT_TRUE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
}

TEST_F(DEXDEYTest, DEX_NegativeResult) {
    cpu.X = 0x00;
    mem.write(0x0000, 0xCA);
    cpu.execute(0xCA);
    EXPECT_EQ(cpu.X, 0xFF);
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_TRUE(cpu.getFlag(CPU::N));
}

TEST_F(DEXDEYTest, DEY_Simple) {
    cpu.Y = 0x05;
    mem.write(0x0000, 0x88); // DEY
    cpu.execute(0x88);
    EXPECT_EQ(cpu.Y, 0x04);
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
    EXPECT_EQ(cpu.PC, 0x0001);
}

TEST_F(DEXDEYTest, DEY_ZeroResult) {
    cpu.Y = 0x01;
    mem.write(0x0000, 0x88);
    cpu.execute(0x88);
    EXPECT_EQ(cpu.Y, 0x00);
    EXPECT_TRUE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
}

TEST_F(DEXDEYTest, DEY_NegativeResult) {
    cpu.Y = 0x00;
    mem.write(0x0000, 0x88);
    cpu.execute(0x88);
    EXPECT_EQ(cpu.Y, 0xFF);
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_TRUE(cpu.getFlag(CPU::N));
}
