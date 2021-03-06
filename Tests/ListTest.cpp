#include <iostream>
#include <thread>
#include <gtest/gtest.h>

#include "LinkedList.hpp"

namespace {
    struct TestClass
    {
        int field;
        void method() {}
    };

    bool operator==(const TestClass &cl1, const TestClass &cl2) { return cl1.field == cl2.field; }
    bool operator!=(const TestClass &cl1, const TestClass &cl2) { return cl1.field != cl2.field; }
    bool operator<(const TestClass &cl1, const TestClass &cl2) { return cl1.field < cl2.field; }
    bool operator>(const TestClass &cl1, const TestClass &cl2) { return cl1.field > cl2.field; }
    bool operator<=(const TestClass &cl1, const TestClass &cl2) { return cl1.field <= cl2.field; }
    bool operator>=(const TestClass &cl1, const TestClass &cl2) { return cl1.field >= cl2.field; }
}

class ListTest : public ::testing::Test
{
protected:
    static void SetUpTestSuite() {}

    ListTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ListTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ListTest, AtTest)
{
    sd::List<int> l = {1, 2, 3, 4, 5};

    EXPECT_EQ(l.at(0), 1);
    EXPECT_EQ(l.at(1), 2);
    EXPECT_EQ(l.at(2), 3);
    EXPECT_EQ(l.at(3), 4);
    EXPECT_EQ(l.at(4), 5);

    EXPECT_THROW(l.at(-2), std::out_of_range);
    EXPECT_THROW(
        try
        {
            l.at(22);
        } catch (const std::out_of_range &e)
        {
            // and this tests that it has the correct message
            EXPECT_STREQ("Index: 22 exceeded allowed boundaries, current list size is: 5", e.what());
            throw;
        },
        std::out_of_range);
}

TEST_F(ListTest, AtOperatorTest)
{
    sd::List<int> l = {1, 2, 3, 4, 5};

    EXPECT_EQ(l[0], 1);
    EXPECT_EQ(l[1], 2);
    EXPECT_EQ(l[2], 3);
    EXPECT_EQ(l[3], 4);
    EXPECT_EQ(l[4], 5);

    EXPECT_THROW(l[-2], std::out_of_range);
    EXPECT_THROW(
        try
        {
            l[22];
        } catch (const std::out_of_range &e)
        {
            // and this tests that it has the correct message
            EXPECT_STREQ("Index: 22 exceeded allowed boundaries, current list size is: 5", e.what());
            throw;
        },
        std::out_of_range);
}

TEST_F(ListTest, AtOperatorClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    EXPECT_EQ(l[0], TestClass{1});
    EXPECT_EQ(l[1], TestClass{2});
    EXPECT_EQ(l[2], TestClass{3});
    EXPECT_EQ(l[3], TestClass{4});
    EXPECT_EQ(l[4], TestClass{5});

    EXPECT_THROW(l[-2], std::out_of_range);
    EXPECT_THROW(
        try
        {
            l[22];
        } catch (const std::out_of_range &e)
        {
            // and this tests that it has the correct message
            EXPECT_STREQ("Index: 22 exceeded allowed boundaries, current list size is: 5", e.what());
            throw;
        },
        std::out_of_range);
}

TEST_F(ListTest, FrontClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    EXPECT_EQ(l.front(), TestClass{1});
}

TEST_F(ListTest, FrontClassFailTest)
{
    sd::List<TestClass> l;

    EXPECT_THROW(
        try
        {
            l.front();
        } catch (const std::runtime_error &e)
        {
            EXPECT_STREQ("List is empty", e.what());
            throw;
        },
        std::runtime_error);
}

TEST_F(ListTest, BackClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    EXPECT_EQ(l.back(), TestClass{5});
}

TEST_F(ListTest, BackClassFailTest)
{
    sd::List<TestClass> l;

    EXPECT_THROW(
        try
        {
            l.back();
        } catch (const std::runtime_error &e)
        {
            EXPECT_STREQ("List is empty", e.what());
            throw;
        },
        std::runtime_error);
}

