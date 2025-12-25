#pragma once

#include <cstdint>
#include "memory.hpp"

class CPU {
public:
    CPU(Memory& mem);
    void reset();
    void execute(uint32_t cycles);

    // 6502 Registers
    uint8_t a;   // accumulator
    uint8_t x;   // index X
    uint8_t y;   // index Y
    uint8_t ps;  // processor status
    uint16_t pc;  // program counter
    uint16_t sp;  // stack pointer

    // 6502 Processor Status flags
    enum {
        AF_SIGN = 0x80,
        AF_OVERFLOW = 0x40,
        AF_RESERVED = 0x20,
        AF_BREAK = 0x10,
        AF_DECIMAL = 0x08,
        AF_INTERRUPT = 0x04,
        AF_ZERO = 0x02,
        AF_CARRY = 0x01
    };

private:
    Memory& memory;

    uint8_t fetch();
    void push(uint8_t value);
    uint8_t pop();

    // Addressing modes
    uint16_t addr_abs();
    uint16_t addr_abs_x();
    uint16_t addr_abs_y();
    uint16_t addr_imm();
    uint16_t addr_ind();
    uint16_t addr_x_ind();
    uint16_t addr_ind_y();
    uint16_t addr_zpg();
    uint16_t addr_zpg_x();
    uint16_t addr_zpg_y();

    void branch(bool condition);

    // Instructions
    void adc(uint16_t addr);
    void and_op(uint16_t addr);
    void asl_op(uint16_t addr);
    void asl_acc();
    void bcc(uint16_t addr);
    void bcs(uint16_t addr);
    void beq(uint16_t addr);
    void bit(uint16_t addr);
    void bmi(uint16_t addr);
    void bne(uint16_t addr);
    void bpl(uint16_t addr);
    void brk();
    void bvc(uint16_t addr);
    void bvs(uint16_t addr);
    void clc();
    void cld();
    void cli();
    void clv();
    void cmp(uint16_t addr);
    void cpx(uint16_t addr);
    void cpy(uint16_t addr);
    void dec(uint16_t addr);
    void dex();
    void dey();
    void eor(uint16_t addr);
    void inc(uint16_t addr);
    void inx();
    void iny();
    void jmp(uint16_t addr);
    void jsr(uint16_t addr);
    void lda(uint16_t addr);
    void ldx(uint16_t addr);
    void ldy(uint16_t addr);
    void lsr_op(uint16_t addr);
    void lsr_acc();
    void nop();
    void ora(uint16_t addr);
    void pha();
    void php();
    void pla();
    void plp();
    void rol_op(uint16_t addr);
    void rol_acc();
    void ror_op(uint16_t addr);
    void ror_acc();
    void rti();
    void rts();
    void sbc(uint16_t addr);
    void sec();
    void sed();
    void sei();
    void sta(uint16_t addr);
    void stx(uint16_t addr);
    void sty(uint16_t addr);
    void tax();
    void tay();
    void tsx();
    void txa();
    void txs();
    void tya();
};
