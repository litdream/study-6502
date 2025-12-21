#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class ASLTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    ASLTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

TEST_F(ASLTest, ASL_ACC_Simple) {
    cpu.A = 0x01;
    mem.write(0x0000, 0x0A); // ASL A
    cpu.execute(0x0A);
    EXPECT_EQ(cpu.A, 0x02);
    EXPECT_FALSE(cpu.getFlag(CPU::C));
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
    EXPECT_EQ(cpu.PC, 0x0001);
}

TEST_F(ASLTest, ASL_ACC_CarryOut) {
    cpu.A = 0x80;
    mem.write(0x0000, 0x0A);
    cpu.execute(0x0A);
    EXPECT_EQ(cpu.A, 0x00);
    EXPECT_TRUE(cpu.getFlag(CPU::C));
    EXPECT_TRUE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
}

TEST_F(ASLTest, ASL_ACC_NegativeResult) {
    cpu.A = 0x40;
    mem.write(0x0000, 0x0A);
    cpu.execute(0x0A);
    EXPECT_EQ(cpu.A, 0x80);
    EXPECT_FALSE(cpu.getFlag(CPU::C));
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_TRUE(cpu.getFlag(CPU::N));
}

TEST_F(ASLTest, ASL_ZP) {
    mem.write(0x0010, 0x01);
    mem.write(0x0000, 0x06); // ASL $10
    mem.write(0x0001, 0x10);
    cpu.execute(0x06);
    EXPECT_EQ(mem.read(0x0010), 0x02);
    EXPECT_EQ(cpu.PC, 0x0002);
}

TEST_F(ASLTest, ASL_ZP_X) {
    cpu.X = 0x05;
    mem.write(0x0015, 0x02);
    mem.write(0x0000, 0x16); // ASL $10,X
    mem.write(0x0001, 0x10);
    cpu.execute(0x16);
    EXPECT_EQ(mem.read(0x0015), 0x04);
    EXPECT_EQ(cpu.PC, 0x0002);
}

TEST_F(ASLTest, ASL_ABS) {
    mem.write(0x1234, 0x04);
    mem.write(0x0000, 0x0E); // ASL $1234
    mem.write(0x0001, 0x34);
    mem.write(0x0002, 0x12);
    cpu.execute(0x0E);
    EXPECT_EQ(mem.read(0x1234), 0x08);
    EXPECT_EQ(cpu.PC, 0x0003);
}

TEST_F(ASLTest, ASL_ABS_X) {
    cpu.X = 0x05;
    mem.write(0x1239, 0x08);
    mem.write(0x0000, 0x1E); // ASL $1234,X
    mem.write(0x0001, 0x34);
    mem.write(0x0002, 0x12);
    cpu.execute(0x1E);
    EXPECT_EQ(mem.read(0x1239), 0x10);
    EXPECT_EQ(cpu.PC, 0x0003);
}
