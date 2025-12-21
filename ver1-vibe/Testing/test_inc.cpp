#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class INCTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    INCTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

TEST_F(INCTest, INC_ZP_Simple) {
    mem.write(0x0010, 0x05);
    mem.write(0x0000, 0xE6); // INC $10
    mem.write(0x0001, 0x10);
    cpu.execute(0xE6);
    EXPECT_EQ(mem.read(0x0010), 0x06);
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
    EXPECT_EQ(cpu.PC, 0x0002);
}

TEST_F(INCTest, INC_ZP_ZeroResult) {
    mem.write(0x0010, 0xFF);
    mem.write(0x0000, 0xE6);
    mem.write(0x0001, 0x10);
    cpu.execute(0xE6);
    EXPECT_EQ(mem.read(0x0010), 0x00);
    EXPECT_TRUE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
}

TEST_F(INCTest, INC_ZP_NegativeResult) {
    mem.write(0x0010, 0x7F);
    mem.write(0x0000, 0xE6);
    mem.write(0x0001, 0x10);
    cpu.execute(0xE6);
    EXPECT_EQ(mem.read(0x0010), 0x80);
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_TRUE(cpu.getFlag(CPU::N));
}

TEST_F(INCTest, INC_ZP_X) {
    cpu.X = 0x05;
    mem.write(0x0015, 0x0A);
    mem.write(0x0000, 0xF6); // INC $10,X
    mem.write(0x0001, 0x10);
    cpu.execute(0xF6);
    EXPECT_EQ(mem.read(0x0015), 0x0B);
    EXPECT_EQ(cpu.PC, 0x0002);
}

TEST_F(INCTest, INC_ZP_X_WrapAround) {
    cpu.X = 0xFF;
    mem.write(0x0004, 0x0A);
    mem.write(0x0000, 0xF6); // INC $05,X (0x05 + 0xFF = 0x104 -> 0x04 in ZP)
    mem.write(0x0001, 0x05);
    cpu.execute(0xF6);
    EXPECT_EQ(mem.read(0x0004), 0x0B);
    EXPECT_EQ(cpu.PC, 0x0002);
}

TEST_F(INCTest, INC_ABS) {
    mem.write(0x1234, 0x05);
    mem.write(0x0000, 0xEE); // INC $1234
    mem.write(0x0001, 0x34);
    mem.write(0x0002, 0x12);
    cpu.execute(0xEE);
    EXPECT_EQ(mem.read(0x1234), 0x06);
    EXPECT_EQ(cpu.PC, 0x0003);
}

TEST_F(INCTest, INC_ABS_X) {
    cpu.X = 0x05;
    mem.write(0x1239, 0x0A);
    mem.write(0x0000, 0xFE); // INC $1234,X
    mem.write(0x0001, 0x34);
    mem.write(0x0002, 0x12);
    cpu.execute(0xFE);
    EXPECT_EQ(mem.read(0x1239), 0x0B);
    EXPECT_EQ(cpu.PC, 0x0003);
}