TEST_F(ListTest, IteratorClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.begin();
    EXPECT_EQ(*it, TestClass{1});
    EXPECT_EQ(*++it, TestClass{2});
    EXPECT_EQ(*++it, TestClass{3});
    EXPECT_EQ(*++it, TestClass{4});
    EXPECT_EQ(*++it, TestClass{5});
    EXPECT_FALSE(++it);
}

TEST_F(ListTest, IteratorBackClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.begin();
    EXPECT_EQ(*it, TestClass{1});
    EXPECT_EQ(*++it, TestClass{2});
    EXPECT_EQ(*--it, TestClass{1});
    EXPECT_EQ(*++it, TestClass{2});
    EXPECT_EQ(*++it, TestClass{3});
    EXPECT_EQ(*++it, TestClass{4});
    EXPECT_EQ(*++it, TestClass{5});
    EXPECT_EQ(*--it, TestClass{4});
    EXPECT_FALSE(++(++it));
}

TEST_F(ListTest, IteratorCompareTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.begin();
    EXPECT_EQ(it, l.begin());
    EXPECT_NE(++it, l.begin());
    EXPECT_NE(++it, l.end());
    EXPECT_EQ(l.end(), l.end());
}

TEST_F(ListTest, IteratorPostIncrementClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.begin();
    EXPECT_EQ(*(it++), TestClass{1});
    EXPECT_EQ(*it, TestClass{2});
    EXPECT_EQ(*(it++), TestClass{2});
    EXPECT_EQ(*(it++), TestClass{3});
    EXPECT_EQ(*(it++), TestClass{4});
    EXPECT_EQ(*(it++), TestClass{5});
    EXPECT_FALSE(it);
}

TEST_F(ListTest, IteratorPostDecrementClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.begin();
    EXPECT_EQ(*it, TestClass{1});
    EXPECT_EQ(*(it++), TestClass{1});
    EXPECT_EQ(*(it--), TestClass{2});
    EXPECT_EQ(*(it++), TestClass{1});
    EXPECT_EQ(*it, TestClass{2});
    EXPECT_EQ(*(it++), TestClass{2});
    EXPECT_EQ(*(it++), TestClass{3});
    EXPECT_EQ(*(it++), TestClass{4});
    EXPECT_EQ(*(it--), TestClass{5});
    EXPECT_EQ(*(it++), TestClass{4});
    EXPECT_EQ(*(it++), TestClass{5});
    EXPECT_FALSE(it);
}

TEST_F(ListTest, IteratorAccesorClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.begin();
    EXPECT_EQ(it->field, 1);
    EXPECT_EQ((++it)->field, 2);
    EXPECT_EQ((++it)->field, 3);
    EXPECT_EQ((++it)->field, 4);
    EXPECT_EQ((++it)->field, 5);
    EXPECT_FALSE(++it);
}

TEST_F(ListTest, ReverseIteratorClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.rBegin();
    EXPECT_EQ(*it, TestClass{5});
    EXPECT_EQ(*++it, TestClass{4});
    EXPECT_EQ(*++it, TestClass{3});
    EXPECT_EQ(*++it, TestClass{2});
    EXPECT_EQ(*++it, TestClass{1});
    EXPECT_FALSE(++it);
}

TEST_F(ListTest, ReverseIteratorBackClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.rBegin();
    EXPECT_EQ(*it, TestClass{5});
    EXPECT_EQ(*++it, TestClass{4});
    EXPECT_EQ(*--it, TestClass{5});
    EXPECT_EQ(*++it, TestClass{4});
    EXPECT_EQ(*++it, TestClass{3});
    EXPECT_EQ(*++it, TestClass{2});
    EXPECT_EQ(*++it, TestClass{1});
    EXPECT_EQ(*--it, TestClass{2});
    EXPECT_FALSE(++(++it));
}

