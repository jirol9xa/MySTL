#pragma once

#include <fmt/format.h>
#include <string>
#include <string_view>

/// Class for storring initial info about operation or class
struct LocationInfo {
  uint_fast32_t line_;
  std::string_view func_name_;
  std::string_view file_name_;

  std::string dump() const {
    // return std::format("[{}:{}:{}]", line, func_name_, file_name_); //
    // doesn't support yet((((
    return fmt::format("[{}:{}:{}]", file_name_, func_name_, line_);
  }

  constexpr uint_fast32_t getLine() const noexcept { return line_; }
  constexpr std::string_view getFunc() const noexcept { return func_name_; }
  constexpr std::string_view getFile() const noexcept { return file_name_; }
};
