#pragma once

#include "Exceptions.hpp"

template <class T>
class LinkedList {
private:
    struct Node {
        T value;
        Node* next;

        explicit Node(const T& value) : value(value), next(nullptr) {} // конструктор для создания узла 
    };

    Node* head;
    Node* tail;
    int length;

    void CheckIndex(int index) const {
        if (index < 0 || index >= length) {
            throw IndexOutOfRange("LinkedList index is out of range");
        }
    }

    Node* GetNode(int index) const { // возвращает указатель на узел по индексу
        CheckIndex(index);
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next; // начав с начала переходим к нужному
        }
        return current;// добавить итератор указывающий на первый элемент списка чтобы сэкономить время while movenext
    }

public:
    class Iterator {
    private:
        const Node* current;

    public:
        explicit Iterator(const Node* start) : current(start) {}

        bool HasValue() const {
            return current != nullptr;
        }

        T Get() const {
            if (current == nullptr) {
                throw IndexOutOfRange("Iterator is out of range");
            }
            return current->value;
        }

        void MoveNext() {
            if (current != nullptr) {
                current = current->next;
            }
        }
    };

    LinkedList() : head(nullptr), tail(nullptr), length(0) {} // создали пустой

    LinkedList(T* items, int count) : LinkedList() {
        if (count < 0) {
            throw InvalidArgument("LinkedList length cannot be negative");
        }
        for (int i = 0; i < count; ++i) {
            Append(items[i]); // добавили в конец
        }
    }

    LinkedList(const LinkedList<T>& list) : LinkedList() { // так же как в DinamicArray конструктор копирования 
        Iterator iterator = list.Begin();
        while (iterator.HasValue()) {
            Append(iterator.Get());
            iterator.MoveNext();
        }
    }

    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this == &other) {
            return *this;
        }
        Clear();
        Iterator iterator = other.Begin();
        while (iterator.HasValue()) {
            Append(iterator.Get());
            iterator.MoveNext();
        }
        return *this;
    }

    ~LinkedList() {
        Clear();
    }

    void Clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        length = 0;
    }

    T GetFirst() const {
        if (length == 0) {
            throw IndexOutOfRange("LinkedList is empty");
        }
        return head->value;
    }

    T GetLast() const {
        if (length == 0) {
            throw IndexOutOfRange("LinkedList is empty");
        }
        return tail->value;
    }

    T Get(int index) const {
        return GetNode(index)->value;
    }

    Iterator Begin() const { // итератор на первый элемент
        return Iterator(head);
    }

    LinkedList<T>* GetSubList(int startIndex, int endIndex) const { // создаем новый список на основе части другого списка от startIndex до endIndex включительно
        CheckIndex(startIndex);
        CheckIndex(endIndex);
        if (startIndex > endIndex) {
            throw InvalidArgument("startIndex cannot be greater than endIndex");
        }
        LinkedList<T>* result = new LinkedList<T>(); // создаем новый 
        Iterator iterator = Begin();
        for (int i = 0; i < startIndex; ++i) {
            iterator.MoveNext();
        }
        for (int i = startIndex; i <= endIndex; ++i) {
            result->Append(iterator.Get()); // скопировали нужные
            iterator.MoveNext();
        }
        return result;
    }

    int GetLength() const {
        return length;
    }

    void Append(const T& item) {
        Node* node = new Node(item); // создаем новый узел
        if (length == 0) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        ++length;
    }

    void Prepend(const T& item) {
        Node* node = new Node(item);
        node->next = head;
        head = node;
        if (length == 0) {
            tail = node;
        }
        ++length;
    }

    void InsertAt(const T& item, int index) { // вставка по индексу
        if (index < 0 || index > length) {
            throw IndexOutOfRange("LinkedList insert index is out of range");
        }
        if (index == 0) { // в начало
            Prepend(item);
            return;
        }
        if (index == length) { // в конец
            Append(item);
            return;
        }
        Node* previous = GetNode(index - 1);
        Node* node = new Node(item);
        node->next = previous->next; // переставляем ссылки относительно нового узла
        previous->next = node;
        ++length;
    }

    LinkedList<T>* Concat(const LinkedList<T>* list) const { // склеиваем два списка (list3 = list1.Concat(&list2))
        LinkedList<T>* result = new LinkedList<T>(*this); // копируем
        Iterator iterator = list->Begin();
        while (iterator.HasValue()) {
            result->Append(iterator.Get()); // добавляем новые элементы в конец
            iterator.MoveNext();
        }
        return result;
    }

    T& operator[](int index) { // n раз вызывается, убрать
        return GetNode(index)->value;
    }

    const T& operator[](int index) const {
        return GetNode(index)->value;
    }
};