TEST_F(ListTest, ReverseIteratorCompareTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.rBegin();
    EXPECT_EQ(it, l.rBegin());
    EXPECT_NE(++it, l.rBegin());
    EXPECT_NE(++it, l.rEnd());
    EXPECT_EQ(l.rEnd(), l.rEnd());
}

TEST_F(ListTest, ReverseIteratorPostIncrementClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.rBegin();
    EXPECT_EQ(*(it++), TestClass{5});
    EXPECT_EQ(*it, TestClass{4});
    EXPECT_EQ(*(it++), TestClass{4});
    EXPECT_EQ(*(it++), TestClass{3});
    EXPECT_EQ(*(it++), TestClass{2});
    EXPECT_EQ(*(it++), TestClass{1});
    EXPECT_FALSE(it);
}

TEST_F(ListTest, ReverseIteratorPostDecrementClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.rBegin();
    EXPECT_EQ(*it, TestClass{5});
    EXPECT_EQ(*(it++), TestClass{5});
    EXPECT_EQ(*(it--), TestClass{4});
    EXPECT_EQ(*(it++), TestClass{5});
    EXPECT_EQ(*it, TestClass{4});
    EXPECT_EQ(*(it++), TestClass{4});
    EXPECT_EQ(*(it++), TestClass{3});
    EXPECT_EQ(*(it++), TestClass{2});
    EXPECT_EQ(*(it--), TestClass{1});
    EXPECT_EQ(*(it++), TestClass{2});
    EXPECT_EQ(*(it++), TestClass{1});
    EXPECT_FALSE(it);
}

TEST_F(ListTest, ReverseIteratorAccesorClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.rBegin();
    EXPECT_EQ(it->field, 5);
    EXPECT_EQ((++it)->field, 4);
    EXPECT_EQ((++it)->field, 3);
    EXPECT_EQ((++it)->field, 2);
    EXPECT_EQ((++it)->field, 1);
    EXPECT_FALSE(++it);
}

TEST_F(ListTest, ConstIteratorClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.cBegin();
    EXPECT_EQ(*it, TestClass{1});
    EXPECT_EQ(*++it, TestClass{2});
    EXPECT_EQ(*++it, TestClass{3});
    EXPECT_EQ(*++it, TestClass{4});
    EXPECT_EQ(*++it, TestClass{5});
    EXPECT_FALSE(++it);
}

TEST_F(ListTest, ConstIteratorBackClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.cBegin();
    EXPECT_EQ(*it, TestClass{1});
    EXPECT_EQ(*++it, TestClass{2});
    EXPECT_EQ(*--it, TestClass{1});
    EXPECT_EQ(*++it, TestClass{2});
    EXPECT_EQ(*++it, TestClass{3});
    EXPECT_EQ(*++it, TestClass{4});
    EXPECT_EQ(*++it, TestClass{5});
    EXPECT_EQ(*--it, TestClass{4});
    EXPECT_FALSE(++(++it));
}

TEST_F(ListTest, ConstIteratorCompareTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.cBegin();
    EXPECT_EQ(it, l.cBegin());
    EXPECT_NE(++it, l.cBegin());
    EXPECT_NE(++it, l.cEnd());
    EXPECT_EQ(l.cEnd(), l.cEnd());
}

TEST_F(ListTest, ConstIteratorPostIncrementClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.cBegin();
    EXPECT_EQ(*(it++), TestClass{1});
    EXPECT_EQ(*it, TestClass{2});
    EXPECT_EQ(*(it++), TestClass{2});
    EXPECT_EQ(*(it++), TestClass{3});
    EXPECT_EQ(*(it++), TestClass{4});
    EXPECT_EQ(*(it++), TestClass{5});
    EXPECT_FALSE(it);
}

