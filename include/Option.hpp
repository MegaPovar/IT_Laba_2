#pragma once

#include <stdexcept>

template <class T> 
class Option {
private:
    bool hasValue; // true/false есть/нету значения
    T value;

public:
    Option() : hasValue(false), value() {}
    Option(const T& value) : hasValue(true), value(value) {}

    static Option<T> Some(const T& value) {
        return Option<T>(value);
    }

    static Option<T> None() {
        return Option<T>();
    }

    bool HasValue() const {
        return hasValue;
    }

    T Value() const {
        if (!hasValue) {
            throw std::logic_error("Option has no value");
        }
        return value;
    }
};
