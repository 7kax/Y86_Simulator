// This file contains the definitions used by the backend.
#pragma once

#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace y86Simulator {
// Number of registers
constexpr int NUM_REG = 16;

// Number of condition codes
constexpr int NUM_CC = 3;

// Size of the memory
constexpr int MEM_SIZE = 0x10000;

// Condition codes
constexpr int CC_ZF = 0;
constexpr int CC_SF = 1;
constexpr int CC_OF = 2;

// Registers
constexpr int REG_rax = 0;
constexpr int REG_rcx = 1;
constexpr int REG_rdx = 2;
constexpr int REG_rbx = 3;
constexpr int REG_rsp = 4;
constexpr int REG_rbp = 5;
constexpr int REG_rsi = 6;
constexpr int REG_rdi = 7;
constexpr int REG_r8 = 8;
constexpr int REG_r9 = 9;
constexpr int REG_r10 = 10;
constexpr int REG_r11 = 11;
constexpr int REG_r12 = 12;
constexpr int REG_r13 = 13;
constexpr int REG_r14 = 14;
constexpr int REG_no_reg = 15;

// Status codes
constexpr int STAT_AOK = 1;
constexpr int STAT_HLT = 2;
constexpr int STAT_ADR = 3;
constexpr int STAT_INS = 4;

// Address Type, y86-64 uses 64-bit addresses
using address_t = unsigned long long;

// Memory unit
using byte_t = unsigned char;
using memory_unit_t = long long;
struct memory_t {
    byte_t data[8];

    // Default constructor
    memory_t() = default;

    // Constructor: convert a memory_unit_t to memory_t
    memory_t(memory_unit_t value) {
        for (int i = 7; i >= 0; i--) {
            data[i] = value & 0xff;
            value >>= 8;
        }
    }

    // Convert the memory_t to memory_unit_t
    operator memory_unit_t() const {
        memory_unit_t result = 0;
        for (int i = 0; i < 8; i++) {
            result <<= 8;
            result += data[i];
        }
        return result;
    }

    // Reverse the memory_t
    void reverse() {
        for (int i = 0; i < 4; i++) {
            std::swap(data[i], data[7 - i]);
        }
    }
};
} // namespace y86Simulator