TEST_F(ListTest, ConstIteratorPostDecrementClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.cBegin();
    EXPECT_EQ(*it, TestClass{1});
    EXPECT_EQ(*(it++), TestClass{1});
    EXPECT_EQ(*(it--), TestClass{2});
    EXPECT_EQ(*(it++), TestClass{1});
    EXPECT_EQ(*it, TestClass{2});
    EXPECT_EQ(*(it++), TestClass{2});
    EXPECT_EQ(*(it++), TestClass{3});
    EXPECT_EQ(*(it++), TestClass{4});
    EXPECT_EQ(*(it--), TestClass{5});
    EXPECT_EQ(*(it++), TestClass{4});
    EXPECT_EQ(*(it++), TestClass{5});
    EXPECT_FALSE(it);
}

TEST_F(ListTest, ConstIteratorAccesorClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.cBegin();
    EXPECT_EQ(it->field, 1);
    EXPECT_EQ((++it)->field, 2);
    EXPECT_EQ((++it)->field, 3);
    EXPECT_EQ((++it)->field, 4);
    EXPECT_EQ((++it)->field, 5);
    EXPECT_FALSE(++it);
}

TEST_F(ListTest, ConstReverseIteratorClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.crBegin();
    EXPECT_EQ(*it, TestClass{5});
    EXPECT_EQ(*++it, TestClass{4});
    EXPECT_EQ(*++it, TestClass{3});
    EXPECT_EQ(*++it, TestClass{2});
    EXPECT_EQ(*++it, TestClass{1});
    EXPECT_FALSE(++it);
}

TEST_F(ListTest, ConstReverseIteratorBackClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.crBegin();
    EXPECT_EQ(*it, TestClass{5});
    EXPECT_EQ(*++it, TestClass{4});
    EXPECT_EQ(*--it, TestClass{5});
    EXPECT_EQ(*++it, TestClass{4});
    EXPECT_EQ(*++it, TestClass{3});
    EXPECT_EQ(*++it, TestClass{2});
    EXPECT_EQ(*++it, TestClass{1});
    EXPECT_EQ(*--it, TestClass{2});
    EXPECT_FALSE(++(++it));
}

TEST_F(ListTest, ConstReverseIteratorCompareTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.crBegin();
    EXPECT_EQ(it, l.crBegin());
    EXPECT_NE(++it, l.crBegin());
    EXPECT_NE(++it, l.crEnd());
    EXPECT_EQ(l.crEnd(), l.crEnd());
}

TEST_F(ListTest, ConstReverseIteratorPostIncrementClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.crBegin();
    EXPECT_EQ(*(it++), TestClass{5});
    EXPECT_EQ(*it, TestClass{4});
    EXPECT_EQ(*(it++), TestClass{4});
    EXPECT_EQ(*(it++), TestClass{3});
    EXPECT_EQ(*(it++), TestClass{2});
    EXPECT_EQ(*(it++), TestClass{1});
    EXPECT_FALSE(it);
}

TEST_F(ListTest, ConstReverseIteratorPostDecrementClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.crBegin();
    EXPECT_EQ(*it, TestClass{5});
    EXPECT_EQ(*(it++), TestClass{5});
    EXPECT_EQ(*(it--), TestClass{4});
    EXPECT_EQ(*(it++), TestClass{5});
    EXPECT_EQ(*it, TestClass{4});
    EXPECT_EQ(*(it++), TestClass{4});
    EXPECT_EQ(*(it++), TestClass{3});
    EXPECT_EQ(*(it++), TestClass{2});
    EXPECT_EQ(*(it--), TestClass{1});
    EXPECT_EQ(*(it++), TestClass{2});
    EXPECT_EQ(*(it++), TestClass{1});
    EXPECT_FALSE(it);
}

TEST_F(ListTest, ConstReverseIteratorAccesorClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    auto it = l.crBegin();
    EXPECT_EQ(it->field, 5);
    EXPECT_EQ((++it)->field, 4);
    EXPECT_EQ((++it)->field, 3);
    EXPECT_EQ((++it)->field, 2);
    EXPECT_EQ((++it)->field, 1);
    EXPECT_FALSE(++it);
}

TEST_F(ListTest, EqualsClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};
    sd::List<TestClass> l2 = {{1}, {2}, {3}, {4}, {5}};

    EXPECT_EQ(l, l2);
}

