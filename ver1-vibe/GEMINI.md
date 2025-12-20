# Apple II+ Emulator Project (C++ / Linux)

## 🎯 Project Goal
Cycle-accurate 6502 emulation targeting Apple II+ hardware to run `.dsk` files via SDL2.

## 🛠 Tech Stack
- **Language:** C++20 / SDL2
- **Target Hardware:** Apple II+ (48K RAM + 16K Language Card)

---

## 🚦 Current Status: [PHASE 1: THE BUS]

### 🔳 Task 1: Apple II Memory Map & Bus
- [ ] Implement 64KB Address Space.
- [ ] Map RAM ($0000–$BFFF).
- [ ] Map I/O Soft Switches ($C000–$C0FF).
- [ ] Map ROM Space ($D000–$FFFF).
- [ ] Implement `loadROM()` to mirror the Apple II Monitor ROM into memory.

### 🔳 Task 2: CPU Core (6502)
- [ ] Define Registers (A, X, Y, PC, S, P).
- [ ] Implement Status Flag logic (N, V, U, B, D, I, Z, C).
- [ ] Create `step()` function (Fetch, Decode, Execute).

---

## 📝 Apple II Specific Notes
- **Text Buffer:** Lives at `$0400`. We need to map this to an SDL2 window.
- **Reset Vector:** The CPU must start by reading addresses `$FFFC` and `$FFFD`.
- **Soft Switches:** Need a robust way to handle "Reading an address triggers a hardware change."

---

## 📖 Reference Links
- [Apple II Information Storage (Sather)](https://mirrors.apple2.org.za/ftp.apple.asimov.net/documentation/hardware/machines/Understanding%20the%20Apple%20II.pdf)
- [6502 Opcode Table](http://www.obelisk.me.uk/6502/reference.html)
