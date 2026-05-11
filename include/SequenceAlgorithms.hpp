#pragma once

#include "ArraySequence.hpp"

template <class T>
template <class TResult>
Sequence<TResult>* Sequence<T>::Map(std::function<TResult(T)> mapper) const { // применяем mapper к каждому элементу
    Sequence<TResult>* result = new MutableArraySequence<TResult>(); // результат может быть другого типа
    for (int i = 0; i < GetLength(); ++i) {
        result->Append(mapper(Get(i)));
    }
    return result;
}

template <class T>
template <class TResult>
Sequence<TResult>* Sequence<T>::MapIndexed(std::function<TResult(T, int)> mapper) const { // map + индекс
    Sequence<TResult>* result = new MutableArraySequence<TResult>();
    for (int i = 0; i < GetLength(); ++i) {
        result->Append(mapper(Get(i), i));
    }
    return result;
}

template <class T>
Sequence<T>* Sequence<T>::Where(std::function<bool(T)> predicate) const { // фильтр по условию
    Sequence<T>* result = new MutableArraySequence<T>();
    for (int i = 0; i < GetLength(); ++i) {
        T value = Get(i);
        if (predicate(value)) {
            result->Append(value);
        }
    }
    return result;
}

template <class T>
template <class TResult>
TResult Sequence<T>::Reduce(std::function<TResult(TResult, T)> reducer, TResult start) const { // свертка в одно значение
    TResult result = start; // начальное значение
    for (int i = 0; i < GetLength(); ++i) {
        result = reducer(result, Get(i));
    }
    return result;
}

template <class T>
template <class TResult>
Sequence<TResult>* Sequence<T>::FlatMap(std::function<Sequence<TResult>*(T)> mapper) const { // каждый элемент дает последовательность
    Sequence<TResult>* result = new MutableArraySequence<TResult>();
    for (int i = 0; i < GetLength(); ++i) {
        Sequence<TResult>* part = mapper(Get(i)); // временная часть результата
        for (int j = 0; j < part->GetLength(); ++j) {
            result->Append(part->Get(j));
        }
        delete part;
    }
    return result;
}

template <class T>
Option<T> Sequence<T>::TryGetFirst(std::function<bool(T)> predicate) const { // найти первый через Option
    for (int i = 0; i < GetLength(); ++i) {
        T value = Get(i);
        if (!predicate || predicate(value)) {
            return Option<T>::Some(value);
        }
    }
    return Option<T>::None();
}

template <class T>
Option<T> Sequence<T>::TryGetLast(std::function<bool(T)> predicate) const { // найти последний через Option
    for (int i = GetLength() - 1; i >= 0; --i) {
        T value = Get(i);
        if (!predicate || predicate(value)) {
            return Option<T>::Some(value);
        }
    }
    return Option<T>::None();
}

template <class T>
Sequence<Sequence<T>*>* Sequence<T>::Split(std::function<bool(T)> separator) const { // разбить на куски
    Sequence<Sequence<T>*>* result = new MutableArraySequence<Sequence<T>*>(); // список кусков
    Sequence<T>* current = new MutableArraySequence<T>(); // текущий кусок

    for (int i = 0; i < GetLength(); ++i) {
        T value = Get(i);
        if (separator(value)) {
            result->Append(current);
            current = new MutableArraySequence<T>(); // начинаем новый кусок
        } else {
            current->Append(value);
        }
    }
    result->Append(current);
    return result;
}

template <class T>
Sequence<T>* Sequence<T>::Slice(int index, int count, const Sequence<T>* inserted) const { // удалить count элементов и вставить другие
    if (count < 0) {
        throw InvalidArgument("Slice count cannot be negative");
    }

    int length = GetLength(); // длина исходной последовательности
    int start;
    if (index < 0) {
        start = length + index; // отрицательный индекс считаем с конца
    } else {
        start = index;
    }
    if (start < 0 || start > length) {
        throw IndexOutOfRange("Slice index is out of range");
    }
    if (start + count > length) {
        throw IndexOutOfRange("Slice count is out of range");
    }

    Sequence<T>* result = new MutableArraySequence<T>(); // новая последовательность результата
    for (int i = 0; i < start; ++i) {
        result->Append(Get(i));
    }
    if (inserted != nullptr) { // если есть что вставить
        for (int i = 0; i < inserted->GetLength(); ++i) {
            result->Append(inserted->Get(i));
        }
    }
    for (int i = start + count; i < length; ++i) {
        result->Append(Get(i));
    }
    return result;
}

template <class T>
Sequence<std::pair<T, T> >* Zip(const Sequence<T>* first, const Sequence<T>* second) { // соединить две последовательности парами
    int length;
    if (first->GetLength() < second->GetLength()) {
        length = first->GetLength();
    } else {
        length = second->GetLength();
    }
    Sequence<std::pair<T, T> >* result = new MutableArraySequence<std::pair<T, T> >(); // pair = пара значений
    for (int i = 0; i < length; ++i) {
        result->Append(std::make_pair(first->Get(i), second->Get(i)));
    }
    return result;
}
