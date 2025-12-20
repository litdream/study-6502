#include "gtest/gtest.h"
#include "memory.hpp"
#include <fstream>

// Test fixture for Memory class
class MemoryTest : public ::testing::Test {
protected:
    Memory mem;

    void SetUp() override {
        // Initialize memory with zeros for each test
        mem = Memory();
    }
};

// Test reading and writing to RAM
TEST_F(MemoryTest, ReadWriteRAM) {
    mem.write(0x1000, 0xA5);
    ASSERT_EQ(mem.read(0x1000), 0xA5);

    mem.write(0x0000, 0x11);
    ASSERT_EQ(mem.read(0x0000), 0x11);

    mem.write(Memory::RAM_END, 0xEF);
    ASSERT_EQ(mem.read(Memory::RAM_END), 0xEF);
}

// Test out of bounds access for RAM (should still access internal data array for now)
TEST_F(MemoryTest, OutOfBoundsRAMAccess) {
    mem.write(0xC000, 0xBE); // I/O region start
    ASSERT_EQ(mem.read(0xC000), 0xBE);

    mem.write(0xD000, 0xAD); // ROM region start
    ASSERT_EQ(mem.read(0xD000), 0xAD);
}

// Test ROM loading
TEST_F(MemoryTest, LoadROM) {
    // Create a dummy ROM file
    std::ofstream rom_file("test_rom.bin", std::ios::binary);
    rom_file << (char)0xDE << (char)0xAD << (char)0xBE << (char)0xEF;
    rom_file.close();

    ASSERT_TRUE(mem.loadROM("test_rom.bin", Memory::ROM_START));

    ASSERT_EQ(mem.read(Memory::ROM_START), 0xDE);
    ASSERT_EQ(mem.read(Memory::ROM_START + 1), 0xAD);
    ASSERT_EQ(mem.read(Memory::ROM_START + 2), 0xBE);
    ASSERT_EQ(mem.read(Memory::ROM_START + 3), 0xEF);

    // Check if writing to ROM is allowed (should be for current implementation)
    mem.write(Memory::ROM_START, 0xFF);
    ASSERT_EQ(mem.read(Memory::ROM_START), 0xFF);
}

// Test that unwritten memory locations are zero
TEST_F(MemoryTest, UnwrittenMemoryIsZero) {
    ASSERT_EQ(mem.read(0x0100), 0x00);
    ASSERT_EQ(mem.read(0xA000), 0x00);
}
