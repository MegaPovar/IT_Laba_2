#pragma once

#include <iostream>

#include "ArraySequence.hpp"
#include "SequenceAlgorithms.hpp"

class Bit { // отдельный тип для 0/1
private:
    bool value; // true = 1, false = 0

public:
    Bit(bool value = false) : value(value) {} // бит из bool
    Bit(int value) : value(value != 0) {} // бит из int

    bool Value() const { // получить значение
        return value;
    }

    Bit operator&(const Bit& other) const { // AND
        return Bit(value && other.value);
    }

    Bit operator|(const Bit& other) const { // OR
        return Bit(value || other.value);
    }

    Bit operator^(const Bit& other) const { // XOR
        return Bit(value != other.value);
    }

    Bit operator~() const { // NOT
        return Bit(!value);
    }

    bool operator==(const Bit& other) const {
        return value == other.value;
    }
};

inline std::ostream& operator<<(std::ostream& out, const Bit& bit) { // выводим Bit как 0 или 1
    out << (bit.Value() ? 1 : 0);
    return out;
}

class BitSequence : public MutableArraySequence<Bit> { // последовательность битов
private:
    void CheckSameLength(const BitSequence& other) const { // для AND/OR/XOR длины должны совпадать
        if (GetLength() != other.GetLength()) {
            throw InvalidArgument("Bit sequences must have the same length");
        }
    }

public:
    BitSequence() : MutableArraySequence<Bit>() {}
    BitSequence(Bit* data, int count) : MutableArraySequence<Bit>(data, count) {}
    explicit BitSequence(const char* bits) : MutableArraySequence<Bit>() { // создать из строки "1010"
        for (int i = 0; bits[i] != '\0'; ++i) {
            if (bits[i] != '0' && bits[i] != '1') {
                throw InvalidArgument("Bit string can contain only 0 and 1");
            }
            Append(Bit(bits[i] == '1'));
        }
    }

    BitSequence And(const BitSequence& other) const { // побитовое И
        CheckSameLength(other);
        BitSequence result;
        for (int i = 0; i < GetLength(); ++i) {
            result.Append(Get(i) & other.Get(i));
        }
        return result;
    }

    BitSequence Or(const BitSequence& other) const { // побитовое ИЛИ
        CheckSameLength(other);
        BitSequence result;
        for (int i = 0; i < GetLength(); ++i) {
            result.Append(Get(i) | other.Get(i));
        }
        return result;
    }

    BitSequence Xor(const BitSequence& other) const { // побитовое исключающее ИЛИ
        CheckSameLength(other);
        BitSequence result;
        for (int i = 0; i < GetLength(); ++i) {
            result.Append(Get(i) ^ other.Get(i));
        }
        return result;
    }

    BitSequence Not() const { // инверсия всех битов
        BitSequence result;
        for (int i = 0; i < GetLength(); ++i) {
            result.Append(~Get(i));
        }
        return result;
    }

    bool IsSet(int index) const { // проверить бит как маску
        return Get(index).Value();
    }
};
