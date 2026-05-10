#pragma once

#include "LinkedList.hpp"
#include "Sequence.hpp"

template <class T>
class ListSequenceBase : public Sequence<T> {
protected:
    LinkedList<T> items;

    virtual ListSequenceBase<T>* Instance() = 0;
    virtual ListSequenceBase<T>* NewEmpty() const = 0;

    ListSequenceBase<T>* AppendInternal(const T& item) {
        items.Append(item);
        return this;
    }

    ListSequenceBase<T>* PrependInternal(const T& item) {
        items.Prepend(item);
        return this;
    }

    ListSequenceBase<T>* InsertInternal(const T& item, int index) {
        items.InsertAt(item, index);
        return this;
    }

public:
    ListSequenceBase() : items() {}
    ListSequenceBase(T* data, int count) : items(data, count) {}
    explicit ListSequenceBase(const LinkedList<T>& list) : items(list) {}

    T GetFirst() const override {
        return items.GetFirst();
    }

    T GetLast() const override {
        return items.GetLast();
    }

    T Get(int index) const override {
        return items.Get(index);
    }

    int GetLength() const override {
        return items.GetLength();
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        LinkedList<T>* subList = items.GetSubList(startIndex, endIndex);
        ListSequenceBase<T>* result = NewEmpty();
        for (int i = 0; i < subList->GetLength(); ++i) {
            result->AppendInternal(subList->Get(i));
        }
        delete subList;
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
        ListSequenceBase<T>* result = Instance();
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
class MutableListSequence : public ListSequenceBase<T> {
protected:
    ListSequenceBase<T>* Instance() override {
        return this;
    }

    ListSequenceBase<T>* NewEmpty() const override {
        return new MutableListSequence<T>();
    }

public:
    MutableListSequence() : ListSequenceBase<T>() {}
    MutableListSequence(T* data, int count) : ListSequenceBase<T>(data, count) {}
    explicit MutableListSequence(const LinkedList<T>& list) : ListSequenceBase<T>(list) {}
    MutableListSequence(const MutableListSequence<T>& other) : ListSequenceBase<T>(other.items) {}

    Sequence<T>* Clone() const override {
        return new MutableListSequence<T>(*this);
    }
};

template <class T>
class ImmutableListSequence : public ListSequenceBase<T> {
protected:
    ListSequenceBase<T>* Instance() override {
        return new ImmutableListSequence<T>(*this);
    }

    ListSequenceBase<T>* NewEmpty() const override {
        return new ImmutableListSequence<T>();
    }

public:
    ImmutableListSequence() : ListSequenceBase<T>() {}
    ImmutableListSequence(T* data, int count) : ListSequenceBase<T>(data, count) {}
    explicit ImmutableListSequence(const LinkedList<T>& list) : ListSequenceBase<T>(list) {}
    ImmutableListSequence(const ImmutableListSequence<T>& other) : ListSequenceBase<T>(other.items) {}

    Sequence<T>* Clone() const override {
        return new ImmutableListSequence<T>(*this);
    }
};

template <class T>
using ListSequence = MutableListSequence<T>;
