#include <chrono> // бенчмарки
#include <iostream>
#include <limits> // для std::numeric_limits
#include <sstream>
#include <string>

#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "SequenceAlgorithms.hpp"

int ReadInt(const std::string& prompt) { // безопасный ввод int
    while (true) {
        std::cout << prompt;

        std::string line;
        if (!std::getline(std::cin, line)) {
            throw InvalidArgument("Input stream was closed");
        }

        std::istringstream input(line); // разбираем строку как число
        int value;
        char extra;
        if ((input >> value) && !(input >> extra)) {
            return value;
        }

        std::cout << "Input error: enter an integer number\n";
    }
}

template <class T>
void PrintSequence(const Sequence<T>* sequence) { // печать любой Sequence
    std::cout << "[";
    for (int i = 0; i < sequence->GetLength(); ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << sequence->Get(i);
    }
    std::cout << "]";
}

void ReplaceIfNewInstance(Sequence<int>*& sequence, Sequence<int>* result) { // нужно для immutable
    if (result != sequence) {
        delete sequence;
        sequence = result;
    }
}

void FillSequence(Sequence<int>*& sequence) { // заполнение последовательности с клавиатуры
    int count = ReadInt("Count: ");
    if (count < 0) {
        throw InvalidArgument("Count cannot be negative");
    }

    for (int i = 0; i < count; ++i) {
        std::ostringstream prompt;
        prompt << "item[" << i << "]: ";
        int value = ReadInt(prompt.str());
        ReplaceIfNewInstance(sequence, sequence->Append(value));
    }
}

MutableArraySequence<int> ReadInsertedSequence() { // читаем элементы для вставки в Slice
    int insertCount = ReadInt("How many items to insert: ");
    if (insertCount < 0) {
        throw InvalidArgument("Insert count cannot be negative");
    }

    MutableArraySequence<int> inserted;
    for (int i = 0; i < insertCount; ++i) {
        std::ostringstream prompt;
        prompt << "insert[" << i << "]: ";
        inserted.Append(ReadInt(prompt.str()));
    }
    return inserted;
}

void WorkWithSequence(Sequence<int>* sequence) { // меню работы с конкретной sequence
    bool running = true;
    while (running) {
        std::cout << "\nSequence menu\n";
        std::cout << "1. Print\n";
        std::cout << "2. Append\n";
        std::cout << "3. Prepend\n";
        std::cout << "4. InsertAt\n";
        std::cout << "5. Get\n";
        std::cout << "6. Subsequence\n";
        std::cout << "7. Map x*x\n";
        std::cout << "8. Where even\n";
        std::cout << "9. Reduce sum\n";
        std::cout << "10. Slice\n";
        std::cout << "0. Back\n";
        int choice = ReadInt("Choice: ");

        try {
            if (choice == 1) {
                PrintSequence(sequence);
                std::cout << "\n";
            } else if (choice == 2) {
                int value = ReadInt("Value: "); // добавить в конец
                ReplaceIfNewInstance(sequence, sequence->Append(value));
            } else if (choice == 3) {
                int value = ReadInt("Value: "); // добавить в начало
                ReplaceIfNewInstance(sequence, sequence->Prepend(value));
            } else if (choice == 4) {
                int value = ReadInt("Value: ");
                int index = ReadInt("Index: "); // куда вставить
                ReplaceIfNewInstance(sequence, sequence->InsertAt(value, index));
            } else if (choice == 5) {
                int index = ReadInt("Index: ");
                std::cout << sequence->Get(index) << "\n";
            } else if (choice == 6) {
                int start = ReadInt("Start index: ");
                int end = ReadInt("End index: ");
                Sequence<int>* sub = sequence->GetSubsequence(start, end);
                PrintSequence(sub);
                std::cout << "\n";
                delete sub;
            } else if (choice == 7) {
                Sequence<int>* result = sequence->Map<int>([](int x) { return x * x; });
                PrintSequence(result);
                std::cout << "\n";
                delete result;
            } else if (choice == 8) {
                Sequence<int>* result = sequence->Where([](int x) { return x % 2 == 0; });
                PrintSequence(result);
                std::cout << "\n";
                delete result;
            } else if (choice == 9) {
                int sum = sequence->Reduce<int>([](int acc, int x) { return acc + x; }, 0);
                std::cout << "Sum: " << sum << "\n";
            } else if (choice == 10) {
                int index = ReadInt("Start index, negative means from end: ");
                int count = ReadInt("How many items to remove: ");
                MutableArraySequence<int> inserted = ReadInsertedSequence(); // на что заменяем
                Sequence<int>* result = sequence->Slice(index, count, &inserted); // новый результат
                delete sequence;
                sequence = result;
                std::cout << "Result: ";
                PrintSequence(sequence);
                std::cout << "\n";
            } else if (choice == 0) {
                running = false;
            }
        } catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << "\n";
        }
    }

    delete sequence;
}

