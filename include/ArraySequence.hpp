#pragma once

#include "DynamicArray.hpp"
#include "Sequence.hpp"

template <class T>
class ArraySequenceBase : public Sequence<T> {
protected:
    DynamicArray<T> items;

    virtual ArraySequenceBase<T>* Instance() = 0;
    virtual ArraySequenceBase<T>* NewEmpty() const = 0;

    ArraySequenceBase<T>* AppendInternal(const T& item) {
        items.Resize(items.GetSize() + 1);
        items.Set(items.GetSize() - 1, item);
        return this;
    }

    ArraySequenceBase<T>* PrependInternal(const T& item) {
        items.Resize(items.GetSize() + 1);
        for (int i = items.GetSize() - 1; i > 0; --i) {
            items.Set(i, items.Get(i - 1));
        }
        items.Set(0, item);
        return this;
    }

    ArraySequenceBase<T>* InsertInternal(const T& item, int index) {
        if (index < 0 || index > items.GetSize()) {
            throw IndexOutOfRange("ArraySequence insert index is out of range");
        }
        items.Resize(items.GetSize() + 1);
        for (int i = items.GetSize() - 1; i > index; --i) {
            items.Set(i, items.Get(i - 1));
        }
        items.Set(index, item);
        return this;
    }

public:
    ArraySequenceBase() : items() {}
    ArraySequenceBase(T* data, int count) : items(data, count) {}
    explicit ArraySequenceBase(const DynamicArray<T>& data) : items(data) {}

    T GetFirst() const override {
        if (GetLength() == 0) {
            throw IndexOutOfRange("ArraySequence is empty");
        }
        return items.Get(0);
    }

    T GetLast() const override {
        if (GetLength() == 0) {
            throw IndexOutOfRange("ArraySequence is empty");
        }
        return items.Get(GetLength() - 1);
    }

    T Get(int index) const override {
        return items.Get(index);
    }

    int GetLength() const override {
        return items.GetSize();
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex < 0 || startIndex >= GetLength() || endIndex >= GetLength()) {
            throw IndexOutOfRange("ArraySequence subsequence index is out of range");
        }
        if (startIndex > endIndex) {
            throw InvalidArgument("startIndex cannot be greater than endIndex");
        }
        ArraySequenceBase<T>* result = NewEmpty();
        for (int i = startIndex; i <= endIndex; ++i) {
            result->AppendInternal(Get(i));
        }
        return result;
    }

    Sequence<T>* Append(const T& item) override {
        return Instance()->AppendInternal(item);
    }

    Sequence<T>* Prepend(const T& item) override {
        return Instance()->PrependInternal(item);
    }

    Sequence<T>* InsertAt(const T& item, int index) override {
        return Instance()->InsertInternal(item, index);
    }

    Sequence<T>* Concat(const Sequence<T>* list) override {
        ArraySequenceBase<T>* result = Instance();
        for (int i = 0; i < list->GetLength(); ++i) {
            result->AppendInternal(list->Get(i));
        }
        return result;
    }

    T& operator[](int index) override {
        return items[index];
    }

    const T& operator[](int index) const override {
        return items[index];
    }
};

template <class T>
class MutableArraySequence : public ArraySequenceBase<T> {
protected:
    ArraySequenceBase<T>* Instance() override {
        return this;
    }

    ArraySequenceBase<T>* NewEmpty() const override {
        return new MutableArraySequence<T>();
    }

public:
    MutableArraySequence() : ArraySequenceBase<T>() {}
    MutableArraySequence(T* data, int count) : ArraySequenceBase<T>(data, count) {}
    MutableArraySequence(const MutableArraySequence<T>& other) : ArraySequenceBase<T>(other.items) {}

    Sequence<T>* Clone() const override {
        return new MutableArraySequence<T>(*this);
    }
};

template <class T>
class ImmutableArraySequence : public ArraySequenceBase<T> {
protected:
    ArraySequenceBase<T>* Instance() override {
        return new ImmutableArraySequence<T>(*this);
    }

    ArraySequenceBase<T>* NewEmpty() const override {
        return new ImmutableArraySequence<T>();
    }

public:
    ImmutableArraySequence() : ArraySequenceBase<T>() {}
    ImmutableArraySequence(T* data, int count) : ArraySequenceBase<T>(data, count) {}
    ImmutableArraySequence(const ImmutableArraySequence<T>& other) : ArraySequenceBase<T>(other.items) {}

    Sequence<T>* Clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }
};

template <class T>
using ArraySequence = MutableArraySequence<T>;
