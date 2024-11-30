#include <gtest/gtest.h>
#include <sstream>
#include "CCircularBuffer.h"
#include "CCircularBufferExt.h"

TEST(Push_back, brackets) {
    CCircularBuffer<int, std::allocator<int>> a(5);
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    a.push_back(5);
    ASSERT_EQ(1, a[0]);
    ASSERT_EQ(2, a[1]);
    ASSERT_EQ(3, a[2]);
    ASSERT_EQ(4, a[3]);
    ASSERT_EQ(5, a[4]);
}

TEST(Push_back, brackets_overflow) {
    CCircularBuffer<int, std::allocator<int>> a(5);
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    a.push_back(5);
    ASSERT_EQ(1, a[5]);
    ASSERT_EQ(2, a[6]);
    ASSERT_EQ(3, a[7]);
    ASSERT_EQ(4, a[8]);
    ASSERT_EQ(5, a[9]);
}

TEST(Push_back, brackets_negative) {
    CCircularBuffer<int, std::allocator<int>> a(5);
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    a.push_back(5);
    ASSERT_EQ(1, a[0]);
    ASSERT_EQ(5, a[-1]);
    ASSERT_EQ(4, a[-2]);
    ASSERT_EQ(3, a[-3]);
    ASSERT_EQ(2, a[-4]);
}

TEST(Push_back, int) {
    CCircularBuffer<int, std::allocator<int>> a(5);
    a.push_back(10);
    a.push_back(25);
    ASSERT_EQ(10, a[0]);
    ASSERT_EQ(25, a[1]);
}

TEST(Push_back, negative) {
    CCircularBuffer<int, std::allocator<int>> a(5);
    a.push_back(-10);
    a.push_back(-25);
    ASSERT_EQ(-10, a[0]);
    ASSERT_EQ(-25, a[1]);
}

TEST(Push_back, strings) {
    CCircularBuffer<std::string, std::allocator<std::string>> a(5);
    a.push_back("Hello World!");
    a.push_back("Love coding");
    ASSERT_EQ("Hello World!", a[0]);
    ASSERT_EQ("Love coding", a[1]);
}

TEST(Push_back, overwriting) {
    CCircularBuffer<int, std::allocator<int>> a(3);
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    a.push_back(5);
    ASSERT_EQ(3, a[0]);
    ASSERT_EQ(4, a[1]);
    ASSERT_EQ(5, a[2]);
}

TEST(Pop_Front, int) {
    CCircularBuffer<int, std::allocator<int>> a(5);
    a.push_back(1);
    a.push_back(3);
    a.push_back(4);
    a.push_back(2);
    a.push_back(5);

    a.pop_front();
    a.pop_front();

    ASSERT_EQ(4, a[0]);
    ASSERT_EQ(2, a[1]);
    ASSERT_EQ(5, a[2]);
}

TEST(Pop_front, char){
    CCircularBuffer<char, std::allocator<char>> a(5);
    a.push_back('a');
    a.push_back('b');
    a.push_back('c');
    a.push_back('d');
    a.push_back('e');
    a.push_back('f');

    a.pop_front();
    a.pop_front();

    a.push_back('g');

    ASSERT_EQ('d', a[0]);
    ASSERT_EQ('e', a[1]);
    ASSERT_EQ('f', a[2]);
    ASSERT_EQ('g', a[3]);
}

TEST(Algorithms, sort) {
    CCircularBuffer<int, std::allocator<int>> a(4);
    a.push_back(1);
    a.push_back(3);
    a.push_back(4);
    a.push_back(2);
    std::sort(a.begin(), a.end());
    ASSERT_EQ(1, a[0]);
    ASSERT_EQ(2, a[1]);
    ASSERT_EQ(3, a[2]);
    ASSERT_EQ(4, a[3]);
}

TEST(Algorithms, decreasing_sort) {
    CCircularBuffer<int, std::allocator<int>> a(4);
    a.push_back(1);
    a.push_back(3);
    a.push_back(4);
    a.push_back(2);
    std::sort(a.begin(), a.end(), std::greater<>());
    ASSERT_EQ(4, a[0]);
    ASSERT_EQ(3, a[1]);
    ASSERT_EQ(2, a[2]);
    ASSERT_EQ(1, a[3]);
}

TEST(Algorithms, reverse) {
    CCircularBuffer<int, std::allocator<int>> a(4);
    a.push_back(1);
    a.push_back(3);
    a.push_back(4);
    a.push_back(2);
    std::reverse(a.begin(), a.end());
    ASSERT_EQ(2, a[0]);
    ASSERT_EQ(4, a[1]);
    ASSERT_EQ(3, a[2]);
    ASSERT_EQ(1, a[3]);
}
