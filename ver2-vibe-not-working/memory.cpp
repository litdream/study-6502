#include "memory.hpp"
#include <iostream>
#include <fstream>

Memory::Memory() : data(ADDRESS_SPACE_SIZE), text_screen_dirty(true) {
    // Initialize memory with zeros
    for (uint32_t i = 0; i < ADDRESS_SPACE_SIZE; ++i) {
        data[i] = 0;
    }

    // Set up zero-page vectors for Monitor ROM
    data[0x36] = 0xED; // COUT low byte
    data[0x37] = 0xFD; // COUT high byte
    data[0x38] = 0x1B; // KBDIN low byte
    data[0x39] = 0xFD; // KBDIN high byte
}

uint8_t Memory::read(uint16_t address) {
    // Keyboard controller
    if (address == 0xC000) {
        // Clear keyboard strobe when KBD is read
        data[0xC010] &= 0x7F;
        return data[0xC000];
    }

    if (address >= RAM_START && address <= RAM_END) {
        // RAM access
        return data[address];
    } else if (address >= IO_START && address <= IO_END) {
        // I/O Soft Switches access (future implementation)
        return data[address];
    } else if (address >= ROM_START && address <= ROM_END) {
        // ROM access
        return data[address];
    }
    return 0; // Should not happen with a 64KB address space
}

void Memory::write(uint16_t address, uint8_t value) {
    if (address >= 0x0400 && address <= 0x07FF) {
        text_screen_dirty = true;
    }

    if (address >= RAM_START && address <= RAM_END) {
        // RAM access
        data[address] = value;
    } else if (address >= IO_START && address <= IO_END) {
        // I/O Soft Switches access (future implementation)
        // std::cout << "Writing to I/O address: 0x" << std::hex << address << " value: 0x" << std::hex << (int)value << std::endl;
        data[address] = value; // For now, write to corresponding memory location
    } else if (address >= ROM_START && address <= ROM_END) {
        // ROM access (ROM is usually read-only, but for now, allow writes for testing)
        // std::cout << "Writing to ROM address: 0x" << std::hex << address << " value: 0x" << std::hex << (int)value << std::endl;
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

void Memory::keyPress(uint8_t key) {
    // Apple II ROM expects the high bit of the ASCII code to be set
    data[0xC000] = key | 0x80;
    data[0xC010] |= 0x80; // Set keyboard strobe
}