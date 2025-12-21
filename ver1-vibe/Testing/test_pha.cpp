#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class PHATest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    PHATest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

TEST_F(PHATest, PHA_PushesAccumulatorToStack) {
    // Given
    cpu.A = 0xAA;
    cpu.S = 0xFF; // Stack pointer initially at 0xFF
    mem.write(0x0000, 0x48); // PHA

    // When
    int cycles = cpu.execute(0x48);

    // Then
    EXPECT_EQ(mem.read(0x01FF), 0xAA);
    EXPECT_EQ(cpu.S, 0xFE); // Stack pointer decremented
    EXPECT_EQ(cycles, 3);   // PHA takes 3 cycles
    EXPECT_EQ(cpu.A, 0xAA); // A register is unchanged
}

TEST_F(PHATest, PHA_PushesDifferentValue) {
    // Given
    cpu.A = 0x55;
    cpu.S = 0xFF; 
    mem.write(0x0000, 0x48); // PHA

    // When
    int cycles = cpu.execute(0x48);

    // Then
    EXPECT_EQ(mem.read(0x01FF), 0x55);
    EXPECT_EQ(cpu.S, 0xFE);
    EXPECT_EQ(cycles, 3);
}