#pragma once

#include <exception>

class AssertException : public std::exception {
  public:
    AssertException(const char *const message) : message_(message) {}
    const char *what() const noexcept override { return message_; }

  private:
    const char *message_;
};