TEST_F(ListTest, EqualsClassTest2)
{
    sd::List<TestClass> l;
    sd::List<TestClass> l2;

    EXPECT_EQ(l, l2);
}

TEST_F(ListTest, NotEqualsClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};
    sd::List<TestClass> l2 = {{11}, {2}, {3}, {4}, {5}};

    EXPECT_NE(l, l2);
}

TEST_F(ListTest, NotEqualsClassTest2)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}, {0}};
    sd::List<TestClass> l2 = {{11}, {2}, {3}, {4}, {5}};

    EXPECT_NE(l, l2);
}

TEST_F(ListTest, LessClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};
    sd::List<TestClass> l2 = {{1}, {2}, {3}, {4}, {6}};

    EXPECT_LT(l, l2);
}

TEST_F(ListTest, LessClassTest2)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};
    sd::List<TestClass> l2 = {{1}, {2}, {3}, {4}, {5}, {6}};

    EXPECT_LT(l, l2);
}

TEST_F(ListTest, GreatherClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {6}};
    sd::List<TestClass> l2 = {{1}, {2}, {3}, {4}, {5}};

    EXPECT_GT(l, l2);
}

TEST_F(ListTest, GreatherClassTest2)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}, {6}};
    sd::List<TestClass> l2 = {{1}, {2}, {3}, {4}, {5}};

    EXPECT_GT(l, l2);
}

TEST_F(ListTest, LessOrEqualClassTest)
{
    sd::List<TestClass> l = {{0}, {2}, {3}, {4}, {5}};
    sd::List<TestClass> l2 = {{1}, {2}, {3}, {4}, {5}};

    EXPECT_LE(l, l2);
}

TEST_F(ListTest, LessOrEqualClassTest2)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};
    sd::List<TestClass> l2 = {{1}, {2}, {3}, {4}, {5}};

    EXPECT_LE(l, l2);
}

TEST_F(ListTest, GreatherOrEqualClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};
    sd::List<TestClass> l2 = {{0}, {2}, {3}, {4}, {5}};

    EXPECT_GE(l, l2);
}

TEST_F(ListTest, GreatherOrEqualClassTest2)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};
    sd::List<TestClass> l2 = {{1}, {2}, {3}, {4}, {5}};

    EXPECT_GE(l, l2);
}

TEST_F(ListTest, PushBackClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    l.pushBack({22});
    l.pushBack({23});
    l.pushBack({23});

    EXPECT_EQ(l[0], TestClass{1});
    EXPECT_EQ(l[1], TestClass{2});
    EXPECT_EQ(l[2], TestClass{3});
    EXPECT_EQ(l[3], TestClass{4});
    EXPECT_EQ(l[4], TestClass{5});
    EXPECT_EQ(l[5], TestClass{22});
    EXPECT_EQ(l[6], TestClass{23});
    EXPECT_EQ(l[7], TestClass{23});
}

TEST_F(ListTest, PushFrontClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    l.pushFront({22});
    l.pushFront({23});
    l.pushFront({23});

    EXPECT_EQ(l[0], TestClass{23});
    EXPECT_EQ(l[1], TestClass{23});
    EXPECT_EQ(l[2], TestClass{22});
    EXPECT_EQ(l[3], TestClass{1});
    EXPECT_EQ(l[4], TestClass{2});
    EXPECT_EQ(l[5], TestClass{3});
    EXPECT_EQ(l[6], TestClass{4});
    EXPECT_EQ(l[7], TestClass{5});
}

TEST_F(ListTest, EmplaceBackClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    l.emplaceBack(22);
    l.emplaceBack(23);
    l.emplaceBack(23);

    EXPECT_EQ(l[0], TestClass{1});
    EXPECT_EQ(l[1], TestClass{2});
    EXPECT_EQ(l[2], TestClass{3});
    EXPECT_EQ(l[3], TestClass{4});
    EXPECT_EQ(l[4], TestClass{5});
    EXPECT_EQ(l[5], TestClass{22});
    EXPECT_EQ(l[6], TestClass{23});
    EXPECT_EQ(l[7], TestClass{23});
}

