# Apple II+ Emulator (ver2-vibe) - CPU and Memory Fixes

This document outlines the critical fixes applied to the `ver2-vibe` Apple II+ emulator to achieve a stable boot. The primary issues were found in the CPU instruction implementation and the initialization of zero-page memory vectors.

## CPU Instruction Fixes (`cpu.cpp`)

The initial CPU implementation in `ver1-vibe/cpu.cpp` contained several bugs that prevented the Apple II Monitor ROM from executing correctly. The following changes were made to align the CPU's behavior with a real 6502 processor:

### 1. Program Counter (`PC`) Handling

*   **Issue:** The `PC` was being incremented at the start of the `execute` function, and then again within each instruction handler when reading operands. This caused the `PC` to advance incorrectly, leading to incorrect instruction fetching and program flow.
*   **Fix:** The `PC` increment was removed from the start of the `execute` function and is now handled within the `step` function (for the opcode byte) and individual instruction handlers (for operand bytes).

### 2. `BRK` and `PHP` Instructions (Status Flags)

*   **Issue:** The `BRK` and `PHP` instructions were incorrectly setting the `B` (Break) and `U` (Unused) flags in the status register (`P`) when pushing it to the stack.
*   **Fix:** The implementation was corrected to match the 6502's behavior, where the `B` flag is set only when `BRK` is executed, and the `U` flag is always set.

### 3. `PLP` Instruction (Status Flags)

*   **Issue:** The `PLP` (Pull Processor Status) instruction was incorrectly preserving the `B` flag.
*   **Fix:** The implementation was corrected to ensure that the `B` flag is not affected by the `PLP` instruction, as per the 6502's behavior.

### 4. `JMP` Indirect Instruction (0x6C)

*   **Issue:** While the 6502's indirect `JMP` bug (where an indirect jump to `$xxFF` would incorrectly wrap to `$xx00` for the high byte) was correctly emulated, the incorrect `PC` handling described above made the instruction fail.
*   **Fix:** With the `PC` handling corrected, the `JMP` indirect instruction now functions as expected.

## Memory Initialization Fixes (`memory.cpp`)

The emulator was failing to boot because critical zero-page vectors, which the Monitor ROM uses for basic I/O, were not initialized. This caused the CPU to jump to address `0x0`, resulting in a `BRK` loop.

### Zero-Page Vectors

The following zero-page vectors were initialized in the `Memory` constructor to point to the correct routines in the Apple II+ Monitor ROM:

*   **`$36` and `$37` (COUT - Character Output):**
    *   `memory[0x36] = 0xED`
    *   `memory[0x37] = 0xFD`
    *   This points to the `COUT` routine at `$FDED`.

*   **`$38` and `$39` (KBDIN - Keyboard Input):**
    *   `memory[0x38] = 0x1B`
    *   `memory[0x39] = 0xFD`
    *   This points to the `KBDIN` routine at `$FD1B`.

These changes were essential to allow the Monitor ROM to correctly handle character output and keyboard input, enabling a successful boot to the Monitor prompt.
