#include "memory.hpp"
#include <iostream>
#include <fstream>

Memory::Memory() : data(ADDRESS_SPACE_SIZE) {
    // Initialize memory with zeros
    for (uint32_t i = 0; i < ADDRESS_SPACE_SIZE; ++i) {
        data[i] = 0;
    }
}

uint8_t Memory::read(uint16_t address) {
    if (address >= RAM_START && address <= RAM_END) {
        // RAM access
        return data[address];
    } else if (address >= IO_START && address <= IO_END) {
        // I/O Soft Switches access (future implementation)
        // For now, just return data from the corresponding memory location
        std::cout << "Reading from I/O address: 0x" << std::hex << address << std::endl;
        return data[address];
    } else if (address >= ROM_START && address <= ROM_END) {
        // ROM access (future implementation)
        // For now, just return data from the corresponding memory location
        std::cout << "Reading from ROM address: 0x" << std::hex << address << std::endl;
        return data[address];
    }
    return 0; // Should not happen with a 64KB address space
}

void Memory::write(uint16_t address, uint8_t value) {
    if (address >= RAM_START && address <= RAM_END) {
        // RAM access
        data[address] = value;
    } else if (address >= IO_START && address <= IO_END) {
        // I/O Soft Switches access (future implementation)
        std::cout << "Writing to I/O address: 0x" << std::hex << address << " value: 0x" << std::hex << (int)value << std::endl;
        data[address] = value; // For now, write to corresponding memory location
    } else if (address >= ROM_START && address <= ROM_END) {
        // ROM access (ROM is usually read-only, but for now, allow writes for testing)
        std::cout << "Writing to ROM address: 0x" << std::hex << address << " value: 0x" << std::hex << (int)value << std::endl;
        data[address] = value; // For now, write to corresponding memory location
    }
}

bool Memory::loadROM(const std::string& filename, uint16_t start_address) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open ROM file: " << filename << std::endl;
        return false;
    }

    uint16_t current_address = start_address;
    char byte;
    while (file.get(byte) && current_address <= ROM_END) {
        data[current_address++] = static_cast<uint8_t>(byte);
    }

    file.close();
    std::cout << "Successfully loaded ROM from " << filename << " to 0x" << std::hex << start_address << std::endl;
    return true;
}