TEST_F(ListTest, EmplaceFrontClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    l.emplaceFront(22);
    l.emplaceFront(23);
    l.emplaceFront(23);

    EXPECT_EQ(l[0], TestClass{23});
    EXPECT_EQ(l[1], TestClass{23});
    EXPECT_EQ(l[2], TestClass{22});
    EXPECT_EQ(l[3], TestClass{1});
    EXPECT_EQ(l[4], TestClass{2});
    EXPECT_EQ(l[5], TestClass{3});
    EXPECT_EQ(l[6], TestClass{4});
    EXPECT_EQ(l[7], TestClass{5});
}

TEST_F(ListTest, InsertClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    l.insert(3, {22});
    l.insert(3, {23});
    l.insert(3, {24});

    EXPECT_EQ(l[0], TestClass{1});
    EXPECT_EQ(l[1], TestClass{2});
    EXPECT_EQ(l[2], TestClass{3});
    EXPECT_EQ(l[3], TestClass{24});
    EXPECT_EQ(l[4], TestClass{23});
    EXPECT_EQ(l[5], TestClass{22});
    EXPECT_EQ(l[6], TestClass{4});
    EXPECT_EQ(l[7], TestClass{5});
}

TEST_F(ListTest, InsertLastClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    l.insert(33, {22});
    l.insert(33, {23});
    l.insert(33, {24});

    EXPECT_EQ(l[0], TestClass{1});
    EXPECT_EQ(l[1], TestClass{2});
    EXPECT_EQ(l[2], TestClass{3});
    EXPECT_EQ(l[3], TestClass{4});
    EXPECT_EQ(l[4], TestClass{5});
    EXPECT_EQ(l[5], TestClass{22});
    EXPECT_EQ(l[6], TestClass{23});
    EXPECT_EQ(l[7], TestClass{24});
}

TEST_F(ListTest, EmplaceClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    l.emplace(3, 22);
    l.emplace(3, 23);
    l.emplace(3, 24);

    EXPECT_EQ(l[0], TestClass{1});
    EXPECT_EQ(l[1], TestClass{2});
    EXPECT_EQ(l[2], TestClass{3});
    EXPECT_EQ(l[3], TestClass{24});
    EXPECT_EQ(l[4], TestClass{23});
    EXPECT_EQ(l[5], TestClass{22});
    EXPECT_EQ(l[6], TestClass{4});
    EXPECT_EQ(l[7], TestClass{5});
}

TEST_F(ListTest, EmplaceLastClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    l.emplace(33, 22);
    l.emplace(33, 23);
    l.emplace(33, 24);

    EXPECT_EQ(l[0], TestClass{1});
    EXPECT_EQ(l[1], TestClass{2});
    EXPECT_EQ(l[2], TestClass{3});
    EXPECT_EQ(l[3], TestClass{4});
    EXPECT_EQ(l[4], TestClass{5});
    EXPECT_EQ(l[5], TestClass{22});
    EXPECT_EQ(l[6], TestClass{23});
    EXPECT_EQ(l[7], TestClass{24});
}

TEST_F(ListTest, RemoveClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    l.remove(0);
    l.remove(2);

    EXPECT_EQ(l[0], TestClass{2});
    EXPECT_EQ(l[1], TestClass{3});
    EXPECT_EQ(l[2], TestClass{5});

    EXPECT_EQ(l[0], l.front());
    EXPECT_EQ(l[2], l.back());
}

TEST_F(ListTest, RemoveFailClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    EXPECT_THROW(
        try
        {
            l.remove(22);
        } catch (const std::out_of_range &e)
        {
            // and this tests that it has the correct message
            EXPECT_STREQ("Index: 22 exceeded allowed boundaries, current list size is: 5", e.what());
            throw;
        },
        std::out_of_range);
}

