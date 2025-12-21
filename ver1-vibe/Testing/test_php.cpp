#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class PHPTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    PHPTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

TEST_F(PHPTest, PHP_PushesStatusToStack) {
    // Given
    cpu.P = 0b11001100; // Example status
    cpu.S = 0xFF; // Stack pointer
    mem.write(0x0000, 0x08); // PHP

    // When
    int cycles = cpu.execute(0x08);

    // Then
    // B and U flags are set to 1 before pushing
    uint8_t expected_status = cpu.P | CPU::B | CPU::U;
    EXPECT_EQ(mem.read(0x01FF), expected_status);
    EXPECT_EQ(cpu.S, 0xFE); // Stack pointer decremented
    EXPECT_EQ(cycles, 3);
}
