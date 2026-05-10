#pragma once

#include "Exceptions.hpp"

template <class T>
class DynamicArray {
private:
    T* data;
    int size;

    void CheckIndex(int index) const {
        if (index < 0 || index >= size) {
            throw IndexOutOfRange("DynamicArray index is out of range");
        }
    }

public:
    DynamicArray() : data(nullptr), size(0) {}

    DynamicArray(T* items, int count) : data(nullptr), size(count) {
        if (count < 0) {
            throw InvalidArgument("DynamicArray size cannot be negative");
        }
        data = count == 0 ? nullptr : new T[count];
        for (int i = 0; i < count; ++i) {
            data[i] = items[i];
        }
    }

    explicit DynamicArray(int size) : data(nullptr), size(size) {
        if (size < 0) {
            throw InvalidArgument("DynamicArray size cannot be negative");
        }
        data = size == 0 ? nullptr : new T[size]();
    }

    DynamicArray(const DynamicArray<T>& dynamicArray)
        : data(nullptr), size(dynamicArray.size) {
        data = size == 0 ? nullptr : new T[size];
        for (int i = 0; i < size; ++i) {
            data[i] = dynamicArray.data[i];
        }
    }

    DynamicArray<T>& operator=(const DynamicArray<T>& other) {
        if (this == &other) {
            return *this;
        }
        T* newData = other.size == 0 ? nullptr : new T[other.size];
        for (int i = 0; i < other.size; ++i) {
            newData[i] = other.data[i];
        }
        delete[] data;
        data = newData;
        size = other.size;
        return *this;
    }

    ~DynamicArray() {
        delete[] data;
    }

    T Get(int index) const {
        CheckIndex(index);
        return data[index];
    }

    int GetSize() const {
        return size;
    }

    void Set(int index, const T& value) {
        CheckIndex(index);
        data[index] = value;
    }

    void Resize(int newSize) {
        if (newSize < 0) {
            throw InvalidArgument("DynamicArray size cannot be negative");
        }
        T* newData = newSize == 0 ? nullptr : new T[newSize]();
        int copyCount = size < newSize ? size : newSize;
        for (int i = 0; i < copyCount; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        size = newSize;
    }

    T& operator[](int index) {
        CheckIndex(index);
        return data[index];
    }

    const T& operator[](int index) const {
        CheckIndex(index);
        return data[index];
    }
};
