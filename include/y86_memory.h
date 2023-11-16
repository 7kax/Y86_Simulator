#pragma once
#include "defs.h"

namespace y86Simulator {
class memory {
  public:
    // Read 8 bytes from memory
    // @param address: the address to read from, no alignment requirement
    // @return: the bytes read
    memory_unit_t read(address_t address);

    // Read 1 byte from memory
    // @param address: the address to read from, no alignment requirement
    // @return: the byte read
    byte_t readByte(address_t address);

    // Write 8 bytes to memory
    // @param address: the address to write to, must be aligned
    // @param value: the value to write
    void write(address_t address, memory_unit_t value);

    // Write 1 byte to memory
    // @param address: the address to write to, no alignment requirement
    // @param value: the value to write
    void writeByte(address_t address, byte_t value);

    // Initialize the memory with the input file
    // @param inputFile: the input file
    void initialize(std::istream &inputFile);

    friend class logger;

  private:
    std::map<address_t, memory_t> MEM;
};
} // namespace y86Simulator