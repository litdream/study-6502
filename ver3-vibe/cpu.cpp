#include "cpu.hpp"
#include <iostream>

#define SET_FLAG(flag, value) (ps = (ps & ~(flag)) | ((value) ? (flag) : 0))
#define GET_FLAG(flag) ((ps & (flag)) != 0)

#define SETNZ(val) \
    SET_FLAG(AF_ZERO, (val) == 0); \
    SET_FLAG(AF_SIGN, ((val) & 0x80) != 0);

CPU::CPU(Memory& mem) : memory(mem) {
    reset();
}

void CPU::reset() {
    a = 0;
    x = 0;
    y = 0;
    ps = 0x20;
    sp = 0x01FF;

    uint8_t lo = memory.read(0xFFFC);
    uint8_t hi = memory.read(0xFFFD);
    pc = (hi << 8) | lo;
}

uint8_t CPU::fetch() {
    return memory.read(pc++);
}

void CPU::push(uint8_t value) {
    memory.write(0x0100 | (sp & 0xFF), value);
    sp--;
}

uint8_t CPU::pop() {
    sp++;
    return memory.read(0x0100 | (sp & 0xFF));
}

// --- Addressing Modes ---

uint16_t CPU::addr_abs() {
    uint8_t lo = fetch();
    uint8_t hi = fetch();
    return (hi << 8) | lo;
}

uint16_t CPU::addr_abs_x() {
    uint8_t lo = fetch();
    uint8_t hi = fetch();
    return ((hi << 8) | lo) + x;
}

uint16_t CPU::addr_abs_y() {
    uint8_t lo = fetch();
    uint8_t hi = fetch();
    return ((hi << 8) | lo) + y;
}

uint16_t CPU::addr_imm() {
    return pc++;
}

uint16_t CPU::addr_ind() {
    uint8_t lo = fetch();
    uint8_t hi = fetch();
    uint16_t addr = (hi << 8) | lo;
    lo = memory.read(addr);
    hi = memory.read(addr + 1);
    return (hi << 8) | lo;
}

uint16_t CPU::addr_x_ind() {
    uint8_t zpg_addr = fetch() + x;
    uint8_t lo = memory.read(zpg_addr);
    uint8_t hi = memory.read(zpg_addr + 1);
    return (hi << 8) | lo;
}

uint16_t CPU::addr_ind_y() {
    uint8_t zpg_addr = fetch();
    uint8_t lo = memory.read(zpg_addr);
    uint8_t hi = memory.read(zpg_addr + 1);
    return ((hi << 8) | lo) + y;
}

uint16_t CPU::addr_zpg() {
    return fetch();
}

uint16_t CPU::addr_zpg_x() {
    return (fetch() + x) & 0xFF;
}

uint16_t CPU::addr_zpg_y() {
    return (fetch() + y) & 0xFF;
}

// --- Instructions ---

void CPU::adc(uint16_t addr) {
    uint8_t val = memory.read(addr);
    uint16_t result = a + val + GET_FLAG(AF_CARRY);
    SET_FLAG(AF_CARRY, result > 0xFF);
    SET_FLAG(AF_OVERFLOW, (~(a ^ val) & (a ^ result) & 0x80) != 0);
    a = result & 0xFF;
    SETNZ(a);
}

void CPU::and_op(uint16_t addr) {
    a &= memory.read(addr);
    SETNZ(a);
}

void CPU::asl_op(uint16_t addr) {
    uint8_t val = memory.read(addr);
    SET_FLAG(AF_CARRY, (val & 0x80) != 0);
    val <<= 1;
    memory.write(addr, val);
    SETNZ(val);
}

void CPU::asl_acc() {
    SET_FLAG(AF_CARRY, (a & 0x80) != 0);
    a <<= 1;
    SETNZ(a);
}

void CPU::branch(bool condition) {
    int8_t offset = fetch();
    if (condition) {
        pc += offset;
    }
}

void CPU::bit(uint16_t addr) {
    uint8_t val = memory.read(addr);
    SET_FLAG(AF_ZERO, (a & val) == 0);
    SET_FLAG(AF_SIGN, (val & 0x80) != 0);
    SET_FLAG(AF_OVERFLOW, (val & 0x40) != 0);
}

void CPU::brk() {
    pc++;
    push(pc >> 8);
    push(pc & 0xFF);
    push(ps | AF_BREAK);
    SET_FLAG(AF_INTERRUPT, true);
    uint8_t lo = memory.read(0xFFFE);
    uint8_t hi = memory.read(0xFFFF);
    pc = (hi << 8) | lo;
}

