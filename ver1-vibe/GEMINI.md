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

### ✅ Task 3: Implement 6502 Opcodes (All Categories)
- [x] Implement Load Accumulator (LDA) opcodes.
- [x] Implement Store Accumulator (STA) opcodes.
- [x] Implement Load X Register (LDX) opcodes.
- [x] Implement Store X Register (STX) opcodes.
- [x] Implement Load Y Register (LDY) opcodes.
- [x] Implement Store Y Register (STY) opcodes.
- [x] Implement Register Transfer opcodes.
- [x] Implement Stack Operations opcodes.
- [x] Implement Logical opcodes.
- [x] Implement Arithmetic opcodes.
- [x] Implement Increment & Decrement opcodes.
- [x] Implement Shift opcodes.
- [x] Implement Jump & Call opcodes.
- [x] Implement Branch opcodes.
- [x] Implement Status Flag Change opcodes.
- [x] Implement System Functions opcodes.
- [x] All 6502 opcodes implemented and tested.
- [x] Track progress in [6502_OPCODES.md](./6502_OPCODES.md).

---

## 🚦 Current Status: [PHASE 2: I/O and Display]

### ✅ Task 1: Keyboard Input
- [x] Implement memory-mapped keyboard (C000/C010) logic.
- [x] Integrate SDL2 keyboard event polling.
- [x] Handle ASCII to Apple II keycode mapping (uppercase, Return key).

### ✅ Task 2: Basic Video Output
- [x] Initialize SDL2 window and renderer.
- [x] Integrate SDL_ttf for rendering TrueType fonts.
- [x] Load `PrintChar21.ttf` for text rendering.
- [x] Implement basic text screen rendering from Apple II memory ($0400-$07FF).
- [x] Implement CPU cycle-accurate timing with `emulateCycles`.

### 🔳 Task 3: Accurate Apple II Text Rendering
- [ ] Implement precise Apple II text screen memory mapping (interleaved memory for horizontal scrolling effects).
- [ ] Implement full Apple II character set mapping, including inverse and flash.

---

## 📝 Apple II Specific Notes
- **Text Buffer:** Lives at `$0400`. We need to map this to an SDL2 window.
- **Reset Vector:** The CPU must start by reading addresses `$FFFC` and `$FFFD`.
- **Soft Switches:** Need a robust way to handle "Reading an address triggers a hardware change."

---

## 📖 Reference Links
- [Apple II Information Storage (Sather)](https://mirrors.apple2.org.za/ftp.apple.asimov.net/documentation/hardware/machines/Understanding%20the%20Apple%20II.pdf)
- [6502 Opcode Table](http://www.obelisk.me.uk/6502/reference.html)
