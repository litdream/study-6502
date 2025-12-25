# Apple II+ Emulator Project (C++ / Linux) - ver3-vibe

## 🎯 Project Goal
Create a stable Apple II+ emulator by replacing the potentially inaccurate 6502 CPU from `ver1-vibe` with the proven, functional CPU logic extracted from the `AppleWin` emulator. The primary objective is to achieve a successful boot of the Apple II+ ROMs.

## 🛠 Tech Stack
- **Language:** C++20 / SDL2
- **Target Hardware:** Apple II+ (48K RAM + 16K Language Card)
- **CPU Source:** Adapted from `AppleWin/source/CPU.cpp`
- **Memory:** Utilizes `ver1-vibe/memory.hpp` and `memory.cpp`

---

## 🚦 Current Status: [PHASE 1: CPU Replacement & Integration]

### ✅ Task 1: Project Scaffolding
- [x] Created new `ver3-vibe` directory.
- [x] Copied essential components (memory, ROMs, fonts, `CMakeLists.txt`) from `ver1-vibe`.

### ✅ Task 2: CPU Extraction from AppleWin
- [x] Analyzed `AppleWin/source/CPU.h` and `AppleWin/source/CPU.cpp` to understand its structure.
- [x] Extracted core 6502 instruction logic from `AppleWin/source/CPU/cpu_instructions.inl`.
- [x] Created a new `CPU` class in `ver3-vibe/cpu.hpp` and `ver3-vibe/cpu.cpp` to encapsulate the `AppleWin` CPU logic.

### ✅ Task 3: Integration
- [x] Created `ver3-vibe/main.cpp` by adapting `ver1-vibe/main.cpp`.
- [x] Integrated the new `CPU` class with the existing `Memory` module and SDL2 frontend.
- [x] Removed redundant initialization code now handled by the `CPU` constructor.

### ✅ Task 4: Build System Configuration
- [x] Updated `ver3-vibe/CMakeLists.txt` to reflect the new project structure.
- [x] Removed obsolete unit testing framework from the build configuration.
- [x] Added commands to copy necessary ROM files to the build directory to resolve runtime errors.

### ✅ Task 5: Compilation
- [x] Successfully compiled the `ver3-vibe` emulator. The project is ready for testing.
