#pragma once

#include "ArraySequence.hpp"

template <class T>
template <class TResult>
Sequence<TResult>* Sequence<T>::Map(std::function<TResult(T)> mapper) const {
    Sequence<TResult>* result = new MutableArraySequence<TResult>();
    for (int i = 0; i < GetLength(); ++i) {
        result->Append(mapper(Get(i)));
    }
    return result;
}

template <class T>
template <class TResult>
Sequence<TResult>* Sequence<T>::MapIndexed(std::function<TResult(T, int)> mapper) const {
    Sequence<TResult>* result = new MutableArraySequence<TResult>();
    for (int i = 0; i < GetLength(); ++i) {
        result->Append(mapper(Get(i), i));
    }
    return result;
}

template <class T>
Sequence<T>* Sequence<T>::Where(std::function<bool(T)> predicate) const {
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
TResult Sequence<T>::Reduce(std::function<TResult(TResult, T)> reducer, TResult start) const {
    TResult result = start;
    for (int i = 0; i < GetLength(); ++i) {
        result = reducer(result, Get(i));
    }
    return result;
}

template <class T>
template <class TResult>
Sequence<TResult>* Sequence<T>::FlatMap(std::function<Sequence<TResult>*(T)> mapper) const {
    Sequence<TResult>* result = new MutableArraySequence<TResult>();
    for (int i = 0; i < GetLength(); ++i) {
        Sequence<TResult>* part = mapper(Get(i));
        for (int j = 0; j < part->GetLength(); ++j) {
            result->Append(part->Get(j));
        }
        delete part;
    }
    return result;
}

template <class T>
Option<T> Sequence<T>::TryGetFirst(std::function<bool(T)> predicate) const {
    for (int i = 0; i < GetLength(); ++i) {
        T value = Get(i);
        if (!predicate || predicate(value)) {
            return Option<T>::Some(value);
        }
    }
    return Option<T>::None();
}

template <class T>
Option<T> Sequence<T>::TryGetLast(std::function<bool(T)> predicate) const {
    for (int i = GetLength() - 1; i >= 0; --i) {
        T value = Get(i);
        if (!predicate || predicate(value)) {
            return Option<T>::Some(value);
        }
    }
    return Option<T>::None();
}

template <class T>
Sequence<Sequence<T>*>* Sequence<T>::Split(std::function<bool(T)> separator) const {
    Sequence<Sequence<T>*>* result = new MutableArraySequence<Sequence<T>*>();
    Sequence<T>* current = new MutableArraySequence<T>();

    for (int i = 0; i < GetLength(); ++i) {
        T value = Get(i);
        if (separator(value)) {
            result->Append(current);
            current = new MutableArraySequence<T>();
        } else {
            current->Append(value);
        }
    }
    result->Append(current);
    return result;
}

template <class T>
Sequence<T>* Sequence<T>::Slice(int index, int count, const Sequence<T>* inserted) const {
    if (count < 0) {
        throw InvalidArgument("Slice count cannot be negative");
    }

    int length = GetLength();
    int start = index < 0 ? length + index : index;
    if (start < 0 || start > length) {
        throw IndexOutOfRange("Slice index is out of range");
    }
    if (start + count > length) {
        throw IndexOutOfRange("Slice count is out of range");
    }

    Sequence<T>* result = new MutableArraySequence<T>();
    for (int i = 0; i < start; ++i) {
        result->Append(Get(i));
    }
    if (inserted != nullptr) {
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
Sequence<std::pair<T, T> >* Zip(const Sequence<T>* first, const Sequence<T>* second) {
    int length = first->GetLength() < second->GetLength() ? first->GetLength() : second->GetLength();
    Sequence<std::pair<T, T> >* result = new MutableArraySequence<std::pair<T, T> >();
    for (int i = 0; i < length; ++i) {
        result->Append(std::make_pair(first->Get(i), second->Get(i)));
    }
    return result;
}
