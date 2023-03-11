#pragma once

#include <cstdint>
#include <string>

/// Class for storring initial info about operation or class
struct LocationInfo {
    uint_fast32_t line;
    const char *func_name;
    const char *file_name;
    const char *var_name;

    std::string dump() const;
};
