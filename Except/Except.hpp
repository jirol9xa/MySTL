#pragma once

#include "Location.hpp"
#include <string>
#include <string_view>

/// My own exception-class with extra info about reason and location of
/// exception
class Exception : public std::exception {
private:
  LocationInfo loc_;
  std::string error_msg_;
  std::string_view reason_;

public:
  Exception(const LocationInfo &loc, std::string_view reason = {})
      : loc_(loc), reason_(reason) {
    error_msg_ = "Exception in " + loc_.dump() + ": " +
                 (!reason_.empty() ? reason_.data() : "") + "\n";
  }

  Exception(const Exception &) = default;
  Exception(Exception &&) = default;
  Exception &operator=(const Exception &) = default;
  Exception &operator=(Exception &&) = default;

  Exception(size_t line, std::string_view func, std::string_view file,
            std::string_view reason = {})
      : Exception(LocationInfo{line, func, file}, reason) {}

  const char *what() const noexcept override { return error_msg_.c_str(); }

  constexpr uint_fast32_t getLine() const noexcept { return loc_.getLine(); }
  constexpr std::string_view getFunc() const noexcept { return loc_.getFunc(); }
  constexpr std::string_view getFile() const noexcept { return loc_.getFile(); }
};
