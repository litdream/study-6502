#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class ROLTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    ROLTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

TEST_F(ROLTest, ROL_ACC_Simple) {
    cpu.A = 0x01;
    mem.write(0x0000, 0x2A); // ROL A
    cpu.execute(0x2A);
    EXPECT_EQ(cpu.A, 0x02);
    EXPECT_FALSE(cpu.getFlag(CPU::C));
}

TEST_F(ROLTest, ROL_ACC_WithCarryIn) {
    cpu.A = 0x01;
    cpu.setFlag(CPU::C);
    mem.write(0x0000, 0x2A);
    cpu.execute(0x2A);
    EXPECT_EQ(cpu.A, 0x03); // 0000 0001 -> 0000 0010 | 1 = 0000 0011
    EXPECT_FALSE(cpu.getFlag(CPU::C));
}

TEST_F(ROLTest, ROL_ACC_CarryOut) {
    cpu.A = 0x80;
    mem.write(0x0000, 0x2A);
    cpu.execute(0x2A);
    EXPECT_EQ(cpu.A, 0x00);
    EXPECT_TRUE(cpu.getFlag(CPU::C));
    EXPECT_TRUE(cpu.getFlag(CPU::Z));
}

TEST_F(ROLTest, ROL_ACC_CarryInAndOut) {
    cpu.A = 0x80;
    cpu.setFlag(CPU::C);
    mem.write(0x0000, 0x2A);
    cpu.execute(0x2A);
    EXPECT_EQ(cpu.A, 0x01);
    EXPECT_TRUE(cpu.getFlag(CPU::C));
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
}

TEST_F(ROLTest, ROL_ACC_NegativeResult) {
    cpu.A = 0x40;
    mem.write(0x0000, 0x2A);
    cpu.execute(0x2A);
    EXPECT_EQ(cpu.A, 0x80);
    EXPECT_TRUE(cpu.getFlag(CPU::N));
}

TEST_F(ROLTest, ROL_ZP) {
    mem.write(0x0010, 0x01);
    mem.write(0x0000, 0x26); // ROL $10
    mem.write(0x0001, 0x10);
    cpu.execute(0x26);
    EXPECT_EQ(mem.read(0x0010), 0x02);
}

TEST_F(ROLTest, ROL_ABS) {
    mem.write(0x1234, 0x80);
    cpu.setFlag(CPU::C);
    mem.write(0x0000, 0x2E); // ROL $1234
    mem.write(0x0001, 0x34);
    mem.write(0x0002, 0x12);
    cpu.execute(0x2E);
    EXPECT_EQ(mem.read(0x1234), 0x01);
    EXPECT_TRUE(cpu.getFlag(CPU::C));
}
