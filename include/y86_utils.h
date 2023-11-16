#pragma once
#include "defs.h"

namespace y86Simulator {
// Remove all the leading and trailing characters which are whitespaces
// @param str: the string to trim
// @return: true if the string is not empty after trimming, false otherwise
bool trim(std::string &str);
} // namespace y86Simulator