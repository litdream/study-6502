#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"
#include <fstream>

// Test fixture for CPU class
class CPUTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    CPUTest() : cpu(mem) { // Initialize CPU with a reference to mem
        // Default setup for each test
    }

    void SetUp() override {
        // Reset memory and CPU state for each test
        mem = Memory(); // Re-initialize memory
        cpu.reset();    // Reset CPU registers to initial state

        // Ensure PC starts at a known good address for testing opcodes
        cpu.PC = 0x0000;

        // Always set the Unused flag (bit 5) to 1, as it's typically hardwired to 1 on 6502
        cpu.setFlag(CPU::U);
    }
};

// Test initial register values after reset
TEST_F(CPUTest, RegisterInitialization) {
    ASSERT_EQ(cpu.A, 0);
    ASSERT_EQ(cpu.X, 0);
    ASSERT_EQ(cpu.Y, 0);
    ASSERT_EQ(cpu.PC, 0);
    ASSERT_EQ(cpu.S, 0xFF);
    // The P register's initial value is typically 0x34 (binary 00110100) after reset on a 6502.
    // This means Interrupt Disable (I) and Unused (U) flags are set.
    // B (Break) flag is not set by reset.
    ASSERT_EQ(cpu.P, (CPU::I | CPU::U)); 
}

// Test Status Flag manipulation
TEST_F(CPUTest, StatusFlagManipulation) {
    // Test Carry Flag
    cpu.clearFlag(CPU::C);
    ASSERT_FALSE(cpu.getFlag(CPU::C));
    cpu.setFlag(CPU::C);
    ASSERT_TRUE(cpu.getFlag(CPU::C));

    // Test Zero Flag
    cpu.clearFlag(CPU::Z);
    ASSERT_FALSE(cpu.getFlag(CPU::Z));
    cpu.setFlag(CPU::Z);
    ASSERT_TRUE(cpu.getFlag(CPU::Z));

    // Test Interrupt Disable Flag
    cpu.clearFlag(CPU::I);
    ASSERT_FALSE(cpu.getFlag(CPU::I));
    cpu.setFlag(CPU::I);
    ASSERT_TRUE(cpu.getFlag(CPU::I));

    // Test Decimal Mode Flag
    cpu.clearFlag(CPU::D);
    ASSERT_FALSE(cpu.getFlag(CPU::D));
    cpu.setFlag(CPU::D);
    ASSERT_TRUE(cpu.getFlag(CPU::D));

    // Test Break Command Flag
    cpu.clearFlag(CPU::B);
    ASSERT_FALSE(cpu.getFlag(CPU::B));
    cpu.setFlag(CPU::B);
    ASSERT_TRUE(cpu.getFlag(CPU::B));

    // Test Overflow Flag
    cpu.clearFlag(CPU::V);
    ASSERT_FALSE(cpu.getFlag(CPU::V));
    cpu.setFlag(CPU::V);
    ASSERT_TRUE(cpu.getFlag(CPU::V));

    // Test Negative Flag
    cpu.clearFlag(CPU::N);
    ASSERT_FALSE(cpu.getFlag(CPU::N));
    cpu.setFlag(CPU::N);
    ASSERT_TRUE(cpu.getFlag(CPU::N));

    // Unused flag should always be 1, ensure it's not cleared by mistake
    cpu.clearFlag(CPU::U); // Try to clear it
    ASSERT_TRUE(cpu.getFlag(CPU::U)); // It should remain set due to CPU reset behavior
}

// Test basic step function (fetch and PC increment)
TEST_F(CPUTest, BasicStepExecution) {
    // Create a dummy ROM with a few NOPs (0xEA)
    std::ofstream rom_file("cpu_test_rom.bin", std::ios::binary);
    rom_file << (char)0xEA << (char)0xEA << (char)0xEA;
    rom_file.close();

    ASSERT_TRUE(mem.loadROM("cpu_test_rom.bin", 0x0000));

    cpu.PC = 0x0000;
    cpu.step();
    ASSERT_EQ(cpu.PC, 0x0001);

    cpu.step();
    ASSERT_EQ(cpu.PC, 0x0002);

    cpu.step();
    ASSERT_EQ(cpu.PC, 0x0003);
}
