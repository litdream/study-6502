#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class LSRTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    LSRTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

TEST_F(LSRTest, LSR_ACC_Simple) {
    cpu.A = 0x02;
    mem.write(0x0000, 0x4A); // LSR A
    cpu.execute(0x4A);
    EXPECT_EQ(cpu.A, 0x01);
    EXPECT_FALSE(cpu.getFlag(CPU::C));
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
    EXPECT_EQ(cpu.PC, 0x0001);
}

TEST_F(LSRTest, LSR_ACC_CarryOut) {
    cpu.A = 0x01;
    mem.write(0x0000, 0x4A);
    cpu.execute(0x4A);
    EXPECT_EQ(cpu.A, 0x00);
    EXPECT_TRUE(cpu.getFlag(CPU::C));
    EXPECT_TRUE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
}

TEST_F(LSRTest, LSR_ACC_NegativeIsAlwaysCleared) {
    cpu.A = 0xFF;
    mem.write(0x0000, 0x4A);
    cpu.execute(0x4A);
    EXPECT_EQ(cpu.A, 0x7F);
    EXPECT_TRUE(cpu.getFlag(CPU::C));
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
}

TEST_F(LSRTest, LSR_ZP) {
    mem.write(0x0010, 0x02);
    mem.write(0x0000, 0x46); // LSR $10
    mem.write(0x0001, 0x10);
    cpu.execute(0x46);
    EXPECT_EQ(mem.read(0x0010), 0x01);
    EXPECT_EQ(cpu.PC, 0x0002);
}

TEST_F(LSRTest, LSR_ZP_X) {
    cpu.X = 0x05;
    mem.write(0x0015, 0x04);
    mem.write(0x0000, 0x56); // LSR $10,X
    mem.write(0x0001, 0x10);
    cpu.execute(0x56);
    EXPECT_EQ(mem.read(0x0015), 0x02);
    EXPECT_EQ(cpu.PC, 0x0002);
}

TEST_F(LSRTest, LSR_ABS) {
    mem.write(0x1234, 0x08);
    mem.write(0x0000, 0x4E); // LSR $1234
    mem.write(0x0001, 0x34);
    mem.write(0x0002, 0x12);
    cpu.execute(0x4E);
    EXPECT_EQ(mem.read(0x1234), 0x04);
    EXPECT_EQ(cpu.PC, 0x0003);
}

TEST_F(LSRTest, LSR_ABS_X) {
    cpu.X = 0x05;
    mem.write(0x1239, 0x10);
    mem.write(0x0000, 0x5E); // LSR $1234,X
    mem.write(0x0001, 0x34);
    mem.write(0x0002, 0x12);
    cpu.execute(0x5E);
    EXPECT_EQ(mem.read(0x1239), 0x08);
    EXPECT_EQ(cpu.PC, 0x0003);
}
