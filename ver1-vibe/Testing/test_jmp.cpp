#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class JMPTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    JMPTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

TEST_F(JMPTest, JMP_ABS) {
    mem.write(0x0000, 0x4C); // JMP $1234
    mem.write(0x0001, 0x34);
    mem.write(0x0002, 0x12);
    cpu.execute(0x4C);
    EXPECT_EQ(cpu.PC, 0x1234);
}

TEST_F(JMPTest, JMP_IND) {
    mem.write(0x1000, 0xCD); // Target address low byte
    mem.write(0x1001, 0xAB); // Target address high byte
    mem.write(0x0000, 0x6C); // JMP ($1000)
    mem.write(0x0001, 0x00);
    mem.write(0x0002, 0x10);
    cpu.execute(0x6C);
    EXPECT_EQ(cpu.PC, 0xABCD);
}

TEST_F(JMPTest, JMP_IND_PageBoundaryBug) {
    mem.write(0x01FF, 0xCD); // Target address low byte at page boundary
    mem.write(0x0100, 0xAB); // High byte is read from start of page, not next page
    mem.write(0x0000, 0x6C); // JMP ($01FF)
    mem.write(0x0001, 0xFF);
    mem.write(0x0002, 0x01);
    cpu.execute(0x6C);
    EXPECT_EQ(cpu.PC, 0xABCD);
}
