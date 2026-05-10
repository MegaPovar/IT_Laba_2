#pragma once

#include <stdexcept> // для std::out_of_range и std::invalid_argument
#include <string>

class IndexOutOfRange : public std::out_of_range { 
public:
    explicit IndexOutOfRange(const std::string& message)
        : std::out_of_range(message) {}
};

class InvalidArgument : public std::invalid_argument {
public:
    explicit InvalidArgument(const std::string& message)
        : std::invalid_argument(message) {}
};
