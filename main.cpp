#include <vector>
#include <iostream>
#include "vector.h"

#define ASSERT_EXCEPT(x) \
    try { x; throw std::logic_error("Test failed: no except"); } catch (const std::runtime_error& e) { }
#define ASSERT(x) if (!(x)) throw std::logic_error("Test failed")

void test_basic() {
    Vector<int> a;

    for (int i = 0; i < 10; ++i) {
        a.PushBack(i);
    }
    ASSERT(a.Size() == 10);
    ASSERT(!a.Empty());
    for (size_t i = 0; i < 10; ++i) {
        ASSERT(a[i] == static_cast<int>(i));
        ASSERT(a.At(i) == static_cast<int>(i));
    }

    ASSERT(a.Capacity() == 16);
    a.ShrinkToFit();
    ASSERT(a.Capacity() == 10);

    a.PopBack();
    ASSERT(a.Size() == 9);
    a.Clear();
    ASSERT(a.Size() == 0);

    try {
        a.PopBack();
        throw std::logic_error("Test failed: no except");
    } catch (const std::runtime_error& e) {

    }
    ASSERT_EXCEPT(a.PopBack());

    a.Reserve(1000);
    ASSERT(a.Capacity() == 1000);
}

void test_iter() {
    Vector<int> a;

    for (int i = 0; i < 10; ++i) {
        a.PushBack(i);
    }
    int i = 0;
    for (const auto it : a) {
        ASSERT(it == i++);
    }
}

void test_swap() {
    Vector<int> a, b;
    a.PushBack(1);
    b.PushBack(2);

    a.Swap(b);
    ASSERT(a.At(0) == 2);
    ASSERT(b.At(0) == 1);
}

void test_move() {
    Vector<std::string> a;
    std::string t = "abcde1";
    a.PushBack(std::move(t));
    ASSERT(t.empty());
    ASSERT(a.At(0) == "abcde1");
}

void test_resize() {
    Vector<std::string> a;

    a.Resize(10, "asb");
    ASSERT(a.Size() == 10);
    for (int i = 0; i < 10; ++i) {
        ASSERT(a.At(i) == "asb");
    }
}

void test_emplace_back() {
    Vector<std::vector<int>> a;

    a.EmplaceBack(1, 2);
    ASSERT(a.Size() == 1);
    ASSERT(a[0][0] == 2);
}

int main() {
    test_basic();
    test_iter();
    test_swap();
    test_move();
    test_resize();
    test_emplace_back();

    std::cout << "Everything is fine!" << std::endl;
}
