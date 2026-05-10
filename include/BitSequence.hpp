#pragma once

#include <iostream>

#include "ArraySequence.hpp"
#include "SequenceAlgorithms.hpp"

class Bit {
private:
    bool value;

public:
    Bit(bool value = false) : value(value) {}
    Bit(int value) : value(value != 0) {}

    bool Value() const {
        return value;
    }

    Bit operator&(const Bit& other) const {
        return Bit(value && other.value);
    }

    Bit operator|(const Bit& other) const {
        return Bit(value || other.value);
    }

    Bit operator^(const Bit& other) const {
        return Bit(value != other.value);
    }

    Bit operator~() const {
        return Bit(!value);
    }

    bool operator==(const Bit& other) const {
        return value == other.value;
    }
};

inline std::ostream& operator<<(std::ostream& out, const Bit& bit) {
    out << (bit.Value() ? 1 : 0);
    return out;
}

class BitSequence : public MutableArraySequence<Bit> {
private:
    void CheckSameLength(const BitSequence& other) const {
        if (GetLength() != other.GetLength()) {
            throw InvalidArgument("Bit sequences must have the same length");
        }
    }

public:
    BitSequence() : MutableArraySequence<Bit>() {}
    BitSequence(Bit* data, int count) : MutableArraySequence<Bit>(data, count) {}
    explicit BitSequence(const char* bits) : MutableArraySequence<Bit>() {
        for (int i = 0; bits[i] != '\0'; ++i) {
            if (bits[i] != '0' && bits[i] != '1') {
                throw InvalidArgument("Bit string can contain only 0 and 1");
            }
            Append(Bit(bits[i] == '1'));
        }
    }

    BitSequence And(const BitSequence& other) const {
        CheckSameLength(other);
        BitSequence result;
        for (int i = 0; i < GetLength(); ++i) {
            result.Append(Get(i) & other.Get(i));
        }
        return result;
    }

    BitSequence Or(const BitSequence& other) const {
        CheckSameLength(other);
        BitSequence result;
        for (int i = 0; i < GetLength(); ++i) {
            result.Append(Get(i) | other.Get(i));
        }
        return result;
    }

    BitSequence Xor(const BitSequence& other) const {
        CheckSameLength(other);
        BitSequence result;
        for (int i = 0; i < GetLength(); ++i) {
            result.Append(Get(i) ^ other.Get(i));
        }
        return result;
    }

    BitSequence Not() const {
        BitSequence result;
        for (int i = 0; i < GetLength(); ++i) {
            result.Append(~Get(i));
        }
        return result;
    }

    bool IsSet(int index) const {
        return Get(index).Value();
    }
};
