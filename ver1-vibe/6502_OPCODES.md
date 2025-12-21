# 6502 Opcodes Implementation Checklist

This document tracks the implementation status of all 6502 instruction categories.

## Load and Store Operations
- [x] LDA (Load Accumulator)
- [x] LDX (Load X Register)
- [x] LDY (Load Y Register)
- [x] STA (Store Accumulator)
- [x] STX (Store X Register)
- [x] STY (Store Y Register)

## Register Transfers
- [x] TAX (Transfer Accumulator to X)
- [x] TAY (Transfer Accumulator to Y)
- [x] TSX (Transfer Stack Pointer to X)
- [x] TXA (Transfer X to Accumulator)
- [x] TXS (Transfer X to Stack Pointer)
- [x] TYA (Transfer Y to Accumulator)

## Stack Operations
- [x] PHA (Push Accumulator)
- [x] PHP (Push Processor Status)
- [x] PLA (Pull Accumulator)
- [x] PLP (Pull Processor Status)

## Logical
- [x] AND (Logical AND)
- [x] EOR (Exclusive OR)
- [x] ORA (Logical Inclusive OR)

## Arithmetic
- [x] ADC (Add with Carry)
- [x] SBC (Subtract with Carry)
- [x] CMP (Compare Accumulator)
- [x] CPX (Compare X Register)
- [x] CPY (Compare Y Register)

## Increments & Decrements
- [x] DEC (Decrement Memory)
- [x] DEX (Decrement X Register)
- [x] DEY (Decrement Y Register)
- [x] INC (Increment Memory)
- [x] INX (Increment X Register)
- [x] INY (Increment Y Register)

## Shifts
- [x] ASL (Arithmetic Shift Left)
- [x] LSR (Logical Shift Right)
- [x] ROL (Rotate Left)
- [x] ROR (Rotate Right)

## Jumps & Calls
- [ ] JMP (Jump)
- [ ] JSR (Jump to Subroutine)
- [ ] RTS (Return from Subroutine)

## Branches
- [ ] BCC (Branch if Carry Clear)
- [ ] BCS (Branch if Carry Set)
- [ ] BEQ (Branch if Equal)
- [ ] BMI (Branch if Minus)
- [ ] BNE (Branch if Not Equal)
- [ ] BPL (Branch if Plus)
- [ ] BVC (Branch if Overflow Clear)
- [ ] BVS (Branch if Overflow Set)

## Status Flag Changes
- [ ] CLC (Clear Carry)
- [ ] CLD (Clear Decimal)
- [ ] CLI (Clear Interrupt Disable)
- [ ] CLV (Clear Overflow)
- [ ] SEC (Set Carry)
- [ ] SED (Set Decimal)
- [ ] SEI (Set Interrupt Disable)

## System Functions
- [ ] BRK (Break)
- [ ] NOP (No Operation)
- [ ] RTI (Return from Interrupt)