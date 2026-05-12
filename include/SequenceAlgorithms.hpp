#pragma once

#include "ArraySequence.hpp"

template <class T, class TSelf>
template <class TResult>
Sequence<TResult>* SequenceOperations<T, TSelf>::Map(std::function<TResult(T)> mapper) const { // применяем mapper к каждому элементу
    const TSelf& sequence = static_cast<const TSelf&>(*this); // CRTP: берем настоящий тип
    Sequence<TResult>* result = new MutableArraySequence<TResult>(); // результат может быть другого типа
    for (int i = 0; i < sequence.GetLength(); ++i) {
        result->Append(mapper(sequence.Get(i)));
    }
    return result;
}

template <class T, class TSelf>
template <class TResult>
Sequence<TResult>* SequenceOperations<T, TSelf>::MapIndexed(std::function<TResult(T, int)> mapper) const { // map + индекс 
    const TSelf& sequence = static_cast<const TSelf&>(*this);
    Sequence<TResult>* result = new MutableArraySequence<TResult>();
    for (int i = 0; i < sequence.GetLength(); ++i) {
        result->Append(mapper(sequence.Get(i), i));
    }
    return result;
}

template <class T, class TSelf>
Sequence<T>* SequenceOperations<T, TSelf>::Where(std::function<bool(T)> predicate) const { // фильтр по условию (потом %2 )
    const TSelf& sequence = static_cast<const TSelf&>(*this);
    Sequence<T>* result = new MutableArraySequence<T>();
    for (int i = 0; i < sequence.GetLength(); ++i) {
        T value = sequence.Get(i);
        if (predicate(value)) {
            result->Append(value);
        }
    }
    return result;
}

template <class T, class TSelf>
template <class TResult>
TResult SequenceOperations<T, TSelf>::Reduce(std::function<TResult(TResult, T)> reducer, TResult start) const { // свертка в одно значение
    const TSelf& sequence = static_cast<const TSelf&>(*this);
    TResult result = start; // начальное значение
    for (int i = 0; i < sequence.GetLength(); ++i) {
        result = reducer(result, sequence.Get(i));
    }
    return result;
}

template <class T, class TSelf>
template <class TResult>
Sequence<TResult>* SequenceOperations<T, TSelf>::FlatMap(std::function<Sequence<TResult>*(T)> mapper) const { // каждый элемент дает последовательность
    const TSelf& sequence = static_cast<const TSelf&>(*this);
    Sequence<TResult>* result = new MutableArraySequence<TResult>(); 
    for (int i = 0; i < sequence.GetLength(); ++i) {
        Sequence<TResult>* part = mapper(sequence.Get(i)); // временная часть результата
        for (int j = 0; j < part->GetLength(); ++j) {
            result->Append(part->Get(j));
        }
        delete part; // удаляем временную часть, так как она уже склеена в результат
    }
    return result;
}

template <class T, class TSelf>
Option<T> SequenceOperations<T, TSelf>::TryGetFirst(std::function<bool(T)> predicate) const { // найти первый подходящий через Option слева направо
    const TSelf& sequence = static_cast<const TSelf&>(*this);
    for (int i = 0; i < sequence.GetLength(); ++i) {
        T value = sequence.Get(i);
        if (!predicate || predicate(value)) {
            return Option<T>::Some(value);
        }
    }
    return Option<T>::None();
}

template <class T, class TSelf>
Option<T> SequenceOperations<T, TSelf>::TryGetLast(std::function<bool(T)> predicate) const { // найти последний через Option
    const TSelf& sequence = static_cast<const TSelf&>(*this);
    for (int i = sequence.GetLength() - 1; i >= 0; --i) {
        T value = sequence.Get(i);
        if (!predicate || predicate(value)) {
            return Option<T>::Some(value);
        }
    }
    return Option<T>::None();
}

template <class T, class TSelf>
Sequence<Sequence<T>*>* SequenceOperations<T, TSelf>::Split(std::function<bool(T)> separator) const { // разбить на куски
    const TSelf& sequence = static_cast<const TSelf&>(*this);
    Sequence<Sequence<T>*>* result = new MutableArraySequence<Sequence<T>*>(); // список кусков
    Sequence<T>* current = new MutableArraySequence<T>(); // текущий кусок

    for (int i = 0; i < sequence.GetLength(); ++i) {
        T value = sequence.Get(i);
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

template <class T, class TSelf>
Sequence<T>* SequenceOperations<T, TSelf>::Slice(int index, int count, const Sequence<T>* inserted) const { // удалить count элементов и вставить другие
    const TSelf& sequence = static_cast<const TSelf&>(*this);
    if (count < 0) {
        throw InvalidArgument("Slice count cannot be negative");
    }

    int length = sequence.GetLength(); // длина исходной последовательности
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
        result->Append(sequence.Get(i));
    }
    if (inserted != nullptr) { // если есть что вставить
        for (int i = 0; i < inserted->GetLength(); ++i) {
            result->Append(inserted->Get(i));
        }
    }
    for (int i = start + count; i < length; ++i) {
        result->Append(sequence.Get(i));
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
