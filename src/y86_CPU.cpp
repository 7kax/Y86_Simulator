#include "y86_CPU.h"
#include "y86_logger.h"
#include "y86_utils.h"

using namespace std;

namespace y86Simulator {
void CPU::run() {
    while (true) {
        this->fetch();
        if (this->STAT != STAT_AOK)
            break;
        this->decode();
        this->execute();
        if (this->STAT != STAT_AOK)
            break;
        this->memory_access();
        this->write_back();
        if (this->STAT != STAT_AOK)
            break;
        this->PC_update();
        this->log->log(*this);
    }
    this->log->log(*this);
}

void CPU::fetch() {
    byte_t temp_byte = this->MEM.readByte(this->PC);
    // The icode is the first 4 bits
    this->icode = temp_byte >> 4;
    // The ifun is the last next 4 bits
    this->ifun = temp_byte & 0xf;
    switch (this->icode) {
    case 0:
    case 1:
    case 9:
        this->valP = this->PC + 1;
        break;
    case 2:
    case 6:
    case 10:
    case 11:
        temp_byte = this->MEM.readByte(this->PC + 1);
        this->rA = (temp_byte >> 4) & 0xf;
        this->rB = temp_byte & 0xf;
        this->valP = this->PC + 2;
        break;
    case 3:
    case 4:
    case 5:
    case 12:
        temp_byte = this->MEM.readByte(this->PC + 1);
        this->rA = (temp_byte >> 4) & 0xf;
        this->rB = temp_byte & 0xf;
        this->valC = this->MEM.read(this->PC + 2);
        this->valP = this->PC + 10;
        break;
    case 7:
    case 8:
        this->valC = this->MEM.read(this->PC + 1);
        this->valP = this->PC + 9;
        break;
    default:
        this->STAT = STAT_INS;
        break;
    }
}

void CPU::decode() {
    switch (this->icode) {
    case 0:
    case 1:
    case 3:
    case 7:
        break;
    case 2:
    case 4:
    case 5:
    case 6:
    case 12:
        this->valA = this->RF[this->rA];
        this->valB = this->RF[this->rB];
        break;
    case 8:
    case 9:
    case 11:
        this->valA = this->RF[REG_rsp];
        this->valB = this->RF[REG_rsp];
        break;
    case 10:
        this->valA = this->RF[this->rA];
        this->valB = this->RF[REG_rsp];
        break;
    default:
        this->STAT = STAT_INS;
        break;
    }
}

void CPU::execute() {
    switch (this->icode) {
    case 0:
        this->STAT = STAT_HLT;
        break;
    case 1:
        break;
    case 2:
    case 7:
        this->valE = this->valA;
        this->set_Cnd();
        break;
    case 3:
        this->valE = this->valC;
        break;
    case 4:
    case 5:
        this->valE = this->valB + this->valC;
        break;
    case 6:
        this->valE = this->operate();
        this->set_CC();
        break;
    case 8:
    case 10:
        this->valE = this->valB - 8;
        break;
    case 9:
    case 11:
        this->valE = this->valB + 8;
        break;
    case 12:
        this->valE = this->valB + this->valC;
        this->set_CC();
        break;
    default:
        this->STAT = STAT_INS;
        break;
    }
}

void CPU::memory_access() {
    switch (this->icode) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 6:
    case 7:
    case 12:
        break;
    case 4:
    case 10:
        if (this->validate_address(this->valE))
            this->MEM.write(this->valE, this->valA);
        else
            this->STAT = STAT_ADR;
        break;
    case 5:
        if (this->validate_address(this->valE))
            this->valM = this->MEM.read(this->valE);
        else
            this->STAT = STAT_ADR;
        break;
    case 8:
        if (this->validate_address(this->valE))
            this->MEM.write(this->valE, this->valP);
        else
            this->STAT = STAT_ADR;
        break;
    case 9:
    case 11:
        if (this->validate_address(this->valA))
            this->valM = this->MEM.read(this->valA);
        else
            this->STAT = STAT_ADR;
        break;
    default:
        this->STAT = STAT_INS;
        break;
    }
}

void CPU::write_back() {
    switch (this->icode) {
    case 0:
    case 1:
    case 4:
    case 7:
        break;
    case 2:
        this->RF[this->rB] = this->Cnd ? this->valE : this->valB;
        break;
    case 3:
    case 6:
    case 12:
        this->RF[this->rB] = this->valE;
        break;
    case 5:
        this->RF[this->rA] = this->valM;
        break;
    case 8:
    case 9:
    case 10:
        this->RF[REG_rsp] = this->valE;
        break;
    case 11:
        this->RF[REG_rsp] = this->valE;
        this->RF[this->rA] = this->valM;
        break;
    default:
        this->STAT = STAT_INS;
        break;
    }
}

void CPU::PC_update() {
    switch (this->icode) {
    case 0:
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 10:
    case 11:
    case 12:
        this->PC = this->valP;
        break;
    case 7:
        this->PC = this->Cnd ? this->valC : this->valP;
        break;
    case 8:
        this->PC = this->valC;
        break;
    case 9:
        this->PC = this->valM;
        break;
    default:
        this->STAT = STAT_INS;
        break;
    }
}

void CPU::set_CC() {
    this->CC[CC_ZF] = (this->valE == 0);
    this->CC[CC_SF] = (this->valE < 0);
    switch (this->ifun) {
    case 0:
        this->CC[CC_OF] = ((this->valA < 0) == (this->valB < 0)) &&
                          ((this->valE < 0) != (this->valA < 0));
        break;
    case 1:
        this->CC[CC_OF] = ((this->valA < 0) != (this->valB < 0)) &&
                          ((this->valE < 0) == (this->valA < 0));
        break;
    case 2:
    case 3:
        this->CC[CC_OF] = false;
        break;
    default:
        this->STAT = STAT_INS;
        break;
    }
}

void CPU::set_Cnd() {
    switch (this->ifun) {
    case 0:
        this->Cnd = true;
        break;
    case 1:
        this->Cnd = (this->CC[CC_SF] ^ this->CC[CC_OF]) | this->CC[CC_ZF];
        break;
    case 2:
        this->Cnd = this->CC[CC_SF] ^ this->CC[CC_OF];
        break;
    case 3:
        this->Cnd = this->CC[CC_ZF];
        break;
    case 4:
        this->Cnd = !this->CC[CC_ZF];
        break;
    case 5:
        this->Cnd = !(this->CC[CC_SF] ^ this->CC[CC_OF]);
        break;
    case 6:
        this->Cnd = !((this->CC[CC_SF] ^ this->CC[CC_OF]) | this->CC[CC_ZF]);
        break;
    default:
        this->STAT = STAT_INS;
        break;
    }
}

memory_t CPU::operate() {
    switch (this->ifun) {
    case 0:
        return this->valA + this->valB;
    case 1:
        return this->valB - this->valA;
    case 2:
        return this->valA & this->valB;
    case 3:
        return this->valA ^ this->valB;
    default:
        this->STAT = STAT_INS;
        break;
    }
    return 0;
}

bool CPU::validate_address(address_t address) {
    return address >= 0 && address < MEM_SIZE;
}

} // namespace y86Simulator