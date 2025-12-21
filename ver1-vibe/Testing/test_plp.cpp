#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class PLPTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    PLPTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

TEST_F(PLPTest, PLP_PullsStatusFromStack) {
    // Given
    uint8_t current_p_b_and_u = cpu.P & (CPU::B | CPU::U);
    uint8_t status_on_stack = 0b11001100;
    mem.write(0x01FF, status_on_stack);
    cpu.S = 0xFE;
    mem.write(0x0000, 0x28); // PLP

    // When
    int cycles = cpu.execute(0x28);

    // Then
    // The B and U flags are not affected by a PLP instruction.
    uint8_t expected_p = (status_on_stack & ~CPU::B & ~CPU::U) | current_p_b_and_u;
    EXPECT_EQ(cpu.P, expected_p);
    EXPECT_EQ(cpu.S, 0xFF); // Stack pointer incremented
    EXPECT_EQ(cycles, 4);
}
