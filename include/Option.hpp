#pragma once

#include <stdexcept>

template <class T> // любой тип данных (int string duble...)
class Option {
private:
    bool hasValue; // true/false есть/нету значения
    T value;

public:
    Option() : hasValue(false), value() {} // создаем option без значения
    Option(const T& value) : hasValue(true), value(value) {} // с значением

    static Option<T> Some(const T& value) { // если есть значение, то возвращаем его
        return Option<T>(value);
    }

    static Option<T> None() { // если нет значения, то возвращаем пустой Option
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
