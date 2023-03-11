#include "LocInfo.hpp"

std::string LocationInfo::dump() const {
    return "Info about birn location:\n line = " + std::to_string(line) +
           ", func_name: " + func_name /*+ ", file_name: " + file_name*/ +
           ", var_name = " + var_name;
}
