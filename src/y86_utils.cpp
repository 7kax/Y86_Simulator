#include "y86_utils.h"

using namespace std;

namespace y86Simulator {
bool trim(string &str) {
    if (str.empty())
        return false;
    int start = 0, end = str.size() - 1;
    while (start <= end && isspace(str[start]))
        start++;
    while (start <= end && isspace(str[end]))
        end--;
    str = str.substr(start, end - start + 1);
    return !str.empty();
}
} // namespace y86Simulator