#pragma once

#include <cstdint>
#include <vector>

// ===== OPCODES =====
#define HALT_     0
#define LOAD_     1
#define STORE_    2
#define ADD_      3
#define SUB_      4
#define INPUT_    5
#define OUTPUT_   6
#define JUMP_     7
#define SKIPZERO_ 8
#define SKIPPOS_  9
#define SKIPERR_  10
#define LOADI_    11
#define STOREI_   12
#define SHIFT_    13
#define AND_      14
#define XOR_      15

// ===== ALU =====
int16_t ALU(int16_t acc_val,
            int16_t op_code,
            int16_t memory_val,
            bool &aeb_flag);

// ===== RAM =====
class RAM {
private:
    std::vector<uint16_t> memory;

public:
    RAM();

    uint16_t read(uint16_t address);
    void write(uint16_t address, uint16_t value);
};

// ===== CPU =====
class CPU {
private:
    bool run_flag;
    bool aeb;
    int16_t ACC;
    uint16_t PC;
    RAM* pmemory;

public:
    explicit CPU(RAM* rawMemoryPointer);
    bool Status();
    void Step();
};
