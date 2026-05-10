#pragma once

#include <functional>
#include <utility>

#include "Option.hpp"

template <class T>
class Sequence {
public:
    virtual ~Sequence() {}

    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(int index) const = 0;
    virtual int GetLength() const = 0;
    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const = 0;

    virtual Sequence<T>* Append(const T& item) = 0;
    virtual Sequence<T>* Prepend(const T& item) = 0;
    virtual Sequence<T>* InsertAt(const T& item, int index) = 0;
    virtual Sequence<T>* Concat(const Sequence<T>* list) = 0;
    virtual Sequence<T>* Clone() const = 0;

    virtual T& operator[](int index) = 0;
    virtual const T& operator[](int index) const = 0;

    template <class TResult>
    Sequence<TResult>* Map(std::function<TResult(T)> mapper) const;

    template <class TResult>
    Sequence<TResult>* MapIndexed(std::function<TResult(T, int)> mapper) const;

    Sequence<T>* Where(std::function<bool(T)> predicate) const;

    template <class TResult>
    TResult Reduce(std::function<TResult(TResult, T)> reducer, TResult start) const;

    template <class TResult>
    Sequence<TResult>* FlatMap(std::function<Sequence<TResult>*(T)> mapper) const;

    Option<T> TryGetFirst(std::function<bool(T)> predicate = nullptr) const;
    Option<T> TryGetLast(std::function<bool(T)> predicate = nullptr) const;
    Sequence<Sequence<T>*>* Split(std::function<bool(T)> separator) const;
    Sequence<T>* Slice(int index, int count, const Sequence<T>* inserted = nullptr) const;
};

template <class T>
Sequence<std::pair<T, T> >* Zip(const Sequence<T>* first, const Sequence<T>* second);
