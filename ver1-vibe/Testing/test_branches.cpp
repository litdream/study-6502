#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class BranchTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    BranchTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0200;
    }

    void testBranch(uint8_t opcode, bool condition, bool page_cross) {
        cpu.PC = 0x0280; // Start PC in middle of a page
        int8_t offset = page_cross ? 0x85 : 0x05; // Branch forward or backward across page
        uint16_t expectedPC = cpu.PC + 2; // Default if branch not taken
        
        mem.write(cpu.PC, opcode);
        mem.write(cpu.PC + 1, offset);
        
        if (condition) {
            expectedPC = cpu.PC + offset + 2;
        }
        
        cpu.execute(opcode);
        EXPECT_EQ(cpu.PC, expectedPC);
    }
};

TEST_F(BranchTest, BCC) {
    cpu.clearFlag(CPU::C); testBranch(0x90, true, false);
    cpu.setFlag(CPU::C);   testBranch(0x90, false, false);
}

TEST_F(BranchTest, BCS) {
    cpu.setFlag(CPU::C);   testBranch(0xB0, true, false);
    cpu.clearFlag(CPU::C); testBranch(0xB0, false, false);
}

TEST_F(BranchTest, BEQ) {
    cpu.setFlag(CPU::Z);   testBranch(0xF0, true, false);
    cpu.clearFlag(CPU::Z); testBranch(0xF0, false, false);
}

TEST_F(BranchTest, BMI) {
    cpu.setFlag(CPU::N);   testBranch(0x30, true, false);
    cpu.clearFlag(CPU::N); testBranch(0x30, false, false);
}

TEST_F(BranchTest, BNE) {
    cpu.clearFlag(CPU::Z); testBranch(0xD0, true, false);
    cpu.setFlag(CPU::Z);   testBranch(0xD0, false, false);
}

TEST_F(BranchTest, BPL) {
    cpu.clearFlag(CPU::N); testBranch(0x10, true, false);
    cpu.setFlag(CPU::N);   testBranch(0x10, false, false);
}

TEST_F(BranchTest, BVC) {
    cpu.clearFlag(CPU::V); testBranch(0x50, true, false);
    cpu.setFlag(CPU::V);   testBranch(0x50, false, false);
}

TEST_F(BranchTest, BVS) {
    cpu.setFlag(CPU::V);   testBranch(0x70, true, false);
    cpu.clearFlag(CPU::V); testBranch(0x70, false, false);
}

TEST_F(BranchTest, BranchPageCrossing) {
    cpu.setFlag(CPU::C);
    cpu.PC = 0x02F0;
    mem.write(0x02F0, 0xB0); // BCS
    mem.write(0x02F1, 0x10); // Branch to 0x0302
    int cycles = cpu.execute(0xB0);
    EXPECT_EQ(cpu.PC, 0x0302);
    EXPECT_EQ(cycles, 4); // 2 (base) + 1 (branch) + 1 (page cross)
}

TEST_F(BranchTest, BranchNoPageCrossing) {
    cpu.setFlag(CPU::C);
    cpu.PC = 0x0280;
    mem.write(0x0280, 0xB0); // BCS
    mem.write(0x0281, 0x10); // Branch to 0x0292
    int cycles = cpu.execute(0xB0);
    EXPECT_EQ(cpu.PC, 0x0292);
    EXPECT_EQ(cycles, 3); // 2 (base) + 1 (branch)
}
