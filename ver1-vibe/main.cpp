#include <iostream>
#include <fstream>
#include "memory.hpp"
#include "cpu.hpp"

int main() {
    Memory mem;
    CPU cpu(mem);

    std::cout << "Apple II Emulator: Memory initialized." << std::endl;

    // Load a dummy ROM for testing CPU fetch
    // Create a dummy ROM file with a few arbitrary opcodes (e.g., NOP, NOP, BRK)
    // (0xEA is NOP, 0x00 is BRK)
    std::ofstream rom_file("boot.rom", std::ios::binary);
    rom_file << (char)0xEA << (char)0xEA << (char)0x00;
    rom_file.close();

    if (!mem.loadROM("boot.rom", 0x0000)) { // Load ROM at address 0x0000 for simple testing
        std::cerr << "Failed to load boot.rom" << std::endl;
        return 1;
    }

    cpu.PC = 0x0000; // Set PC to the start of our dummy ROM

    std::cout << "\n--- CPU Step Test ---\n";
    cpu.step(); // Fetch 0xEA
    cpu.step(); // Fetch 0xEA
    cpu.step(); // Fetch 0x00
    std::cout << "--- End CPU Step Test ---\n";

    return 0;
}
