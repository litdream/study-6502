#include "gtest/gtest.h"
#include "cpu.hpp"
#include "memory.hpp"

class ADCTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    ADCTest() : cpu(mem) {}

    void SetUp() override {
        cpu.reset();
        cpu.PC = 0x0000;
    }
};

TEST_F(ADCTest, ADC_IMM_SimpleAddition) {
    cpu.A = 0x10;
    mem.write(0x0000, 0x69); // ADC #$15
    mem.write(0x0001, 0x15);
    cpu.execute(0x69);
    EXPECT_EQ(cpu.A, 0x25);
    EXPECT_FALSE(cpu.getFlag(CPU::C));
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
    EXPECT_FALSE(cpu.getFlag(CPU::V));
}

TEST_F(ADCTest, ADC_IMM_WithCarryIn) {
    cpu.A = 0x10;
    cpu.setFlag(CPU::C);
    mem.write(0x0000, 0x69); // ADC #$15
    mem.write(0x0001, 0x15);
    cpu.execute(0x69);
    EXPECT_EQ(cpu.A, 0x26);
    EXPECT_FALSE(cpu.getFlag(CPU::C));
}

TEST_F(ADCTest, ADC_IMM_CarryOut) {
    cpu.A = 0xFF;
    mem.write(0x0000, 0x69); // ADC #$01
    mem.write(0x0001, 0x01);
    cpu.execute(0x69);
    EXPECT_EQ(cpu.A, 0x00);
    EXPECT_TRUE(cpu.getFlag(CPU::C));
    EXPECT_TRUE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
    EXPECT_FALSE(cpu.getFlag(CPU::V));
}

TEST_F(ADCTest, ADC_IMM_OverflowPositive) {
    cpu.A = 0x7F; // 127
    mem.write(0x0000, 0x69); // ADC #$01
    mem.write(0x0001, 0x01); // 1
    cpu.execute(0x69);
    EXPECT_EQ(cpu.A, 0x80); // -128
    EXPECT_FALSE(cpu.getFlag(CPU::C));
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_TRUE(cpu.getFlag(CPU::N));
    EXPECT_TRUE(cpu.getFlag(CPU::V));
}

TEST_F(ADCTest, ADC_IMM_OverflowNegative) {
    cpu.A = 0x80; // -128
    mem.write(0x0000, 0x69); // ADC #$FF
    mem.write(0x0001, 0xFF); // -1
    cpu.execute(0x69);
    EXPECT_EQ(cpu.A, 0x7F); // 127
    EXPECT_TRUE(cpu.getFlag(CPU::C));
    EXPECT_FALSE(cpu.getFlag(CPU::Z));
    EXPECT_FALSE(cpu.getFlag(CPU::N));
    EXPECT_TRUE(cpu.getFlag(CPU::V));
}

TEST_F(ADCTest, ADC_ZP) {
    cpu.A = 0x10;
    mem.write(0x0020, 0x15);
    mem.write(0x0000, 0x65); // ADC $20
    mem.write(0x0001, 0x20);
    cpu.execute(0x65);
    EXPECT_EQ(cpu.A, 0x25);
}

TEST_F(ADCTest, ADC_ABS) {
    cpu.A = 0x10;
    mem.write(0x1234, 0x15);
    mem.write(0x0000, 0x6D); // ADC $1234
    mem.write(0x0001, 0x34);
    mem.write(0x0002, 0x12);
    cpu.execute(0x6D);
    EXPECT_EQ(cpu.A, 0x25);
}