TEST_F(ListTest, PopFrontClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    l.popFront();
    l.popFront();

    EXPECT_EQ(l[0], l.front());

    EXPECT_EQ(l[0], TestClass{3});
    EXPECT_EQ(l[1], TestClass{4});
    EXPECT_EQ(l[2], TestClass{5});
}

TEST_F(ListTest, PopFrontToEmptyClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    l.popFront();
    l.popFront();
    l.popFront();
    l.popFront();

    EXPECT_EQ(l.back(), l.front());
    EXPECT_EQ(l[0], TestClass{5});

    l.popFront();

    EXPECT_TRUE(l.empty());
}

TEST_F(ListTest, PopFrontFailClassTest)
{
    sd::List<TestClass> l;

    EXPECT_THROW(
        try
        {
            l.popFront();
        } catch (const std::runtime_error &e)
        {
            // and this tests that it has the correct message
            EXPECT_STREQ("List is empty", e.what());
            throw;
        },
        std::runtime_error);
}

TEST_F(ListTest, PopBackClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    l.popBack();
    l.popBack();

    EXPECT_EQ(l[0], TestClass{1});
    EXPECT_EQ(l[1], TestClass{2});
    EXPECT_EQ(l[2], TestClass{3});

    EXPECT_EQ(l[2], l.back());
}

TEST_F(ListTest, PopBackToEmptyClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    l.popBack();
    l.popBack();
    l.popBack();
    l.popBack();

    EXPECT_EQ(l.back(), l.front());
    EXPECT_EQ(l[0], TestClass{1});

    l.popBack();

    EXPECT_TRUE(l.empty());
}

TEST_F(ListTest, PopBackFailClassTest)
{
    sd::List<TestClass> l;

    EXPECT_THROW(
        try
        {
            l.popBack();
        } catch (const std::runtime_error &e)
        {
            // and this tests that it has the correct message
            EXPECT_STREQ("List is empty", e.what());
            throw;
        },
        std::runtime_error);
}

TEST_F(ListTest, ClearClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    EXPECT_EQ(l.size(), 5);
    EXPECT_TRUE(l.begin());

    l.clear();

    EXPECT_EQ(l.size(), 0);
    EXPECT_FALSE(l.begin());
}

TEST_F(ListTest, SizeClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    EXPECT_EQ(l.size(), 5);

    l.pushBack({22});

    EXPECT_EQ(l.size(), 6);

    l.remove(0);
    l.remove(0);

    EXPECT_EQ(l.size(), 4);
}

TEST_F(ListTest, EmptyClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    EXPECT_FALSE(l.empty());

    l.clear();

    EXPECT_TRUE(l.empty());
}

TEST_F(ListTest, SpawnConstructorClassTest)
{
    sd::List<TestClass> l(3, TestClass{1});

    EXPECT_EQ(l.size(), 3);

    EXPECT_EQ(l[0], TestClass{1});
    EXPECT_EQ(l[1], TestClass{1});
    EXPECT_EQ(l[2], TestClass{1});
}

TEST_F(ListTest, IteratorConstructorClassTest)
{
    std::vector<TestClass> v = {{1}, {2}, {3}, {4}, {5}};
    sd::List<TestClass> l(v.begin(), v.end());

    EXPECT_EQ(l.size(), 5);

    EXPECT_EQ(l[0], TestClass{1});
    EXPECT_EQ(l[1], TestClass{2});
    EXPECT_EQ(l[2], TestClass{3});
    EXPECT_EQ(l[3], TestClass{4});
    EXPECT_EQ(l[4], TestClass{5});
}

TEST_F(ListTest, CopyConstructorClassTest)
{
    sd::List<TestClass> v = {{1}, {2}, {3}, {4}, {5}};
    sd::List<TestClass> l(v);

    EXPECT_EQ(l.size(), 5);

    EXPECT_EQ(l[0], TestClass{1});
    EXPECT_EQ(l[1], TestClass{2});
    EXPECT_EQ(l[2], TestClass{3});
    EXPECT_EQ(l[3], TestClass{4});
    EXPECT_EQ(l[4], TestClass{5});
}

