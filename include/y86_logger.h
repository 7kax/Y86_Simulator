#pragma once
#include "defs.h"
#include "fifo_map.hpp"
#include "json.hpp"
#include "y86_CPU.h"

namespace y86Simulator {

using namespace nlohmann;

// A workaround to give to use fifo_map as map, we are just ignoring the 'less'
// compare
template <class K, class V, class dummy_compare, class A>
using my_workaround_fifo_map = fifo_map<K, V, fifo_map_compare<K>, A>;
using my_json = basic_json<my_workaround_fifo_map>;

class logger {
  public:
    // log the status of the CPU
    // @param cpu: the CPU to log
    void log(const CPU &cpu);

    // print the log
    // @param out: the output stream
    void print(std::ostream &out);

  private:
    my_json output;
};
} // namespace y86Simulator