#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class INXINYTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    INXINYTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

TEST_F(INXINYTest, INX_Simple) {
    cpu.X = 0x05;
    mem.write(0x0000, 0xE8); // INX
    cpu.execute(0xE8);
    EXPECT_EQ(cpu.X, 0x06);
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
    EXPECT_EQ(cpu.PC, 0x0001);
}

TEST_F(INXINYTest, INX_ZeroResult) {
    cpu.X = 0xFF;
    mem.write(0x0000, 0xE8);
    cpu.execute(0xE8);
    EXPECT_EQ(cpu.X, 0x00);
    EXPECT_TRUE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
}

TEST_F(INXINYTest, INX_NegativeResult) {
    cpu.X = 0x7F;
    mem.write(0x0000, 0xE8);
    cpu.execute(0xE8);
    EXPECT_EQ(cpu.X, 0x80);
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_TRUE(cpu.getFlag(CPU::N));
}

TEST_F(INXINYTest, INY_Simple) {
    cpu.Y = 0x05;
    mem.write(0x0000, 0xC8); // INY
    cpu.execute(0xC8);
    EXPECT_EQ(cpu.Y, 0x06);
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
    EXPECT_EQ(cpu.PC, 0x0001);
}

TEST_F(INXINYTest, INY_ZeroResult) {
    cpu.Y = 0xFF;
    mem.write(0x0000, 0xC8);
    cpu.execute(0xC8);
    EXPECT_EQ(cpu.Y, 0x00);
    EXPECT_TRUE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
}

TEST_F(INXINYTest, INY_NegativeResult) {
    cpu.Y = 0x7F;
    mem.write(0x0000, 0xC8);
    cpu.execute(0xC8);
    EXPECT_EQ(cpu.Y, 0x80);
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_TRUE(cpu.getFlag(CPU::N));
}
