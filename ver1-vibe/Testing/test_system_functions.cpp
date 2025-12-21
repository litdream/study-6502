#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class SystemFunctionsTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    SystemFunctionsTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0200;
    }
};

TEST_F(SystemFunctionsTest, NOP) {
    uint16_t initialPC = cpu.PC;
    mem.write(cpu.PC, 0xEA); // NOP
    int cycles = cpu.execute(0xEA);
    EXPECT_EQ(cpu.PC, initialPC + 1);
    EXPECT_EQ(cycles, 2);
    // Ensure no registers or flags were changed
    uint8_t initialP = cpu.P;
    uint8_t initialA = cpu.A;
    uint8_t initialX = cpu.X;
    uint8_t initialY = cpu.Y;
    uint8_t initialS = cpu.S;
    
    cpu.execute(0xEA);

    EXPECT_EQ(cpu.P, initialP);
    EXPECT_EQ(cpu.A, initialA);
    EXPECT_EQ(cpu.X, initialX);
    EXPECT_EQ(cpu.Y, initialY);
    EXPECT_EQ(cpu.S, initialS);
}
