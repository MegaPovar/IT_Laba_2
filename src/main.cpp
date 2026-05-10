#include <chrono> // бенчмарки
#include <iostream>
#include <limits> // для std::numeric_limits
#include <sstream>
#include <string>

#include "ArraySequence.hpp"
#include "BitSequence.hpp"
#include "ListSequence.hpp"
#include "SequenceAlgorithms.hpp"

int ReadInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;

        std::string line;
        if (!std::getline(std::cin, line)) {
            throw InvalidArgument("Input stream was closed");
        }

        std::istringstream input(line);
        int value;
        char extra;
        if ((input >> value) && !(input >> extra)) {
            return value;
        }

        std::cout << "Input error: enter an integer number\n";
    }
}

std::string ReadString(const std::string& prompt) {
    while (true) {
        std::cout << prompt;

        std::string value;
        if (!std::getline(std::cin, value)) {
            throw InvalidArgument("Input stream was closed");
        }

        if (!value.empty()) {
            return value;
        }

        std::cout << "Input error: value cannot be empty\n";
    }
}

template <class T>
void PrintSequence(const Sequence<T>* sequence) {
    std::cout << "[";
    for (int i = 0; i < sequence->GetLength(); ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << sequence->Get(i);
    }
    std::cout << "]";
}

void ReplaceIfNewInstance(Sequence<int>*& sequence, Sequence<int>* result) {
    if (result != sequence) {
        delete sequence;
        sequence = result;
    }
}

void FillSequence(Sequence<int>*& sequence) {
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

MutableArraySequence<int> ReadInsertedSequence() {
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

void WorkWithSequence(Sequence<int>* sequence) {
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
                int value = ReadInt("Value: ");
                ReplaceIfNewInstance(sequence, sequence->Append(value));
            } else if (choice == 3) {
                int value = ReadInt("Value: ");
                ReplaceIfNewInstance(sequence, sequence->Prepend(value));
            } else if (choice == 4) {
                int value = ReadInt("Value: ");
                int index = ReadInt("Index: ");
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
                MutableArraySequence<int> inserted = ReadInsertedSequence();
                Sequence<int>* result = sequence->Slice(index, count, &inserted);
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

void BitSequenceDemo() {
    std::string first = ReadString("First bit sequence, example 1010: ");
    std::string second = ReadString("Second bit sequence: ");

    BitSequence a(first.c_str());
    BitSequence b(second.c_str());
    BitSequence andResult = a.And(b);
    BitSequence orResult = a.Or(b);
    BitSequence xorResult = a.Xor(b);
    BitSequence notResult = a.Not();

    std::cout << "AND: ";
    PrintSequence(&andResult);
    std::cout << "\nOR: ";
    PrintSequence(&orResult);
    std::cout << "\nXOR: ";
    PrintSequence(&xorResult);
    std::cout << "\nNOT first: ";
    PrintSequence(&notResult);
    std::cout << "\n";
}

void Benchmark() {
    const int count = 20000;
    MutableArraySequence<int> arraySequence;
    MutableListSequence<int> listSequence;

    auto startArray = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < count; ++i) {
        arraySequence.Append(i);
    }
    auto endArray = std::chrono::high_resolution_clock::now();

    auto startList = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < count; ++i) {
        listSequence.Append(i);
    }
    auto endList = std::chrono::high_resolution_clock::now();

    std::cout << "Append " << count << " items\n";
    std::cout << "ArraySequence: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endArray - startArray).count()
              << " ms\n";
    std::cout << "ListSequence: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endList - startList).count()
              << " ms\n";
}

int main() {
    bool running = true;
    while (running) {
        std::cout << "\nLab 2: Sequence ADT\n";
        std::cout << "1. Mutable ArraySequence<int>\n";
        std::cout << "2. Immutable ArraySequence<int>\n";
        std::cout << "3. Mutable ListSequence<int>\n";
        std::cout << "4. Immutable ListSequence<int>\n";
        std::cout << "5. BitSequence demo\n";
        std::cout << "6. Benchmark\n";
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
                BitSequenceDemo();
            } else if (choice == 6) {
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
