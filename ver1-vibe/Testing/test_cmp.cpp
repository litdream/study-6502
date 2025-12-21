#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class CMPTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    CMPTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

TEST_F(CMPTest, CMP_IMM_AGreater) {
    cpu.A = 0x15;
    mem.write(0x0000, 0xC9); // CMP #$10
    mem.write(0x0001, 0x10);
    cpu.execute(0xC9);
    EXPECT_EQ(cpu.A, 0x15); // A is not changed
    EXPECT_TRUE(cpu.getFlag(CPU::C));
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
}

TEST_F(CMPTest, CMP_IMM_AEqual) {
    cpu.A = 0x10;
    mem.write(0x0000, 0xC9); // CMP #$10
    mem.write(0x0001, 0x10);
    cpu.execute(0xC9);
    EXPECT_EQ(cpu.A, 0x10);
    EXPECT_TRUE(cpu.getFlag(CPU::C));
    EXPECT_TRUE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
}

TEST_F(CMPTest, CMP_IMM_ALess) {
    cpu.A = 0x05;
    mem.write(0x0000, 0xC9); // CMP #$10
    mem.write(0x0001, 0x10);
    cpu.execute(0xC9);
    EXPECT_EQ(cpu.A, 0x05);
    EXPECT_FALSE(cpu.getFlag(CPU::C));
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_TRUE(cpu.getFlag(CPU::N)); // 5 - 16 = -11, which is negative
}

TEST_F(CMPTest, CMP_ZP) {
    cpu.A = 0x15;
    mem.write(0x0020, 0x10);
    mem.write(0x0000, 0xC5); // CMP $20
    mem.write(0x0001, 0x20);
    cpu.execute(0xC5);
    EXPECT_TRUE(cpu.getFlag(CPU::C));
}

TEST_F(CMPTest, CMP_ABS) {
    cpu.A = 0x15;
    mem.write(0x1234, 0x10);
    mem.write(0x0000, 0xCD); // CMP $1234
    mem.write(0x0001, 0x34);
    mem.write(0x0002, 0x12);
    cpu.execute(0xCD);
    EXPECT_TRUE(cpu.getFlag(CPU::C));
}
