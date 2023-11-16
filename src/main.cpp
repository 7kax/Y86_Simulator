#include "y86_CPU.h"
#include "y86_logger.h"

using namespace std;

int main() {
    y86Simulator::CPU cpu;
    y86Simulator::logger logger;
    cpu.initialize(cin, &logger);
    try {
        cpu.run();
    } catch (runtime_error &e) {
        cerr << e.what();
    }
    logger.print(cout);
    return 0;
}