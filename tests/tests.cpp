#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

#include "ArraySequence.hpp"
#include "BitSequence.hpp"
#include "LinkedList.hpp"
#include "ListSequence.hpp"
#include "SequenceAlgorithms.hpp"

static void TestDynamicArray() { // тесты DynamicArray
    int source[] = {1, 2, 3};
    DynamicArray<int> array(source, 3);
    assert(array.GetSize() == 3);
    assert(array.Get(1) == 2);
    array.Set(1, 20);
    assert(array[1] == 20);
    array.Resize(5);
    assert(array.GetSize() == 5);
    array.Set(4, 50);
    assert(array.Get(4) == 50);

    bool thrown = false;
    try {
        array.Get(10);
    } catch (const IndexOutOfRange&) {
        thrown = true;
    }
    assert(thrown);
}

static void TestLinkedList() { // тесты LinkedList
    int source[] = {2, 3};
    LinkedList<int> list(source, 2);
    list.Prepend(1);
    list.Append(4);
    list.InsertAt(10, 2);
    assert(list.GetLength() == 5);
    assert(list.GetFirst() == 1);
    assert(list.GetLast() == 4);
    assert(list.Get(2) == 10);

    LinkedList<int>* sub = list.GetSubList(1, 3);
    assert(sub->GetLength() == 3);
    assert(sub->Get(0) == 2);
    delete sub;
}

static void TestSequences() { // тесты mutable/immutable sequence
    int source[] = {1, 2, 3};
    MutableArraySequence<int> mutableArray(source, 3);
    Sequence<int>* same = mutableArray.Append(4);
    assert(same == &mutableArray); // append возвращает тот же объект
    assert(mutableArray.GetLast() == 4);

    ImmutableArraySequence<int> immutableArray(source, 3);
    Sequence<int>* changed = immutableArray.Append(4);  
    assert(immutableArray.GetLength() == 3);
    assert(changed->GetLength() == 4);
    assert(changed->GetLast() == 4);
    delete changed;

    MutableListSequence<int> listSeq(source, 3);
    listSeq.Prepend(0);
    assert(listSeq.GetFirst() == 0);
    assert(listSeq[2] == 2);
}

static void TestAlgorithms() { // тесты map/reduce/slice/option
    int source[] = {1, 2, 3, 4, 5};
    MutableArraySequence<int> sequence(source, 5);

    Sequence<int>* squares = sequence.Map<int>([](int x) { return x * x; }); // квадраты
    assert(squares->Get(2) == 9);
    delete squares;

    Sequence<int>* even = sequence.Where([](int x) { return x % 2 == 0; }); // только четные
    assert(even->GetLength() == 2);
    assert(even->Get(1) == 4);
    delete even;

    int sum = sequence.Reduce<int>([](int acc, int x) { return acc + x; }, 0); // сумма
    assert(sum == 15);

    Sequence<int>* indexed = sequence.MapIndexed<int>([](int x, int index) { return x + index; });
    assert(indexed->Get(3) == 7);
    delete indexed;

    Sequence<int>* inserted = new MutableArraySequence<int>();
    inserted->Append(9)->Append(10);
    Sequence<int>* sliced = sequence.Slice(1, 2, inserted);
    assert(sliced->GetLength() == 5);
    assert(sliced->Get(0) == 1);
    assert(sliced->Get(1) == 9);
    assert(sliced->Get(2) == 10);
    assert(sliced->Get(3) == 4);
    delete inserted;
    delete sliced;

    Option<int> found = sequence.TryGetFirst([](int x) { return x > 3; });
    assert(found.HasValue());
    assert(found.Value() == 4);
}

static void TestBitSequence() { // тесты BitSequence
    BitSequence a("1010");
    BitSequence b("1100");

    BitSequence andResult = a.And(b);
    BitSequence orResult = a.Or(b);
    BitSequence xorResult = a.Xor(b);
    BitSequence notResult = a.Not();

    assert(andResult.Get(0) == Bit(1));
    assert(andResult.Get(1) == Bit(0));
    assert(orResult.Get(1) == Bit(1));
    assert(xorResult.Get(1) == Bit(1));
    assert(notResult.Get(0) == Bit(0));
    assert(notResult.Get(1) == Bit(1));
    assert(a.IsSet(2));
}

int main() {
    TestDynamicArray();
    TestLinkedList();
    TestSequences();
    TestAlgorithms();
    TestBitSequence();

    std::cout << "All tests passed" << std::endl;
    return 0;
}
