#include <iostream>
#include "memory.hpp"

int main() {
    Memory mem;

    // Test writing and reading RAM
    mem.write(0x0000, 0xA5);
    mem.write(0x1234, 0xCD);

    std::cout << "Memory at 0x0000: " << std::hex << (int)mem.read(0x0000) << std::endl;
    std::cout << "Memory at 0x1234: " << std::hex << (int)mem.read(0x1234) << std::endl;

    // Test default value (should be 0)
    std::cout << "Memory at 0x0100 (unwritten): " << std::hex << (int)mem.read(0x0100) << std::endl;

    // Load ROM
    if (!mem.loadROM("monitor.rom", Memory::ROM_START)) {
        std::cerr << "Failed to load monitor.rom" << std::endl;
        return 1;
    }

    // Test reading from ROM region
    std::cout << "Memory at ROM_START (0xD000): " << std::hex << (int)mem.read(Memory::ROM_START) << std::endl;
    std::cout << "Memory at ROM_START + 1 (0xD001): " << std::hex << (int)mem.read(Memory::ROM_START + 1) << std::endl;
    std::cout << "Memory at ROM_END (0xFFFF): " << std::hex << (int)mem.read(Memory::ROM_END) << std::endl;

    return 0;
}