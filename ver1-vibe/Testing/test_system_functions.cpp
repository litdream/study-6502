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

TEST_F(SystemFunctionsTest, BRK) {
    uint16_t initialPC = cpu.PC;
    uint8_t initialP = cpu.P;
    mem.write(0xFFFE, 0x34); // IRQ vector low byte
    mem.write(0xFFFF, 0x12); // IRQ vector high byte
    mem.write(cpu.PC, 0x00); // BRK
    
    int cycles = cpu.execute(0x00);

    EXPECT_EQ(cpu.PC, 0x1234);
    EXPECT_TRUE(cpu.getFlag(CPU::I));
    EXPECT_EQ(cpu.S, 0xFC); // Pushed 3 bytes
    
    // Check stack contents
    EXPECT_EQ(mem.read(0x01FF), (initialPC + 2) >> 8);   // PC high
    EXPECT_EQ(mem.read(0x01FE), (initialPC + 2) & 0xFF); // PC low
    EXPECT_EQ(mem.read(0x01FD), initialP | CPU::B | CPU::U); // Status
    EXPECT_EQ(cycles, 7);
}
