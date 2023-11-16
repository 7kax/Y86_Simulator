#include "y86_logger.h"

using namespace std;

namespace y86Simulator {
void logger::log(const CPU &cpu) {
    my_json dest;
    // set PC
    dest["PC"] = cpu.PC;
    // set REG
    dest["REG"]["rax"] = cpu.RF[REG_rax];
    dest["REG"]["rcx"] = cpu.RF[REG_rcx];
    dest["REG"]["rdx"] = cpu.RF[REG_rdx];
    dest["REG"]["rbx"] = cpu.RF[REG_rbx];
    dest["REG"]["rsp"] = cpu.RF[REG_rsp];
    dest["REG"]["rbp"] = cpu.RF[REG_rbp];
    dest["REG"]["rsi"] = cpu.RF[REG_rsi];
    dest["REG"]["rdi"] = cpu.RF[REG_rdi];
    dest["REG"]["r8"] = cpu.RF[REG_r8];
    dest["REG"]["r9"] = cpu.RF[REG_r9];
    dest["REG"]["r10"] = cpu.RF[REG_r10];
    dest["REG"]["r11"] = cpu.RF[REG_r11];
    dest["REG"]["r12"] = cpu.RF[REG_r12];
    dest["REG"]["r13"] = cpu.RF[REG_r13];
    dest["REG"]["r14"] = cpu.RF[REG_r14];
    // set MEM
    for (auto &i : cpu.MEM.MEM) {
        if (i.second != 0)
            dest["MEM"][to_string(i.first)] =
                static_cast<memory_unit_t>(i.second);
    }
    // set CC
    dest["CC"]["ZF"] = (int)cpu.CC[CC_ZF];
    dest["CC"]["SF"] = (int)cpu.CC[CC_SF];
    dest["CC"]["OF"] = (int)cpu.CC[CC_OF];
    // set STAT
    dest["STAT"] = cpu.STAT;
    // add to info
    this->output.push_back(dest);
}

void logger::print(std::ostream &out) { out << this->output.dump(4) << endl; }
} // namespace y86Simulator