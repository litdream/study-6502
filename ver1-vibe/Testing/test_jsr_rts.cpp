#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class JSRRTSSTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    JSRRTSSTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0200; // Start PC at 0x0200 for tests
    }
};

TEST_F(JSRRTSSTest, JSR_CorrectPCAndStack) {
    // 0x0200: JSR $1234
    mem.write(0x0200, 0x20);
    mem.write(0x0201, 0x34);
    mem.write(0x0202, 0x12);
    
    cpu.execute(0x20);

    EXPECT_EQ(cpu.PC, 0x1234);
    EXPECT_EQ(cpu.S, 0xFD); // Stack pointer decremented by 2
    
    // JSR pushes PC-1. PC was 0x0203 after fetching the operand. So 0x0202 is pushed.
    EXPECT_EQ(mem.read(0x01FF), 0x02); // High byte
    EXPECT_EQ(mem.read(0x01FE), 0x02); // Low byte
}

TEST_F(JSRRTSSTest, RTS_CorrectPCAndStack) {
    cpu.S = 0xFD;
    // Set up stack for RTS to return to 0x0202 + 1
    mem.write(0x01FF, 0x02); // High byte of return address
    mem.write(0x01FE, 0x02); // Low byte of return address
    
    mem.write(0x1234, 0x60); // RTS instruction
    cpu.PC = 0x1234;
    
    cpu.execute(0x60);

    EXPECT_EQ(cpu.PC, 0x0203);
    EXPECT_EQ(cpu.S, 0xFF); // Stack pointer incremented by 2
}

TEST_F(JSRRTSSTest, JSR_and_RTS_integration) {
    uint16_t initialPC = cpu.PC;

    // 0x0200: JSR $1234
    mem.write(0x0200, 0x20);
    mem.write(0x0201, 0x34);
    mem.write(0x0202, 0x12);

    // 0x1234: RTS
    mem.write(0x1234, 0x60);
    
    // Execute JSR
    cpu.execute(0x20);
    EXPECT_EQ(cpu.PC, 0x1234);
    
    // Execute RTS
    cpu.execute(0x60);
    EXPECT_EQ(cpu.PC, initialPC + 3);
}