void CPU::clc() { SET_FLAG(AF_CARRY, false); }
void CPU::cld() { SET_FLAG(AF_DECIMAL, false); }
void CPU::cli() { SET_FLAG(AF_INTERRUPT, false); }
void CPU::clv() { SET_FLAG(AF_OVERFLOW, false); }

void CPU::cmp(uint16_t addr) {
    uint8_t val = memory.read(addr);
    uint8_t result = a - val;
    SET_FLAG(AF_CARRY, a >= val);
    SETNZ(result);
}

void CPU::cpx(uint16_t addr) {
    uint8_t val = memory.read(addr);
    uint8_t result = x - val;
    SET_FLAG(AF_CARRY, x >= val);
    SETNZ(result);
}

void CPU::cpy(uint16_t addr) {
    uint8_t val = memory.read(addr);
    uint8_t result = y - val;
    SET_FLAG(AF_CARRY, y >= val);
    SETNZ(result);
}

void CPU::dec(uint16_t addr) {
    uint8_t val = memory.read(addr) - 1;
    memory.write(addr, val);
    SETNZ(val);
}

void CPU::dex() { x--; SETNZ(x); }
void CPU::dey() { y--; SETNZ(y); }

void CPU::eor(uint16_t addr) {
    a ^= memory.read(addr);
    SETNZ(a);
}

void CPU::inc(uint16_t addr) {
    uint8_t val = memory.read(addr) + 1;
    memory.write(addr, val);
    SETNZ(val);
}

void CPU::inx() { x++; SETNZ(x); }
void CPU::iny() { y++; SETNZ(y); }

void CPU::jmp(uint16_t addr) {
    pc = addr;
}

void CPU::jsr(uint16_t addr) {
    pc--;
    push(pc >> 8);
    push(pc & 0xFF);
    pc = addr;
}

void CPU::lda(uint16_t addr) {
    a = memory.read(addr);
    SETNZ(a);
}

void CPU::ldx(uint16_t addr) {
    x = memory.read(addr);
    SETNZ(x);
}

void CPU::ldy(uint16_t addr) {
    y = memory.read(addr);
    SETNZ(y);
}

void CPU::lsr_op(uint16_t addr) {
    uint8_t val = memory.read(addr);
    SET_FLAG(AF_CARRY, (val & 1) != 0);
    val >>= 1;
    memory.write(addr, val);
    SETNZ(val);
}

void CPU::lsr_acc() {
    SET_FLAG(AF_CARRY, (a & 1) != 0);
    a >>= 1;
    SETNZ(a);
}

void CPU::nop() {}

void CPU::ora(uint16_t addr) {
    a |= memory.read(addr);
    SETNZ(a);
}

void CPU::pha() { push(a); }
void CPU::php() { push(ps | AF_BREAK); }
void CPU::pla() { a = pop(); SETNZ(a); }
void CPU::plp() { ps = (pop() & ~AF_BREAK) | AF_RESERVED; }

void CPU::rol_op(uint16_t addr) {
    uint8_t val = memory.read(addr);
    uint8_t carry = GET_FLAG(AF_CARRY);
    SET_FLAG(AF_CARRY, (val & 0x80) != 0);
    val = (val << 1) | carry;
    memory.write(addr, val);
    SETNZ(val);
}

void CPU::rol_acc() {
    uint8_t carry = GET_FLAG(AF_CARRY);
    SET_FLAG(AF_CARRY, (a & 0x80) != 0);
    a = (a << 1) | carry;
    SETNZ(a);
}

void CPU::ror_op(uint16_t addr) {
    uint8_t val = memory.read(addr);
    uint8_t carry = GET_FLAG(AF_CARRY);
    SET_FLAG(AF_CARRY, (val & 1) != 0);
    val = (val >> 1) | (carry ? 0x80 : 0);
    memory.write(addr, val);
    SETNZ(val);
}

void CPU::ror_acc() {
    uint8_t carry = GET_FLAG(AF_CARRY);
    SET_FLAG(AF_CARRY, (a & 1) != 0);
    a = (a >> 1) | (carry ? 0x80 : 0);
    SETNZ(a);
}

void CPU::rti() {
    ps = (pop() & ~AF_BREAK) | AF_RESERVED;
    uint8_t lo = pop();
    uint8_t hi = pop();
    pc = (hi << 8) | lo;
}

void CPU::rts() {
    uint8_t lo = pop();
    uint8_t hi = pop();
    pc = ((hi << 8) | lo) + 1;
}

void CPU::sbc(uint16_t addr) {
    uint8_t val = memory.read(addr) ^ 0xFF;
    uint16_t result = a + val + GET_FLAG(AF_CARRY);
    SET_FLAG(AF_CARRY, result > 0xFF);
    SET_FLAG(AF_OVERFLOW, (~(a ^ val) & (a ^ result) & 0x80) != 0);
    a = result & 0xFF;
    SETNZ(a);
}

