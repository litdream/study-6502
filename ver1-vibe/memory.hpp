#ifndef RAY_MEMORY_HPP
#define RAY_MEMORY_HPP

#include <vector>
#include <cstdint>

class Memory {
public:
    static constexpr uint32_t ADDRESS_SPACE_SIZE = 0x10000; // 64KB

    // Memory Regions
    static constexpr uint16_t RAM_START = 0x0000;
    static constexpr uint16_t RAM_END = 0xBFFF; // 48KB RAM + 16KB Language Card (which is part of RAM for now)
    static constexpr uint16_t IO_START = 0xC000;
    static constexpr uint16_t IO_END = 0xCFFF;
    static constexpr uint16_t ROM_START = 0xD000;
    static constexpr uint16_t ROM_END = 0xFFFF;
    std::vector<uint8_t> data;

    Memory();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
    bool loadROM(const std::string& filename, uint16_t start_address);
};

#endif // RAY_MEMORY_HPP
