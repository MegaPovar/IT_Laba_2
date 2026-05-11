#pragma once

#include "Exceptions.hpp"

template <class T> // template для любого типа данных
class DynamicArray {
private:
    T* data; // указатель типа data -> [10][20][30]
    int size; // элементы

    void CheckIndex(int index) const { 
        if (index < 0 || index >= size) {
            throw IndexOutOfRange("DynamicArray index is out of range");
        }
    }

public:
    DynamicArray() : data(nullptr), size(0) {} // создаем пустой массив

    DynamicArray(T* items, int count) : data(nullptr), size(count) { // DynamicArray из массива
        if (count < 0) {
            throw InvalidArgument("DynamicArray size cannot be negative");
        }
        if (count == 0) {
            data = nullptr;
        } else {
            data = new T[count]; // создаем массив в памяти с размером count
        }
        for (int i = 0; i < count; ++i) {
            data[i] = items[i];
        }
    }

    explicit DynamicArray(int size) : data(nullptr), size(size) { // DynamicArray нужного размера
        if (size < 0) {
            throw InvalidArgument("DynamicArray size cannot be negative");
        }
        if (size == 0) {
            data = nullptr;
        } else {
            data = new T[size](); // память под массив, () - заполняет нулями
        }
    }

    DynamicArray(const DynamicArray<T>& dynamicArray) // конструктор копирования. новый массив на основе другого для избежания проблемы поверхностного копирования
        : data(nullptr), size(dynamicArray.size) {
        if (size == 0) {
            data = nullptr;
        } else {
            data = new T[size]; // память для копирования нового
        }
        for (int i = 0; i < size; ++i) {
            data[i] = dynamicArray.data[i];
        }
    }

    DynamicArray<T>& operator=(const DynamicArray<T>& other) { // 
        if (this == &other) { // this указатель на те]кущий объект
            return *this;
        }
        T* newData;
        if (other.size == 0) {
            newData = nullptr;
        } else {
            newData = new T[other.size];
        }
        for (int i = 0; i < other.size; ++i) { //копируем данные в только что созданный массив
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

    void Set(int index, const T& value) { // меняем элемент массива
        CheckIndex(index);
        data[index] = value;
    }

    void Resize(int newSize) {
        if (newSize < 0) {
            throw InvalidArgument("DynamicArray size cannot be negative");
        }
        T* newData; // создаем новый
        if (newSize == 0) {
            newData = nullptr;
        } else {
            newData = new T[newSize]();
        }

        int copyCount;
        if (size < newSize) {
            copyCount = size;
        } else {
            copyCount = newSize;
        }
        for (int i = 0; i < copyCount; ++i) { // копируем старое
            newData[i] = data[i];
        }
        delete[] data; // удаляем старое
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