void CPU::sec() { SET_FLAG(AF_CARRY, true); }
void CPU::sed() { SET_FLAG(AF_DECIMAL, true); }
void CPU::sei() { SET_FLAG(AF_INTERRUPT, true); }

void CPU::sta(uint16_t addr) { memory.write(addr, a); }
void CPU::stx(uint16_t addr) { memory.write(addr, x); }
void CPU::sty(uint16_t addr) { memory.write(addr, y); }

void CPU::tax() { x = a; SETNZ(x); }
void CPU::tay() { y = a; SETNZ(y); }
void CPU::tsx() { x = sp & 0xFF; SETNZ(x); }
void CPU::txa() { a = x; SETNZ(a); }
void CPU::txs() { sp = 0x0100 | x; }
void CPU::tya() { a = y; SETNZ(a); }


void CPU::execute(uint32_t cycles) {
    uint32_t cycles_to_execute = cycles;
    while (cycles_to_execute > 0) {
        uint8_t opcode = fetch();
        switch (opcode) {
            case 0x69: adc(addr_imm()); break;
            case 0x65: adc(addr_zpg()); break;
            case 0x75: adc(addr_zpg_x()); break;
            case 0x6D: adc(addr_abs()); break;
            case 0x7D: adc(addr_abs_x()); break;
            case 0x79: adc(addr_abs_y()); break;
            case 0x61: adc(addr_x_ind()); break;
            case 0x71: adc(addr_ind_y()); break;

            case 0x29: and_op(addr_imm()); break;
            case 0x25: and_op(addr_zpg()); break;
            case 0x35: and_op(addr_zpg_x()); break;
            case 0x2D: and_op(addr_abs()); break;
            case 0x3D: and_op(addr_abs_x()); break;
            case 0x39: and_op(addr_abs_y()); break;
            case 0x21: and_op(addr_x_ind()); break;
            case 0x31: and_op(addr_ind_y()); break;

            case 0x0A: asl_acc(); break;
            case 0x06: asl_op(addr_zpg()); break;
            case 0x16: asl_op(addr_zpg_x()); break;
            case 0x0E: asl_op(addr_abs()); break;
            case 0x1E: asl_op(addr_abs_x()); break;

            case 0x90: branch(!GET_FLAG(AF_CARRY)); break;
            case 0xB0: branch(GET_FLAG(AF_CARRY)); break;
            case 0xF0: branch(GET_FLAG(AF_ZERO)); break;
            case 0xD0: branch(!GET_FLAG(AF_ZERO)); break;
            case 0x30: branch(GET_FLAG(AF_SIGN)); break;
            case 0x10: branch(!GET_FLAG(AF_SIGN)); break;
            case 0x50: branch(!GET_FLAG(AF_OVERFLOW)); break;
            case 0x70: branch(GET_FLAG(AF_OVERFLOW)); break;

            case 0x24: bit(addr_zpg()); break;
            case 0x2C: bit(addr_abs()); break;

            case 0x00: brk(); break;

            case 0x18: clc(); break;
            case 0xD8: cld(); break;
            case 0x58: cli(); break;
            case 0xB8: clv(); break;

            case 0xC9: cmp(addr_imm()); break;
            case 0xC5: cmp(addr_zpg()); break;
            case 0xD5: cmp(addr_zpg_x()); break;
            case 0xCD: cmp(addr_abs()); break;
            case 0xDD: cmp(addr_abs_x()); break;
            case 0xD9: cmp(addr_abs_y()); break;
            case 0xC1: cmp(addr_x_ind()); break;
            case 0xD1: cmp(addr_ind_y()); break;

            case 0xE0: cpx(addr_imm()); break;
            case 0xE4: cpx(addr_zpg()); break;
            case 0xEC: cpx(addr_abs()); break;

            case 0xC0: cpy(addr_imm()); break;
            case 0xC4: cpy(addr_zpg()); break;
            case 0xCC: cpy(addr_abs()); break;

            case 0xC6: dec(addr_zpg()); break;
            case 0xD6: dec(addr_zpg_x()); break;
            case 0xCE: dec(addr_abs()); break;
            case 0xDE: dec(addr_abs_x()); break;
            case 0xCA: dex(); break;
            case 0x88: dey(); break;

            case 0x49: eor(addr_imm()); break;
            case 0x45: eor(addr_zpg()); break;
            case 0x55: eor(addr_zpg_x()); break;
            case 0x4D: eor(addr_abs()); break;
            case 0x5D: eor(addr_abs_x()); break;
            case 0x59: eor(addr_abs_y()); break;
            case 0x41: eor(addr_x_ind()); break;
            case 0x51: eor(addr_ind_y()); break;

            case 0xE6: inc(addr_zpg()); break;
            case 0xF6: inc(addr_zpg_x()); break;
            case 0xEE: inc(addr_abs()); break;
            case 0xFE: inc(addr_abs_x()); break;
            case 0xE8: inx(); break;
            case 0xC8: iny(); break;

            case 0x4C: jmp(addr_abs()); break;
            case 0x6C: jmp(addr_ind()); break;

            case 0x20: jsr(addr_abs()); break;

            case 0xA9: lda(addr_imm()); break;
            case 0xA5: lda(addr_zpg()); break;
            case 0xB5: lda(addr_zpg_x()); break;
            case 0xAD: lda(addr_abs()); break;
            case 0xBD: lda(addr_abs_x()); break;
            case 0xB9: lda(addr_abs_y()); break;
            case 0xA1: lda(addr_x_ind()); break;
            case 0xB1: lda(addr_ind_y()); break;

            case 0xA2: ldx(addr_imm()); break;
            case 0xA6: ldx(addr_zpg()); break;
            case 0xB6: ldx(addr_zpg_y()); break;
            case 0xAE: ldx(addr_abs()); break;
            case 0xBE: ldx(addr_abs_y()); break;

            case 0xA0: ldy(addr_imm()); break;
            case 0xA4: ldy(addr_zpg()); break;
            case 0xB4: ldy(addr_zpg_x()); break;
            case 0xAC: ldy(addr_abs()); break;
            case 0xBC: ldy(addr_abs_x()); break;

            case 0x4A: lsr_acc(); break;
            case 0x46: lsr_op(addr_zpg()); break;
            case 0x56: lsr_op(addr_zpg_x()); break;
            case 0x4E: lsr_op(addr_abs()); break;
            case 0x5E: lsr_op(addr_abs_x()); break;

            case 0xEA: nop(); break;

            case 0x09: ora(addr_imm()); break;
            case 0x05: ora(addr_zpg()); break;
            case 0x15: ora(addr_zpg_x()); break;
            case 0x0D: ora(addr_abs()); break;
            case 0x1D: ora(addr_abs_x()); break;
            case 0x19: ora(addr_abs_y()); break;
            case 0x01: ora(addr_x_ind()); break;
            case 0x11: ora(addr_ind_y()); break;

            case 0x48: pha(); break;
            case 0x08: php(); break;
            case 0x68: pla(); break;
            case 0x28: plp(); break;

            case 0x2A: rol_acc(); break;
            case 0x26: rol_op(addr_zpg()); break;
            case 0x36: rol_op(addr_zpg_x()); break;
            case 0x2E: rol_op(addr_abs()); break;
            case 0x3E: rol_op(addr_abs_x()); break;

            case 0x6A: ror_acc(); break;
            case 0x66: ror_op(addr_zpg()); break;
            case 0x76: ror_op(addr_zpg_x()); break;
            case 0x6E: ror_op(addr_abs()); break;
            case 0x7E: ror_op(addr_abs_x()); break;

            case 0x40: rti(); break;
            case 0x60: rts(); break;

            case 0xE9: sbc(addr_imm()); break;
            case 0xE5: sbc(addr_zpg()); break;
            case 0xF5: sbc(addr_zpg_x()); break;
            case 0xED: sbc(addr_abs()); break;
            case 0xFD: sbc(addr_abs_x()); break;
            case 0xF9: sbc(addr_abs_y()); break;
            case 0xE1: sbc(addr_x_ind()); break;
            case 0xF1: sbc(addr_ind_y()); break;

            case 0x38: sec(); break;
            case 0xF8: sed(); break;
            case 0x78: sei(); break;

            case 0x85: sta(addr_zpg()); break;
            case 0x95: sta(addr_zpg_x()); break;
            case 0x8D: sta(addr_abs()); break;
            case 0x9D: sta(addr_abs_x()); break;
            case 0x99: sta(addr_abs_y()); break;
            case 0x81: sta(addr_x_ind()); break;
            case 0x91: sta(addr_ind_y()); break;

            case 0x86: stx(addr_zpg()); break;
            case 0x96: stx(addr_zpg_y()); break;
            case 0x8E: stx(addr_abs()); break;

            case 0x84: sty(addr_zpg()); break;
            case 0x94: sty(addr_zpg_x()); break;
            case 0x8C: sty(addr_abs()); break;

            case 0xAA: tax(); break;
            case 0xA8: tay(); break;
            case 0xBA: tsx(); break;
            case 0x8A: txa(); break;
            case 0x9A: txs(); break;
            case 0x98: tya(); break;

            default:
                // std::cout << "Unknown opcode: " << std::hex << (int)opcode << std::endl;
                break;
        }
        cycles_to_execute--;
    }
}
