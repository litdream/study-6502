#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class FlagControlTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    FlagControlTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

TEST_F(FlagControlTest, CLC) {
    cpu.setFlag(CPU::C);
    mem.write(0x0000, 0x18); // CLC
    cpu.execute(0x18);
    EXPECT_FALSE(cpu.getFlag(CPU::C));
    EXPECT_EQ(cpu.PC, 0x0001);
}

TEST_F(FlagControlTest, CLD) {
    cpu.setFlag(CPU::D);
    mem.write(0x0000, 0xD8); // CLD
    cpu.execute(0xD8);
    EXPECT_FALSE(cpu.getFlag(CPU::D));
}

TEST_F(FlagControlTest, CLI) {
    cpu.setFlag(CPU::I);
    mem.write(0x0000, 0x58); // CLI
    cpu.execute(0x58);
    EXPECT_FALSE(cpu.getFlag(CPU::I));
}

TEST_F(FlagControlTest, CLV) {
    cpu.setFlag(CPU::V);
    mem.write(0x0000, 0xB8); // CLV
    cpu.execute(0xB8);
    EXPECT_FALSE(cpu.getFlag(CPU::V));
}

TEST_F(FlagControlTest, SEC) {
    cpu.clearFlag(CPU::C);
    mem.write(0x0000, 0x38); // SEC
    cpu.execute(0x38);
    EXPECT_TRUE(cpu.getFlag(CPU::C));
}

TEST_F(FlagControlTest, SED) {
    cpu.clearFlag(CPU::D);
    mem.write(0x0000, 0xF8); // SED
    cpu.execute(0xF8);
    EXPECT_TRUE(cpu.getFlag(CPU::D));
}

TEST_F(FlagControlTest, SEI) {
    cpu.clearFlag(CPU::I);
    mem.write(0x0000, 0x78); // SEI
    cpu.execute(0x78);
    EXPECT_TRUE(cpu.getFlag(CPU::I));
}
