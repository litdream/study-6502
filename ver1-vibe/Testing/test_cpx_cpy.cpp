#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class CPXTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    CPXTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

TEST_F(CPXTest, CPX_IMM_XGreater) {
    cpu.X = 0x15;
    mem.write(0x0000, 0xE0); // CPX #$10
    mem.write(0x0001, 0x10);
    cpu.execute(0xE0);
    EXPECT_EQ(cpu.X, 0x15);
    EXPECT_TRUE(cpu.getFlag(CPU::C));
}

TEST_F(CPXTest, CPX_ZP_XEqual) {
    cpu.X = 0x10;
    mem.write(0x0020, 0x10);
    mem.write(0x0000, 0xE4); // CPX $20
    mem.write(0x0001, 0x20);
    cpu.execute(0xE4);
    EXPECT_TRUE(cpu.getFlag(CPU::Z));
}

TEST_F(CPXTest, CPX_ABS_XLess) {
    cpu.X = 0x05;
    mem.write(0x1234, 0x10);
    mem.write(0x0000, 0xEC); // CPX $1234
    mem.write(0x0001, 0x34);
    mem.write(0x0002, 0x12);
    cpu.execute(0xEC);
    EXPECT_FALSE(cpu.getFlag(CPU::C));
}

class CPYTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    CPYTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

TEST_F(CPYTest, CPY_IMM_YGreater) {
    cpu.Y = 0x15;
    mem.write(0x0000, 0xC0); // CPY #$10
    mem.write(0x0001, 0x10);
    cpu.execute(0xC0);
    EXPECT_EQ(cpu.Y, 0x15);
    EXPECT_TRUE(cpu.getFlag(CPU::C));
}

TEST_F(CPYTest, CPY_ZP_YEqual) {
    cpu.Y = 0x10;
    mem.write(0x0020, 0x10);
    mem.write(0x0000, 0xC4); // CPY $20
    mem.write(0x0001, 0x20);
    cpu.execute(0xC4);
    EXPECT_TRUE(cpu.getFlag(CPU::Z));
}

TEST_F(CPYTest, CPY_ABS_YLess) {
    cpu.Y = 0x05;
    mem.write(0x1234, 0x10);
    mem.write(0x0000, 0xCC); // CPY $1234
    mem.write(0x0001, 0x34);
    mem.write(0x0002, 0x12);
    cpu.execute(0xCC);
    EXPECT_FALSE(cpu.getFlag(CPU::C));
}
