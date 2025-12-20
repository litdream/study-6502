# Apple II+ Emulator Project (C++ / Linux)

## 🎯 Project Goal
Cycle-accurate 6502 emulation targeting Apple II+ hardware to run `.dsk` files via SDL2.

## 🛠 Tech Stack
- **Language:** C++20 / SDL2
- **Target Hardware:** Apple II+ (48K RAM + 16K Language Card)

---

## 🚦 Current Status: [PHASE 1: THE BUS]

### ✅ Task 1: Apple II Memory Map & Bus
- [x] Implement 64KB Address Space.
- [x] Map RAM ($0000–$BFFF).
- [x] Map I/O Soft Switches ($C000–$C0FF).
- [x] Map ROM Space ($D000–$FFFF).
- [x] Implement `loadROM()` to mirror the Apple II Monitor ROM into memory.

### ✅ Task 2: CPU Core (6502)
- [x] Define Registers (A, X, Y, PC, S, P).
- [x] Implement Status Flag logic (N, V, U, B, D, I, Z, C).
- [x] Create `step()` function (Fetch, Decode, Execute).

### 🔳 Task 3: Implement 6502 Opcodes (Load/Store Focus)
- [ ] Implement Load Accumulator (LDA) opcodes.
- [ ] Implement Store Accumulator (STA) opcodes.
- [ ] Implement Load X Register (LDX) opcodes.
- [ ] Implement Store X Register (STX) opcodes.
- [ ] Implement Load Y Register (LDY) opcodes.
- [ ] Implement Store Y Register (STY) opcodes.
- [ ] Track progress in [6502_OPCODES.md](./6502_OPCODES.md).

---

## 📝 Apple II Specific Notes
- **Text Buffer:** Lives at `$0400`. We need to map this to an SDL2 window.
- **Reset Vector:** The CPU must start by reading addresses `$FFFC` and `$FFFD`.
- **Soft Switches:** Need a robust way to handle "Reading an address triggers a hardware change."

---

## 📖 Reference Links
- [Apple II Information Storage (Sather)](https://mirrors.apple2.org.za/ftp.apple.asimov.net/documentation/hardware/machines/Understanding%20the%20Apple%20II.pdf)
- [6502 Opcode Table](http://www.obelisk.me.uk/6502/reference.html)
