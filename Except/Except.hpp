#pragma once

#include <new>
#include <cstdint>

/// My own exception-class with extra info about reason and location of exxception
class Exception : public std::exception {
    private:
        int_fast32_t line

    public:

    const char *what() const noexcept override; 
};

const char *Exception::what() const noexcept {}
