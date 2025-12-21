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
- [ ] TAX (Transfer Accumulator to X)
- [ ] TAY (Transfer Accumulator to Y)
- [ ] TSX (Transfer Stack Pointer to X)
- [ ] TXA (Transfer X to Accumulator)
- [ ] TXS (Transfer X to Stack Pointer)
- [ ] TYA (Transfer Y to Accumulator)

## Stack Operations
- [ ] PHA (Push Accumulator)
- [ ] PHP (Push Processor Status)
- [ ] PLA (Pull Accumulator)
- [ ] PLP (Pull Processor Status)

## Logical
- [ ] AND (Logical AND)
- [ ] EOR (Exclusive OR)
- [ ] ORA (Logical Inclusive OR)

## Arithmetic
- [ ] ADC (Add with Carry)
- [ ] SBC (Subtract with Carry)
- [ ] CMP (Compare Accumulator)
- [ ] CPX (Compare X Register)
- [ ] CPY (Compare Y Register)

## Increments & Decrements
- [ ] DEC (Decrement Memory)
- [ ] DEX (Decrement X Register)
- [ ] DEY (Decrement Y Register)
- [ ] INC (Increment Memory)
- [ ] INX (Increment X Register)
- [ ] INY (Increment Y Register)

## Shifts
- [ ] ASL (Arithmetic Shift Left)
- [ ] LSR (Logical Shift Right)
- [ ] ROL (Rotate Left)
- [ ] ROR (Rotate Right)

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