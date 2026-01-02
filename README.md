# VN-CPU Emulator

A Von Neumann architecture CPU emulator with a custom assembler, written in C++. This project simulates a 16-bit processor with 4096 words of memory, featuring a complete instruction set for arithmetic, logic, control flow, and I/O operations.

![Project Status](https://img.shields.io/badge/status-active-brightgreen)
![License](https://img.shields.io/badge/license-MIT-blue)
![C++](https://img.shields.io/badge/C++-17-orange)

---

## Table of Contents

1. [Overview](#overview)
2. [Architecture](#architecture)
3. [Getting Started](#getting-started)
4. [The Assembler](#the-assembler)
5. [The Emulator](#the-emulator)
6. [Instruction Set Reference](#instruction-set-reference)
7. [Programming Guide](#programming-guide)
8. [Internals Deep Dive](#internals-deep-dive)
9. [Tips and Tricks](#tips-and-tricks)
10. [Examples](#examples)
11. [Troubleshooting](#troubleshooting)
12. [Contributing](#contributing)
13. [Roadmap](#roadmap)

---

## Overview

### What is This?

The VN-CPU Emulator is an educational project that implements a simplified Von Neumann architecture computer. It consists of two main components:

1. **Assembler** (`assembler`) - Converts human-readable assembly code into binary machine code
2. **Emulator** (`main`) - Executes the binary machine code, simulating CPU operations

### Why Von Neumann?

The Von Neumann architecture is foundational to modern computing. Key characteristics:

- **Stored Program Concept**: Instructions and data share the same memory
- **Sequential Execution**: Instructions are fetched and executed one at a time
- **Single Memory Bus**: Both instructions and data travel over the same pathway

<!-- FIGURE: Add Von Neumann architecture diagram here -->
```
┌─────────────────────────────────────────────────────────────┐
│                      VON NEUMANN ARCHITECTURE               │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│    ┌─────────────┐         ┌─────────────────────────┐     │
│    │             │         │          CPU            │     │
│    │    RAM      │◄───────►│  ┌─────┐  ┌─────────┐  │     │
│    │   (4096     │  Data   │  │ ALU │  │ Control │  │     │
│    │   words)    │   Bus   │  └─────┘  │  Unit   │  │     │
│    │             │         │     ▲     └────┬────┘  │     │
│    └─────────────┘         │     │          │       │     │
│                            │  ┌──┴──┐  ┌────┴────┐  │     │
│                            │  │ ACC │  │   PC    │  │     │
│                            │  └─────┘  └─────────┘  │     │
│                            └─────────────────────────┘     │
│                                      ▲                     │
│                                      │                     │
│                               ┌──────┴──────┐              │
│                               │   I/O Unit  │              │
│                               │ stdin/stdout│              │
│                               └─────────────┘              │
└─────────────────────────────────────────────────────────────┘
```

### Key Features

- ✅ 16-bit instruction word format
- ✅ 4096 words of addressable memory (12-bit addresses)
- ✅ 16 opcodes covering arithmetic, logic, control flow, and I/O
- ✅ Two-pass assembler with label support
- ✅ Indirect addressing modes (LOADI, STOREI)
- ✅ Overflow detection via error flag
- ✅ Interactive I/O through stdin/stdout

---

## Architecture

### Memory Layout

```
Address Range    Usage
─────────────────────────────────────
0x000 - 0xFFE   Program & Data Space
0xFFF (4095)    Reserved / Stack Top
─────────────────────────────────────
Total: 4096 words × 16 bits = 8 KB
```

<!-- FIGURE: Add memory map diagram here -->

### Instruction Format

Each instruction is a 16-bit word divided into two parts:

```
┌────────────────┬─────────────────────────────────┐
│   OPCODE (4)   │         ADDRESS (12)            │
├────────────────┼─────────────────────────────────┤
│  Bits 15-12    │          Bits 11-0              │
└────────────────┴─────────────────────────────────┘

Example: LOAD 100
Binary:  0001 0000 0110 0100
         ──── ────────────────
         0x1  0x064 (100 in decimal)
```

### Registers

| Register | Size    | Description                              |
|----------|---------|------------------------------------------|
| **PC**   | 16-bit  | Program Counter - address of next instruction |
| **ACC**  | 16-bit  | Accumulator - main working register (signed) |
| **AEB**  | 1-bit   | Arithmetic Error Bit - overflow flag     |

### Components Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                        SYSTEM COMPONENTS                        │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌──────────────────┐    ┌──────────────────────────────────┐  │
│  │    ASSEMBLER     │    │             EMULATOR             │  │
│  │                  │    │                                  │  │
│  │  ┌────────────┐  │    │  ┌─────┐  ┌─────┐  ┌─────────┐  │  │
│  │  │ First Pass │  │    │  │ RAM │◄─┤ CPU │──┤   ALU   │  │  │
│  │  │ (Labels)   │  │    │  └─────┘  └──┬──┘  └─────────┘  │  │
│  │  └─────┬──────┘  │    │              │                  │  │
│  │        ▼         │    │         ┌────┴────┐             │  │
│  │  ┌────────────┐  │    │         │ Fetch   │             │  │
│  │  │Second Pass │  │    │         │ Decode  │             │  │
│  │  │ (Codegen)  │  │    │         │ Execute │             │  │
│  │  └─────┬──────┘  │    │         └─────────┘             │  │
│  │        ▼         │    │                                  │  │
│  │   [out.txt]      │───►│         [Binary Input]          │  │
│  └──────────────────┘    └──────────────────────────────────┘  │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

## Getting Started

### Prerequisites

- **C++ Compiler**: GCC 7+, Clang 5+, or MSVC 2017+
- **Make** (optional): For build automation
- **macOS/Linux**: Primary development platforms

### Building from Source

#### Option 1: Manual Compilation

```bash
# Clone the repository
git clone https://github.com/EchidnaTheG/vn-cpu-emulator.git
cd vn-cpu-emulator

# Create build directory
mkdir -p build

# Compile the assembler
g++ -std=c++17 -o build/assembler src/assembler.cpp src/coreutils.cpp

# Compile the emulator
g++ -std=c++17 -o build/emulator src/main.cpp src/coreutils.cpp
```

#### Option 2: Using Make (Recommended)

```bash
# Create a Makefile first (see Makefile section below)
make all
```

<!-- PLACEHOLDER: Add Makefile content -->
<details>
<summary>📄 Sample Makefile</summary>

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

all: $(BUILD_DIR)/assembler $(BUILD_DIR)/emulator

$(BUILD_DIR)/assembler: $(SRC_DIR)/assembler.cpp $(SRC_DIR)/coreutils.cpp
    @mkdir -p $(BUILD_DIR)
    $(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -o $@ $^

$(BUILD_DIR)/emulator: $(SRC_DIR)/main.cpp $(SRC_DIR)/coreutils.cpp
    @mkdir -p $(BUILD_DIR)
    $(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -o $@ $^

clean:
    rm -rf $(BUILD_DIR)

.PHONY: all clean
```

</details>

### Quick Start: Your First Program

1. **Write an assembly program** (`hello.asm`):

```asm
; hello.asm - Simple addition program
INPUT 0          ; Read a number from user
STORE x          ; Store it in variable x
ADD x            ; Double it (x + x)
OUTPUT 0         ; Print the result
HALT 0           ; Stop execution

x: DATA 0        ; Variable declaration
```

2. **Assemble the program**:

```bash
./build/assembler hello.asm
# Output: Successfully assembled N instructions.
# Creates: out.txt
```

3. **Run the emulator**:

```bash
./build/emulator < out.txt
# Enter a number when prompted
# See the doubled result
```

### Project Structure

```
vn-cpu-emulator/
├── README.md                 # This file
├── include/
│   └── coreutils.h++         # Header declarations
├── src/
│   ├── main.cpp              # Emulator entry point
│   ├── assembler.cpp         # Two-pass assembler
│   └── coreutils.cpp         # CPU, RAM, ALU implementations
├── test_programs/
│   ├── test1_asm.txt         # ALU operations test
│   ├── test2_asm.txt         # Collatz conjecture
│   └── test3_asm.txt         # Indirect addressing test
├── build/                    # Compiled binaries (generated)
└── docs/                     # Additional documentation
    └── figures/              # Architecture diagrams
```

---

## The Assembler

### Overview

The assembler is a **two-pass assembler** that converts human-readable assembly language into 16-bit binary machine code.

### Two-Pass Assembly Process

<!-- FIGURE: Add assembler pipeline diagram -->

```
                    TWO-PASS ASSEMBLY
                    
    Source Code                    Binary Output
        │                               ▲
        ▼                               │
┌───────────────┐              ┌────────────────┐
│  FIRST PASS   │              │  SECOND PASS   │
│               │              │                │
│ • Scan labels │              │ • Generate     │
│ • Count       │──────────────│   opcodes      │
│   instructions│ Symbol Table │ • Resolve      │
│ • Build       │              │   addresses    │
│   symbol table│              │ • Output binary│
└───────────────┘              └────────────────┘
```

#### First Pass
- Scans the entire source file
- Identifies label definitions (lines ending with `:`)
- Counts instruction addresses
- Builds a **symbol table** mapping labels to addresses

#### Second Pass
- Generates actual machine code
- Resolves label references using the symbol table
- Outputs binary strings to `out.txt`

### Assembly Language Syntax

```asm
; This is a comment (ignored by assembler)

OPCODE operand     ; Standard instruction

label: OPCODE operand   ; Labeled instruction

variable: DATA value    ; Data declaration
```

### Syntax Rules

| Rule | Description | Example |
|------|-------------|---------|
| Comments | Start with `;`, ignored | `; This is a comment` |
| Labels | End with `:`, define addresses | `loop:` |
| Instructions | OPCODE followed by operand | `LOAD x` |
| Data | `DATA` keyword defines constants | `x: DATA 42` |
| Case | Opcodes are case-sensitive (uppercase) | `LOAD` not `load` |

### Assembler Output

The assembler produces `out.txt` containing one 16-bit binary string per line:

```
0101000000000000    ; INPUT 0
0010000000010100    ; STORE at address 20
0110000000000000    ; OUTPUT 0
0000000000000000    ; HALT 0
```

---

## The Emulator

### Overview

The emulator simulates the fetch-decode-execute cycle of a real CPU, processing binary machine code instruction by instruction.

### Execution Cycle

<!-- FIGURE: Add fetch-decode-execute cycle diagram -->

```
┌─────────────────────────────────────────────────────────────┐
│                  FETCH-DECODE-EXECUTE CYCLE                 │
└─────────────────────────────────────────────────────────────┘

     ┌──────────┐
     │  START   │
     └────┬─────┘
          │
          ▼
    ┌───────────┐     Read instruction
    │   FETCH   │◄────from RAM[PC]
    └─────┬─────┘
          │
          ▼
    ┌───────────┐     Extract opcode (bits 15-12)
    │   DECODE  │     Extract address (bits 11-0)
    └─────┬─────┘
          │
          ▼
    ┌───────────┐     Perform operation
    │  EXECUTE  │     Update PC, ACC, flags
    └─────┬─────┘
          │
          ▼
     ┌─────────┐
     │  HALT?  │──Yes──► Stop
     └────┬────┘
          │ No
          └──────────────────────┐
                                 │
          ┌──────────────────────┘
          ▼
    (Back to FETCH)
```

### Running Programs

```bash
# Basic execution
./build/emulator < out.txt

# With input redirection (for automated testing)
echo "10" | ./build/emulator < out.txt

# Piping assembler output directly (advanced)
./build/assembler program.asm && ./build/emulator < out.txt
```

### I/O Behavior

- **INPUT**: Reads a 16-bit signed integer from stdin
- **OUTPUT**: Writes the ACC value to stdout as decimal
- After reading from file, stdin is reopened to `/dev/tty` for interactive input

---

## Instruction Set Reference

### Complete Instruction Table

| Opcode | Binary | Mnemonic   | Operation                                      |
|--------|--------|------------|------------------------------------------------|
| 0      | 0000   | `HALT`     | Stop execution                                 |
| 1      | 0001   | `LOAD`     | ACC ← RAM[address]                            |
| 2      | 0010   | `STORE`    | RAM[address] ← ACC                            |
| 3      | 0011   | `ADD`      | ACC ← ACC + RAM[address]                      |
| 4      | 0100   | `SUB`      | ACC ← ACC - RAM[address]                      |
| 5      | 0101   | `INPUT`    | ACC ← read from stdin                         |
| 6      | 0110   | `OUTPUT`   | Write ACC to stdout                           |
| 7      | 0111   | `JUMP`     | PC ← address (unconditional)                  |
| 8      | 1000   | `SKIPZERO` | if ACC == 0: PC ← PC + 2                      |
| 9      | 1001   | `SKIPPOS`  | if ACC > 0: PC ← PC + 2                       |
| 10     | 1010   | `SKIPERR`  | if AEB == 1: PC ← PC + 2, AEB ← 0             |
| 11     | 1011   | `LOADI`    | ACC ← RAM[RAM[address]] (indirect load)       |
| 12     | 1100   | `STOREI`   | RAM[RAM[address]] ← ACC (indirect store)      |
| 13     | 1101   | `SHIFT`    | ACC ← ACC << n (if n≥0) or ACC >> -n (if n<0) |
| 14     | 1110   | `AND`      | ACC ← ACC & RAM[address] (bitwise AND)        |
| 15     | 1111   | `XOR`      | ACC ← ACC ^ RAM[address] (bitwise XOR)        |

### Detailed Instruction Descriptions

#### Control Flow Instructions

<details>
<summary><b>HALT</b> - Stop Execution</summary>

```asm
HALT 0
```
- Stops the CPU
- The address field is ignored (conventionally `0`)
- Sets `run_flag = false`

</details>

<details>
<summary><b>JUMP</b> - Unconditional Jump</summary>

```asm
JUMP label
JUMP 100
```
- Sets PC to the specified address
- Used for loops and unconditional branches

</details>

<details>
<summary><b>SKIPZERO</b> - Skip if ACC is Zero</summary>

```asm
SKIPZERO 0
```
- If ACC == 0: skips the next instruction (PC += 2)
- Otherwise: continues normally (PC += 1)
- Commonly used with JUMP for conditional branching

**Pattern:**
```asm
    SKIPZERO 0      ; If ACC == 0, skip the JUMP
    JUMP else_branch
    ; ... then code ...
```

</details>

<details>
<summary><b>SKIPPOS</b> - Skip if ACC is Positive</summary>

```asm
SKIPPOS 0
```
- If ACC > 0: skips the next instruction
- Used for signed comparisons

</details>

<details>
<summary><b>SKIPERR</b> - Skip if Overflow Occurred</summary>

```asm
SKIPERR 0
```
- If AEB (Arithmetic Error Bit) is set: skips next instruction
- Clears AEB after checking
- Used for overflow handling

</details>

#### Data Movement Instructions

<details>
<summary><b>LOAD</b> - Load from Memory</summary>

```asm
LOAD variable
LOAD 100
```
- ACC ← RAM[address]
- Reads 16-bit value from memory into accumulator

</details>

<details>
<summary><b>STORE</b> - Store to Memory</summary>

```asm
STORE variable
STORE 100
```
- RAM[address] ← ACC
- Writes accumulator value to memory

</details>

<details>
<summary><b>LOADI</b> - Indirect Load</summary>

```asm
LOADI pointer
```
- ACC ← RAM[RAM[address]]
- First reads address from RAM[address], then loads from that address
- Essential for array access and pointers

**Example:**
```asm
; If ptr contains 50, and RAM[50] contains 42:
LOADI ptr    ; ACC becomes 42
```

</details>

<details>
<summary><b>STOREI</b> - Indirect Store</summary>

```asm
STOREI pointer
```
- RAM[RAM[address]] ← ACC
- Stores ACC at the address pointed to by the memory location

</details>

#### Arithmetic Instructions

<details>
<summary><b>ADD</b> - Addition</summary>

```asm
ADD variable
```
- ACC ← ACC + RAM[address]
- Sets AEB on overflow (result > INT16_MAX or < INT16_MIN)

</details>

<details>
<summary><b>SUB</b> - Subtraction</summary>

```asm
SUB variable
```
- ACC ← ACC - RAM[address]
- Sets AEB on underflow

</details>

<details>
<summary><b>SHIFT</b> - Bit Shift</summary>

```asm
SHIFT amount
```
- If RAM[address] ≥ 0: ACC ← ACC << RAM[address] (left shift)
- If RAM[address] < 0: ACC ← ACC >> -RAM[address] (right shift)

**Examples:**
```asm
SHIFT two      ; Left shift by 2 (multiply by 4)
SHIFT negtwo   ; Right shift by 2 (divide by 4)

two: DATA 2
negtwo: DATA -2
```

</details>

#### Logic Instructions

<details>
<summary><b>AND</b> - Bitwise AND</summary>

```asm
AND mask
```
- ACC ← ACC & RAM[address]
- Useful for masking bits

</details>

<details>
<summary><b>XOR</b> - Bitwise XOR</summary>

```asm
XOR value
```
- ACC ← ACC ^ RAM[address]
- XOR with itself clears ACC: `XOR x` where x contains ACC's value → 0

</details>

#### I/O Instructions

<details>
<summary><b>INPUT</b> - Read from stdin</summary>

```asm
INPUT 0
```
- Reads a signed 16-bit integer from standard input
- Address field is ignored

</details>

<details>
<summary><b>OUTPUT</b> - Write to stdout</summary>

```asm
OUTPUT 0
```
- Prints ACC value to standard output as decimal
- Address field is ignored

</details>

---

## Programming Guide

### Common Patterns

#### If-Then-Else

```asm
; if (x == 0) then A else B

    LOAD x
    SKIPZERO 0      ; if x == 0, skip to then_branch
    JUMP else_branch

then_branch:
    ; ... code for x == 0 ...
    JUMP end_if

else_branch:
    ; ... code for x != 0 ...

end_if:
    ; continue...
```

#### While Loop

```asm
; while (counter > 0) { body; counter--; }

loop_start:
    LOAD counter
    SKIPPOS 0       ; if counter > 0, skip the exit jump
    JUMP loop_end
    
    ; ... loop body ...
    
    LOAD counter
    SUB one
    STORE counter
    JUMP loop_start

loop_end:
    ; continue...

counter: DATA 10
one: DATA 1
```

#### Multiplication (via repeated addition)

```asm
; result = a * b (where b > 0)

    LOAD zero
    STORE result    ; result = 0

mult_loop:
    LOAD b
    SKIPPOS 0
    JUMP mult_done
    
    LOAD result
    ADD a
    STORE result    ; result += a
    
    LOAD b
    SUB one
    STORE b         ; b--
    
    JUMP mult_loop

mult_done:
    LOAD result
    OUTPUT 0

a: DATA 5
b: DATA 3
result: DATA 0
one: DATA 1
zero: DATA 0
```

#### Array Access (using indirect addressing)

```asm
; Read array[index]

    LOAD array_base
    ADD index
    STORE ptr
    LOADI ptr       ; ACC = array[index]

array_base: DATA 100  ; Array starts at address 100
index: DATA 2
ptr: DATA 0
```

### Best Practices

1. **Always initialize variables**: Use `DATA 0` for working variables
2. **Use meaningful labels**: `loop_start` is better than `L1`
3. **Comment your code**: Assembly is cryptic; comments are essential
4. **Watch for overflow**: Use `SKIPERR` after arithmetic operations
5. **Reserve memory carefully**: Plan your memory layout

### Memory Management Tips

```
┌─────────────────────────────────────────┐
│          SUGGESTED MEMORY LAYOUT        │
├─────────────────────────────────────────┤
│  0x000 - 0x0FF  │  Program code         │
│  0x100 - 0x1FF  │  Variables            │
│  0x200 - 0x3FF  │  Arrays / buffers     │
│  0x400 - 0xFFE  │  Free space           │
│  0xFFF          │  Reserved             │
└─────────────────────────────────────────┘
```

---

## Internals Deep Dive

### RAM Class

```cpp
class RAM {
private:
    std::vector<uint16_t> memory;  // 4096 words

public:
    RAM();                                    // Initialize to zeros
    uint16_t read(uint16_t address);          // Read word
    void write(uint16_t address, uint16_t value);  // Write word
};
```

**Key Implementation Details:**
- Memory is stored as `std::vector<uint16_t>` with 4096 elements
- Bounds checking prevents out-of-range access
- All values are unsigned 16-bit for storage

### CPU Class

```cpp
class CPU {
private:
    bool run_flag = true;     // Execution continues while true
    bool aeb = false;         // Arithmetic Error Bit
    int16_t ACC = 0;          // Accumulator (signed!)
    uint16_t PC = 0;          // Program Counter
    RAM* pmemory = nullptr;   // Pointer to RAM instance

public:
    explicit CPU(RAM* rawMemoryPointer);
    bool Status();            // Returns run_flag
    void Step();              // Execute one instruction
};
```

**Key Implementation Details:**
- ACC is **signed** (`int16_t`) for arithmetic operations
- PC is **unsigned** (`uint16_t`) for addressing
- CPU holds a pointer to RAM (dependency injection pattern)

### ALU Function

```cpp
int16_t ALU(int16_t acc_val,
            int16_t op_code,
            int16_t memory_val,
            bool &aeb_flag);
```

**Supported Operations:**
- `ADD_`: Addition with overflow detection
- `SUB_`: Subtraction with underflow detection
- `AND_`: Bitwise AND
- `XOR_`: Bitwise XOR
- `SHIFT_`: Bidirectional shift based on sign

**Overflow Detection:**
```cpp
int32_t temp = acc_val + memory_val;
if (temp > INT16_MAX || temp < INT16_MIN) {
    aeb_flag = true;  // Set error flag
}
```

### Instruction Decoding

```cpp
void CPU::Step() {
    uint16_t ins = pmemory->read(PC);
    uint16_t opcode = ins >> 12;           // Top 4 bits
    uint16_t ins_address = ins & 0x0FFF;   // Bottom 12 bits
    
    switch (opcode) {
        case HALT_:
            run_flag = false;
            break;
        case LOAD_:
            ACC = pmemory->read(ins_address);
            PC += 1;
            break;
        // ... other cases
    }
}
```

<!-- FIGURE: Add bit manipulation diagram -->

```
Instruction Word:  0001 0000 0110 0100
                   ──── ────────────────
                    │         │
                    │         └── Address: 0x064 = 100
                    │
                    └── Opcode: 0x1 = LOAD

After decoding:
  opcode      = 0001 (binary) = 1 (decimal) = LOAD
  ins_address = 0000 0110 0100 (binary) = 100 (decimal)
```

### Data Flow During Execution

```
┌─────────────────────────────────────────────────────────────────┐
│                    LOAD INSTRUCTION DATA FLOW                   │
└─────────────────────────────────────────────────────────────────┘

  ┌─────┐         ┌─────────────┐         ┌─────────────┐
  │ PC  │────────►│ RAM.read(PC)│────────►│  Instruction│
  │ =5  │         │             │         │  0001...    │
  └─────┘         └─────────────┘         └──────┬──────┘
                                                  │
                                                  ▼
                                          ┌──────────────┐
                                          │    Decode    │
                                          │ opcode=LOAD  │
                                          │ addr=100     │
                                          └──────┬───────┘
                                                 │
                                                 ▼
  ┌─────┐         ┌─────────────┐         ┌──────────────┐
  │ ACC │◄────────│RAM.read(100)│◄────────│   Execute    │
  │ =42 │         │   = 42      │         │              │
  └─────┘         └─────────────┘         └──────────────┘
                                                 │
                                                 ▼
                                          ┌──────────────┐
                                          │   PC += 1    │
                                          │   PC = 6     │
                                          └──────────────┘
```

---

## Tips and Tricks

### Debugging Assembly Programs

1. **Add OUTPUT statements** liberally during development:
   ```asm
   LOAD x
   OUTPUT 0    ; Debug: see current value of x
   ```

2. **Use XOR for clearing**: Faster than loading zero:
   ```asm
   ; Clear ACC (assumes 'zero' contains 0)
   LOAD zero
   ; Or use XOR with any known value:
   LOAD x
   XOR x       ; ACC = 0
   ```

3. **Check for infinite loops**: Ensure all loops have valid exit conditions

### Performance Considerations

- **Minimize memory access**: Keep frequently used values in calculation chains
- **Use SHIFT for multiplication/division by powers of 2**:
  ```asm
  ; Multiply by 4 (shift left 2)
  SHIFT two
  
  ; Divide by 8 (shift right 3)
  SHIFT negthree
  ```

### Common Mistakes

| Mistake | Symptom | Solution |
|---------|---------|----------|
| Forgetting `HALT` | Program runs forever | Always end with `HALT 0` |
| Uninitialized variables | Garbage values | Use `DATA 0` |
| Wrong label spelling | Assembler error | Check case sensitivity |
| Infinite loops | Program hangs | Verify loop exit conditions |
| Overflow without checking | Wrong results | Use `SKIPERR` |

### Useful Code Snippets

<details>
<summary>Absolute Value</summary>

```asm
; ACC = |ACC|
    SKIPPOS 0       ; If positive, skip negation
    JUMP negate
    JUMP done_abs

negate:
    ; Negate using two's complement: -x = ~x + 1
    XOR all_ones    ; Flip all bits
    ADD one         ; Add 1

done_abs:
    ; ACC now contains |original value|

all_ones: DATA -1   ; All bits set (0xFFFF)
one: DATA 1
```

</details>

<details>
<summary>Maximum of Two Values</summary>

```asm
; result = max(a, b)
    LOAD a
    SUB b           ; a - b
    SKIPPOS 0       ; If a > b, skip
    JUMP b_is_bigger

a_is_bigger:
    LOAD a
    STORE result
    JUMP max_done

b_is_bigger:
    LOAD b
    STORE result

max_done:
    ; result contains max(a, b)
```

</details>

<details>
<summary>Check if Even/Odd</summary>

```asm
; Check if x is even or odd
    LOAD x
    AND one         ; Mask lowest bit
    SKIPZERO 0      ; If 0, it's even
    JUMP is_odd
    JUMP is_even

is_odd:
    ; ... handle odd ...
    
is_even:
    ; ... handle even ...

one: DATA 1
```

</details>

---

## Examples

### Example 1: Basic Arithmetic (test1_asm.txt)

Tests fundamental ALU operations:

```asm
INPUT 0          ; Enter 10
STORE A
OUTPUT 0         ; Echo input

; Test ADD (10 + 10 = 20)
LOAD A
ADD A
OUTPUT 0

; Test SUB (10 - 5 = 5)
LOAD A
SUB five
OUTPUT 0

; Test SHIFT (-2 means Shift Right 2 bits. 10 >> 2 = 2)
LOAD A
SHIFT negtwo
OUTPUT 0

; Test XOR (10 ^ 10 = 0)
LOAD A
XOR A
OUTPUT 0

HALT 0

A: DATA 0
five: DATA 5
negtwo: DATA -2
```

**Expected Output** (input: 10):
```
10
20
5
2
0
```

### Example 2: Collatz Conjecture (test2_asm.txt)

Implements the famous 3n+1 problem:

```asm
INPUT 0
STORE n
OUTPUT 0       ; Print starting number

loop: LOAD n
      SUB one
      SKIPZERO 0   ; If n-1 == 0, we are done
      JUMP check
      HALT 0       ; End of program

check: LOAD n
       AND one     ; Check if Odd (n & 1)
       SKIPZERO 0  ; If result is 0, it's Even
       JUMP is_odd
       JUMP is_even

is_even: LOAD n
         SHIFT negone  ; Divide by 2
         STORE n
         OUTPUT 0
         JUMP loop

is_odd:  LOAD n
         ADD n      ; n * 2
         ADD n      ; n * 3
         ADD one    ; 3n + 1
         STORE n
         OUTPUT 0
         JUMP loop

n: DATA 0
one: DATA 1
negone: DATA -1
```

**Expected Output** (input: 6):
```
6
3
10
5
16
8
4
2
1
```

### Example 3: Indirect Addressing (test3_asm.txt)

Demonstrates pointer-based memory access:

```asm
; Fill array at addresses 50, 51, 52 with values 10, 20, 30

; Write 10 to address 50
LOAD ptr
STORE ptr_temp
LOAD val1
STOREI ptr_temp

; Write 20 to address 51
LOAD ptr
ADD one
STORE ptr_temp
LOAD val2
STOREI ptr_temp

; Write 30 to address 52
LOAD ptr
ADD two
STORE ptr_temp
LOAD val3
STOREI ptr_temp

; Read back and display
LOAD ptr
STORE ptr_temp
LOADI ptr_temp
OUTPUT 0       ; Prints 10

LOAD ptr
ADD one
STORE ptr_temp
LOADI ptr_temp
OUTPUT 0       ; Prints 20

LOAD ptr
ADD two
STORE ptr_temp
LOADI ptr_temp
OUTPUT 0       ; Prints 30

HALT 0

ptr: DATA 50
ptr_temp: DATA 0
one: DATA 1
two: DATA 2
val1: DATA 10
val2: DATA 20
val3: DATA 30
```

**Expected Output**:
```
10
20
30
```

---

## Troubleshooting

### Common Errors and Solutions

#### Assembler Errors

| Error | Cause | Solution |
|-------|-------|----------|
| "Usage: ./assembler <filename>" | No input file | Provide assembly file path |
| Undefined label | Typo in label name | Check spelling and case |
| Invalid opcode | Unknown instruction | Use valid opcodes from table |

#### Runtime Errors

| Symptom | Cause | Solution |
|---------|-------|----------|
| No output | Missing `OUTPUT` or early `HALT` | Check program flow |
| Program hangs | Infinite loop or no `HALT` | Verify loop conditions |
| Wrong numbers | Overflow or logic error | Add debug outputs |
| Segmentation fault | Memory out of bounds | Check address calculations |

### Debug Checklist

- [ ] Does the program end with `HALT 0`?
- [ ] Are all labels spelled correctly?
- [ ] Are all variables initialized with `DATA`?
- [ ] Do loops have valid exit conditions?
- [ ] Is the input file being read correctly?

---

## Contributing

### How to Contribute

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/new-instruction`
3. Make your changes
4. Test thoroughly
5. Submit a pull request

### Coding Standards

- Use C++17 features
- Follow existing code style
- Add comments for complex logic
- Update documentation for new features

### Areas for Contribution

- [ ] Additional instructions (MUL, DIV, MOD)
- [ ] Interactive debugger
- [ ] Memory visualization
- [ ] Stack implementation
- [ ] Subroutine support (CALL/RET)
- [ ] Unit tests

---

## Roadmap

### Version 1.0 (Current)
- ✅ Basic CPU emulation
- ✅ Two-pass assembler
- ✅ 16 instruction set
- ✅ Indirect addressing

### Version 1.1 (Planned)
- [ ] Improved error messages in assembler
- [ ] Step-by-step debugger
- [ ] Memory dump feature
- [ ] Better I/O handling

### Version 2.0 (Future)
- [ ] Subroutine support (CALL/RET instructions)
- [ ] Hardware stack
- [ ] Interrupt handling
- [ ] Multiple source file assembly

---

## License

<!-- Add your license here -->
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## Acknowledgments

- Inspired by classic computer architecture courses
- Based on Von Neumann's original architecture concepts
- Built for educational purposes

---

## Appendix

### A. ASCII Quick Reference

<!-- PLACEHOLDER: Add ASCII table if needed -->

### B. Two's Complement Reference

| Decimal | 16-bit Binary          | Hex    |
|---------|------------------------|--------|
| 0       | 0000 0000 0000 0000   | 0x0000 |
| 1       | 0000 0000 0000 0001   | 0x0001 |
| -1      | 1111 1111 1111 1111   | 0xFFFF |
| 32767   | 0111 1111 1111 1111   | 0x7FFF |
| -32768  | 1000 0000 0000 0000   | 0x8000 |

### C. Glossary

| Term | Definition |
|------|------------|
| **ACC** | Accumulator - main CPU register for computations |
| **AEB** | Arithmetic Error Bit - overflow indicator |
| **ALU** | Arithmetic Logic Unit - performs calculations |
| **Opcode** | Operation code - identifies the instruction type |
| **PC** | Program Counter - address of current instruction |
| **Two's Complement** | Method for representing signed integers |

---

<p align="center">
  <i>Happy Emulating! 🖥️</i>
</p>