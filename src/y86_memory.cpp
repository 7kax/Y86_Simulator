#include "y86_memory.h"
#include "y86_utils.h"

using namespace std;

namespace y86Simulator {

memory_unit_t memory::read(address_t address) {
    address_t alignedAddress = address / 8 * 8;
    int offset = address % 8;
    if (MEM.find(alignedAddress) == MEM.end())
        throw runtime_error("Invalid memory address\n");
    if (offset == 0)
        return static_cast<memory_unit_t>(MEM[alignedAddress]);
    if (MEM.find(alignedAddress + 8) == MEM.end())
        throw runtime_error("Invalid memory address\n");
    memory_t left_raw = MEM[alignedAddress];
    memory_t right_raw = MEM[alignedAddress + 8];
    left_raw.reverse();
    right_raw.reverse();
    memory_t ret;
    for (int i = 0; i < 8; i++) {
        if (offset + i < 8)
            ret.data[i] = left_raw.data[offset + i];
        else
            ret.data[i] = right_raw.data[offset + i - 8];
    }
    ret.reverse();
    return static_cast<memory_unit_t>(ret);
}

byte_t memory::readByte(address_t address) {
    address_t alignedAddress = address / 8 * 8;
    int offset = address % 8;
    if (MEM.find(alignedAddress) == MEM.end())
        throw runtime_error("Invalid memory address\n");
    memory_t raw = MEM[alignedAddress];
    return raw.data[7 - offset];
}

void memory::write(address_t address, memory_unit_t value) {
    // Address alignment
    if (address % 8 != 0)
        throw runtime_error("Unaligned memory address\n");
    MEM[address] = static_cast<memory_t>(value);
    // MEM[address].reverse();
}

void memory::writeByte(address_t address, byte_t value) {
    address_t alignedAddress = address / 8 * 8;
    int offset = address % 8;
    MEM[alignedAddress].data[7 - offset] = value;
}

void memory::initialize(istream &inputFile) {
    vector<byte_t> bytes;
    string line;
    while (getline(inputFile, line)) {
        // Remove the characters after the '|', including the '|'
        line = line.substr(0, line.find('|'));
        // Trim the leading and trailing spaces
        if (!trim(line))
            continue;
        // If the line ends with ':', there is no data in this line
        if (line.back() == ':')
            continue;
        // The number before the ": " is the address
        address_t address = stoull(line.substr(0, line.find(":")), 0, 16);
        // Resize the bytes vector to fit the address
        bytes.resize(address);
        // Get the data after the ": "
        line = line.substr(line.find(": ") + 2);
        // Split the data into bytes
        for (int i = 0; i < line.size(); i += 2) {
            string byte = line.substr(i, 2);
            bytes.push_back(stoi(byte, 0, 16));
        }
    }
    // Align the memory size to 8 bytes
    bytes.resize((bytes.size() + 7) / 8 * 8);
    // Push the bytes into the memory
    for (int i = 0; i < bytes.size(); i += 8) {
        memory_t raw;
        for (int j = 0; j < 8; j++)
            raw.data[j] = bytes[i + j];
        raw.reverse();
        MEM[i] = raw;
    }
}
} // namespace y86Simulator