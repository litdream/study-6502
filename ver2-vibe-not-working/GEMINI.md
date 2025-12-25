# Apple II+ Emulator Project (C++ / Linux) - ver2-vibe

## 🎯 Project Goal
Focus on booting the Apple II+ computer and implementing robust console (keyboard and monitor) functionality.

## 🛠 Tech Stack
- **Language:** C++20 / SDL2
- **Target Hardware:** Apple II+ (48K RAM + 16K Language Card)
- **CPU:** Utilizes `ver1-vibe/cpu.hpp` and `cpu.cpp`
- **Memory:** Utilizes `ver1-vibe/memory.hpp` and `memory.cpp`

---

## 🚦 Current Status: [PHASE 1: Boot & Console]

### 🔳 Task 1: Basic System Setup
- [ ] Initialize memory, load ROMs (Monitor ROM, Apple2_Plus.rom).
- [ ] Implement initial CPU boot sequence (reset vector).
- [ ] Integrate existing CPU (from `ver1-vibe/cpu.hpp` and `cpu.cpp`).

### 🔳 Task 2: Memory Management
- [ ] Implement precise Apple II memory mapping (RAM, I/O Soft Switches, ROM).
- [ ] Handle soft switch interactions for keyboard and display.

### 🔳 Task 3: Keyboard Input
- [ ] Implement memory-mapped keyboard (C000/C010) logic.
- [ ] Integrate SDL2 keyboard event polling.
- [ ] Handle ASCII to Apple II keycode mapping (uppercase, Return key).

### 🔳 Task 4: Text Video Output
- [ ] Initialize SDL2 window and renderer.
- [ ] Integrate SDL_ttf for rendering TrueType fonts (PrintChar21.ttf, PRNumber3.ttf).
- [ ] Implement basic text screen rendering from Apple II memory ($0400-$07FF).
- [ ] Implement accurate Apple II text screen memory mapping (interleaved memory for horizontal scrolling effects).
- [ ] Implement full Apple II character set mapping, including inverse and flash.
- [ ] Implement CPU cycle-accurate timing for display updates.

---

## 📝 Apple II Specific Notes
- **Text Buffer:** Lives at `$0400`. We need to map this to an SDL2 window.
- **Reset Vector:** The CPU must start by reading addresses `$FFFC` and `$FFFD`.
- **Soft Switches:** Need a robust way to handle "Reading an address triggers a hardware change."

---

## 📖 Reference Links
- [Apple II Information Storage (Sather)](https://mirrors.apple2.org.za/ftp.apple.asimov.net/documentation/hardware/machines/Understanding%20the%20Apple%20II.pdf)
- [6502 Opcode Table](http://www.obelisk.me.uk/6502/reference.html)
