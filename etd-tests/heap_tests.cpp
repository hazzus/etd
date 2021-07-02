#include "heap.h"
#include <gtest/gtest.h>

TEST(heap, dummy) {
    etd::heap<int> x;
    x.push(1);
    ASSERT_EQ(x.top(), 1);
}

std::vector<int> lost = {4, 8, 15, 16, 23, 42};
template <typename T, typename C, typename A>
void push_lost(etd::heap<T, C, A>& to) {
    std::random_shuffle(lost.begin(), lost.end());
    for (int i : lost) {
        to.push(i);
    }
}

TEST(heap, simple) {
    etd::heap<int> x;
    ASSERT_TRUE(x.empty());
    push_lost(x);
    ASSERT_EQ(x.top(), 4);
    ASSERT_EQ(x.size(), 6);
    x.pop();
    ASSERT_EQ(x.top(), 8);
    ASSERT_EQ(x.size(), 5);
}

TEST(heap, greater) {
    etd::heap<int, std::greater<int>> x;
    push_lost(x);
    ASSERT_EQ(x.top(), 42);
    x.pop();
    ASSERT_EQ(x.top(), 23);
}

TEST(heap, copy) {
    etd::heap<int> x, y;
    push_lost(x);
    auto z = x;
    y = x;
    ASSERT_EQ(x, z);
    ASSERT_EQ(x, y);
}