void Benchmark() { // замер времени операций
    const int appendCount = 20000;
    const int initialCount = 5000;
    const int insertCount = 2000;
    MutableArraySequence<int> arrayAppendSequence;
    MutableListSequence<int> listAppendSequence;

    auto startArray = std::chrono::high_resolution_clock::now(); // старт замера Array append
    for (int i = 0; i < appendCount; ++i) {
        arrayAppendSequence.Append(i);
    }
    auto endArray = std::chrono::high_resolution_clock::now();

    auto startList = std::chrono::high_resolution_clock::now(); // старт замера List append
    for (int i = 0; i < appendCount; ++i) {
        listAppendSequence.Append(i);
    }
    auto endList = std::chrono::high_resolution_clock::now();

    MutableArraySequence<int> arrayInsertSequence; // отдельные sequence для вставки в середину
    MutableListSequence<int> listInsertSequence;
    for (int i = 0; i < initialCount; ++i) {
        arrayInsertSequence.Append(i);
        listInsertSequence.Append(i);
    }

    auto startArrayInsert = std::chrono::high_resolution_clock::now(); // старт замера Array insert
    for (int i = 0; i < insertCount; ++i) {
        arrayInsertSequence.InsertAt(-1, arrayInsertSequence.GetLength() / 2);
    }
    auto endArrayInsert = std::chrono::high_resolution_clock::now();

    auto startListInsert = std::chrono::high_resolution_clock::now(); // старт замера List insert
    for (int i = 0; i < insertCount; ++i) {
        listInsertSequence.InsertAt(-1, listInsertSequence.GetLength() / 2);
    }
    auto endListInsert = std::chrono::high_resolution_clock::now();

    std::cout << "Append " << appendCount << " items\n";
    std::cout << "ArraySequence: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endArray - startArray).count()
              << " ms\n";
    std::cout << "ListSequence: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endList - startList).count()
              << " ms\n";

    std::cout << "\nInsertAt middle " << insertCount
              << " times, initial length " << initialCount << "\n";
    std::cout << "ArraySequence: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endArrayInsert - startArrayInsert).count()
              << " ms\n";
    std::cout << "ListSequence: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endListInsert - startListInsert).count()
              << " ms\n";
}

int main() { // главное меню программы
    bool running = true;
    while (running) {
        std::cout << "\nLab 2: Sequence ADT\n";
        std::cout << "1. Mutable ArraySequence<int>\n";
        std::cout << "2. Immutable ArraySequence<int>\n";
        std::cout << "3. Mutable ListSequence<int>\n";
        std::cout << "4. Immutable ListSequence<int>\n";
        std::cout << "5. Benchmark\n";
        std::cout << "0. Exit\n";
        int choice = ReadInt("Choice: ");

        try {
            if (choice == 1) {
                Sequence<int>* sequence = new MutableArraySequence<int>();
                FillSequence(sequence);
                WorkWithSequence(sequence);
            } else if (choice == 2) {
                Sequence<int>* sequence = new ImmutableArraySequence<int>();
                FillSequence(sequence);
                WorkWithSequence(sequence);
            } else if (choice == 3) {
                Sequence<int>* sequence = new MutableListSequence<int>();
                FillSequence(sequence);
                WorkWithSequence(sequence);
            } else if (choice == 4) {
                Sequence<int>* sequence = new ImmutableListSequence<int>();
                FillSequence(sequence);
                WorkWithSequence(sequence);
            } else if (choice == 5) {
                Benchmark();
            } else if (choice == 0) {
                running = false;
            } else {
                std::cout << "Unknown command\n";
            }
        } catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << "\n";
        }

    }

    return 0;
}