TEST_F(ListTest, MoveConstructorClassTest)
{
    sd::List<TestClass> v = {{1}, {2}, {3}, {4}, {5}};
    sd::List<TestClass> l(std::move(v));

    EXPECT_TRUE(v.empty());
    EXPECT_EQ(l.size(), 5);

    EXPECT_EQ(l[0], TestClass{1});
    EXPECT_EQ(l[1], TestClass{2});
    EXPECT_EQ(l[2], TestClass{3});
    EXPECT_EQ(l[3], TestClass{4});
    EXPECT_EQ(l[4], TestClass{5});
}

TEST_F(ListTest, InitializerListConstructorClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    EXPECT_EQ(l.size(), 5);

    EXPECT_EQ(l[0], TestClass{1});
    EXPECT_EQ(l[1], TestClass{2});
    EXPECT_EQ(l[2], TestClass{3});
    EXPECT_EQ(l[3], TestClass{4});
    EXPECT_EQ(l[4], TestClass{5});
}

TEST_F(ListTest, AssignmentClassTest)
{
    sd::List<TestClass> v = {{1}, {2}, {3}, {4}, {5}};
    sd::List<TestClass> l;
    l = v;

    EXPECT_EQ(l.size(), 5);

    EXPECT_EQ(l[0], TestClass{1});
    EXPECT_EQ(l[1], TestClass{2});
    EXPECT_EQ(l[2], TestClass{3});
    EXPECT_EQ(l[3], TestClass{4});
    EXPECT_EQ(l[4], TestClass{5});
}

TEST_F(ListTest, MoveAssignmentClassTest)
{
    sd::List<TestClass> v = {{1}, {2}, {3}, {4}, {5}};
    sd::List<TestClass> l;
    l = std::move(v);

    EXPECT_TRUE(v.empty());
    EXPECT_EQ(l.size(), 5);

    EXPECT_EQ(l[0], TestClass{1});
    EXPECT_EQ(l[1], TestClass{2});
    EXPECT_EQ(l[2], TestClass{3});
    EXPECT_EQ(l[3], TestClass{4});
    EXPECT_EQ(l[4], TestClass{5});
}

TEST_F(ListTest, InitializerListAssignmentClassTest)
{
    sd::List<TestClass> l;
    l = {{1}, {2}, {3}, {4}, {5}};

    EXPECT_EQ(l.size(), 5);

    EXPECT_EQ(l[0], TestClass{1});
    EXPECT_EQ(l[1], TestClass{2});
    EXPECT_EQ(l[2], TestClass{3});
    EXPECT_EQ(l[3], TestClass{4});
    EXPECT_EQ(l[4], TestClass{5});
}

TEST_F(ListTest, ConstClassTest)
{
    const sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};

    EXPECT_EQ(l[0], TestClass{1});
    EXPECT_EQ(l[1], TestClass{2});
    EXPECT_EQ(l[2], TestClass{3});
    EXPECT_EQ(l[3], TestClass{4});
    EXPECT_EQ(l[4], TestClass{5});
}

TEST_F(ListTest, SwapClassTest)
{
    sd::List<TestClass> l = {{1}, {2}, {3}, {4}, {5}};
    sd::List<TestClass> l2 = {{11}, {12}, {13}, {14}, {15}};

    l.swap(l2);

    EXPECT_EQ(l2[0], TestClass{1});
    EXPECT_EQ(l2[1], TestClass{2});
    EXPECT_EQ(l2[2], TestClass{3});
    EXPECT_EQ(l2[3], TestClass{4});
    EXPECT_EQ(l2[4], TestClass{5});
    EXPECT_EQ(l2.size(), 5);

    EXPECT_EQ(l[0], TestClass{11});
    EXPECT_EQ(l[1], TestClass{12});
    EXPECT_EQ(l[2], TestClass{13});
    EXPECT_EQ(l[3], TestClass{14});
    EXPECT_EQ(l[4], TestClass{15});
    EXPECT_EQ(l.size(), 5);
}