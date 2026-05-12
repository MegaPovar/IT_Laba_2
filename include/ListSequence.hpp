#pragma once

#include "LinkedList.hpp"
#include "Sequence.hpp"

template <class T>
class ListSequenceBase : public Sequence<T> { // базовый класс для list sequence
protected:
    LinkedList<T> items; // внутри храним LinkedList

    virtual ListSequenceBase<T>* Instance() = 0; // this или копия для immutable
    virtual ListSequenceBase<T>* NewEmpty() const = 0; // пустая последовательность нужного типа

    ListSequenceBase<T>* AppendInternal(const T& item) { // добавление в конец списка
        items.Append(item);
        return this;
    }

    ListSequenceBase<T>* PrependInternal(const T& item) { // добавление в начало списка
        items.Prepend(item);
        return this;
    }

    ListSequenceBase<T>* InsertInternal(const T& item, int index) { // вставка через LinkedList
        items.InsertAt(item, index);
        return this;
    }

public:
    ListSequenceBase() : items() {} // пустой список
    ListSequenceBase(T* data, int count) : items(data, count) {} // из обычного массива
    explicit ListSequenceBase(const LinkedList<T>& list) : items(list) {} // из готового LinkedList

    T GetFirst() const override { // первый элемент
        return items.GetFirst();
    }

    T GetLast() const override { // последний элемент
        return items.GetLast();
    }

    T Get(int index) const override { // получить по индексу
        return items.Get(index);
    }

    int GetLength() const override { // длина списка
        return items.GetLength();
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override { // кусок последовательности
        if (startIndex < 0 || endIndex < 0 || startIndex >= GetLength() || endIndex >= GetLength()) {
            throw IndexOutOfRange("ListSequence subsequence index is out of range");
        }
        if (startIndex > endIndex) {
            throw InvalidArgument("startIndex cannot be greater than endIndex");
        }

        ListSequenceBase<T>* result = NewEmpty(); // результат нужного типа
        typename LinkedList<T>::Iterator iterator = items.Begin(); // идем по списку без лишнего subList
        for (int i = 0; i < startIndex; ++i) {
            iterator.MoveNext();
        }
        for (int i = startIndex; i <= endIndex; ++i) {
            result->AppendInternal(iterator.Get());
            iterator.MoveNext();
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
class MutableListSequence : public ListSequenceBase<T> { // изменяемая версия
protected:
    ListSequenceBase<T>* Instance() override {
        return this; // работаем с текущим объектом
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
class ImmutableListSequence : public ListSequenceBase<T> { // неизменяемая версия
protected:
    ListSequenceBase<T>* Instance() override {
        return new ImmutableListSequence<T>(*this); // возвращаем копию
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
