#include "gtest/gtest.h"
#include "../cpu.hpp"
#include "../memory.hpp"

class CPUTest : public ::testing::Test {
protected:
    Memory* mem;
    CPU* cpu;

    void SetUp() override {
        mem = new Memory();
        cpu = new CPU(*mem);
    }

    void TearDown() override {
        delete cpu;
        delete mem;
    }
};

TEST_F(CPUTest, InitialState) {
    EXPECT_EQ(cpu->a, 0);
    EXPECT_EQ(cpu->x, 0);
    EXPECT_EQ(cpu->y, 0);
    EXPECT_EQ(cpu->ps, 0x20);
    EXPECT_EQ(cpu->sp, 0x01FF);
}

TEST_F(CPUTest, GH264_JMP_IND) {
    // No page-cross
    cpu->reset();
    cpu->pc = 0x300;
    uint16_t abs = cpu->pc + 3;
    uint16_t dst = abs + 2;
    mem->write(cpu->pc + 0, 0x6c); // JMP (IND)
    mem->write(cpu->pc + 1, abs & 0xff);
    mem->write(cpu->pc + 2, abs >> 8);
    mem->write(abs, dst & 0xff);
    mem->write(abs + 1, dst >> 8);

    cpu->execute(1);
    EXPECT_EQ(cpu->pc, dst);
}

TEST_F(CPUTest, GH271_ASL_DEC_INC_ABSX) {
    const uint16_t base = 0x20ff;
    const uint8_t d = 0x40;

    // ASL abs,x no page-cross
    cpu->reset();
    cpu->x = 0;
    mem->write(base + cpu->x, d);
    cpu->pc = 0x300;
    mem->write(cpu->pc + 0, 0x1e);
    mem->write(cpu->pc + 1, base & 0xff);
    mem->write(cpu->pc + 2, base >> 8);
    cpu->execute(1);
    EXPECT_EQ(mem->read(base + cpu->x), (d << 1) & 0xff);

    // ASL abs,x page-cross
    cpu->reset();
    cpu->x = 1;
    mem->write(base + cpu->x, d);
    cpu->pc = 0x300;
    mem->write(cpu->pc + 0, 0x1e);
    mem->write(cpu->pc + 1, base & 0xff);
    mem->write(cpu->pc + 2, base >> 8);
    cpu->execute(1);
    EXPECT_EQ(mem->read(base + cpu->x), (d << 1) & 0xff);

    // DEC abs,x no page-cross
    cpu->reset();
    cpu->x = 0;
    mem->write(base + cpu->x, d);
    cpu->pc = 0x300;
    mem->write(cpu->pc + 0, 0xde);
    mem->write(cpu->pc + 1, base & 0xff);
    mem->write(cpu->pc + 2, base >> 8);
    cpu->execute(1);
    EXPECT_EQ(mem->read(base + cpu->x), (d - 1) & 0xff);

    // DEC abs,x page-cross
    cpu->reset();
    cpu->x = 1;
    mem->write(base + cpu->x, d);
    cpu->pc = 0x300;
    mem->write(cpu->pc + 0, 0xde);
    mem->write(cpu->pc + 1, base & 0xff);
    mem->write(cpu->pc + 2, base >> 8);
    cpu->execute(1);
    EXPECT_EQ(mem->read(base + cpu->x), (d - 1) & 0xff);

    // INC abs,x no page-cross
    cpu->reset();
    cpu->x = 0;
    mem->write(base + cpu->x, d);
    cpu->pc = 0x300;
    mem->write(cpu->pc + 0, 0xfe);
    mem->write(cpu->pc + 1, base & 0xff);
    mem->write(cpu->pc + 2, base >> 8);
    cpu->execute(1);
    EXPECT_EQ(mem->read(base + cpu->x), (d + 1) & 0xff);

    // INC abs,x page-cross
    cpu->reset();
    cpu->x = 1;
    mem->write(base + cpu->x, d);
    cpu->pc = 0x300;
    mem->write(cpu->pc + 0, 0xfe);
    mem->write(cpu->pc + 1, base & 0xff);
    mem->write(cpu->pc + 2, base >> 8);
    cpu->execute(1);
    EXPECT_EQ(mem->read(base + cpu->x), (d + 1) & 0xff);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
