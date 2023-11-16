#pragma once
#include "defs.h"
#include "y86_memory.h"

namespace y86Simulator {

class logger;

class CPU {
  public:
    // Constructor
    CPU() : PC(0), CC({true, false, false}), STAT(STAT_AOK) {
        this->RF.fill(0);
    }

    // Run the CPU
    void run();

    // Initialize the CPU with the input file and the logger
    // @param inputFile: the input file
    // @param log: the logger
    void initialize(std::istream &inputFile, logger *log) {
        this->MEM.initialize(inputFile);
        this->log = log;
    }

    friend class logger;

  private:
    // Fetch the instruction
    void fetch();

    // Decode the instruction
    void decode();

    // Execute the instruction
    void execute();

    // Memory operation
    void memory_access();

    // Write back
    void write_back();

    // Update the PC
    void PC_update();

    // Set the condition codes
    void set_CC();

    // Set the condition
    void set_Cnd();

    // Perform the operation
    memory_t operate();

    // Validate address
    bool validate_address(address_t address);

    // Status of the CPU
    address_t PC;                          // Program Counter
    std::array<memory_unit_t, NUM_REG> RF; // Register File
    std::array<bool, NUM_CC> CC;           // Condition Codes
    int STAT;                              // Status
    bool Cnd;                              // Condition
    memory MEM;                            // Memory

    // Temporary variables
    unsigned char icode, ifun, rA, rB;
    memory_unit_t valA, valB, valP, valC, valE, valM;

    // Logger
    logger *log;
};
} // namespace y86Simulator