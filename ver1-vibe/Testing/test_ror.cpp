#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class RORTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    RORTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

TEST_F(RORTest, ROR_ACC_Simple) {
    cpu.A = 0x02;
    mem.write(0x0000, 0x6A); // ROR A
    cpu.execute(0x6A);
    EXPECT_EQ(cpu.A, 0x01);
    EXPECT_FALSE(cpu.getFlag(CPU::C));
}

TEST_F(RORTest, ROR_ACC_WithCarryIn) {
    cpu.A = 0x02;
    cpu.setFlag(CPU::C);
    mem.write(0x0000, 0x6A);
    cpu.execute(0x6A);
    EXPECT_EQ(cpu.A, 0x81); // 0000 0010 -> 0000 0001 | 1000 0000 = 1000 0001
    EXPECT_FALSE(cpu.getFlag(CPU::C));
}

TEST_F(RORTest, ROR_ACC_CarryOut) {
    cpu.A = 0x01;
    mem.write(0x0000, 0x6A);
    cpu.execute(0x6A);
    EXPECT_EQ(cpu.A, 0x00);
    EXPECT_TRUE(cpu.getFlag(CPU::C));
    EXPECT_TRUE(cpu.getFlag(CPU::Z));
}

TEST_F(RORTest, ROR_ACC_CarryInAndOut) {
    cpu.A = 0x01;
    cpu.setFlag(CPU::C);
    mem.write(0x0000, 0x6A);
    cpu.execute(0x6A);
    EXPECT_EQ(cpu.A, 0x80);
    EXPECT_TRUE(cpu.getFlag(CPU::C));
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_TRUE(cpu.getFlag(CPU::N));
}

TEST_F(RORTest, ROR_ZP) {
    mem.write(0x0010, 0x02);
    mem.write(0x0000, 0x66); // ROR $10
    mem.write(0x0001, 0x10);
    cpu.execute(0x66);
    EXPECT_EQ(mem.read(0x0010), 0x01);
}

TEST_F(RORTest, ROR_ABS) {
    mem.write(0x1234, 0x01);
    cpu.setFlag(CPU::C);
    mem.write(0x0000, 0x6E); // ROR $1234
    mem.write(0x0001, 0x34);
    mem.write(0x0002, 0x12);
    cpu.execute(0x6E);
    EXPECT_EQ(mem.read(0x1234), 0x80);
    EXPECT_TRUE(cpu.getFlag(CPU::C));
}
