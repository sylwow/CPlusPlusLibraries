#include <cstddef>
#include <format>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "MemoryManager.hpp"

struct ExampleClass
{
    ExampleClass *ptr = nullptr;
    std::vector<ExampleClass *> &destSet;

    ExampleClass(std::vector<ExampleClass *> &dest, ExampleClass *ptr = nullptr) : ptr(ptr), destSet(dest) {}
    ~ExampleClass() { destSet.push_back(this); }
};

class MemoryManagerTest : public ::testing::Test
{
  protected:
    static std::vector<ExampleClass *> &getDestructorSet()
    {
        static std::vector<ExampleClass *> ob;
        return ob;
    }

    void SetUp() override { getDestructorSet().clear(); }

    void TearDown() override { sd::MemoryManager::instance().garbageCollect(); }

    bool wasDestructed(std::vector<ExampleClass *> ptrs)
    {
        for (auto ptr : ptrs)
        {
            auto &v = getDestructorSet();
            if (std::find(v.begin(), v.end(), ptr) != v.end())
            {
                return true;
            }
        }
        return false;
    }

    int destructionCnt() { return getDestructorSet().size(); }

    ExampleClass *make(ExampleClass *ptr = nullptr) { return sd::make<ExampleClass>(getDestructorSet(), ptr); }
};

TEST_F(MemoryManagerTest, ManagerShouldAllocateObject)
{
    auto ptr = make();

    EXPECT_NE(nullptr, ptr);
    EXPECT_EQ(nullptr, ptr->ptr);
}

TEST_F(MemoryManagerTest, ManagerShouldCollectObjects)
{
    make();
    make();
    make();
    make();
    make();
    make();
    make();
    make();
    make();
    make();
    make();
    make();
    make();
    make();

    sd::MemoryManager::instance().garbageCollect();

    EXPECT_LE(1, destructionCnt());
}

TEST_F(MemoryManagerTest, ManagerShouldNotCollectObject)
{
    auto ptr1 = make();
    auto ptr2 = make();
    auto ptr3 = make();
    auto ptr4 = make();

    sd::MemoryManager::instance().garbageCollect();

    EXPECT_FALSE(wasDestructed({ptr1, ptr2, ptr3, ptr4}));
}

TEST_F(MemoryManagerTest, ManagerShouldCollectObjectsWithCircleReferences)
{
    {
        auto circle = make(nullptr);
        auto circle2 = make(circle);
        circle->ptr = circle2;

        circle = make(nullptr);
        circle2 = make(circle);
        circle->ptr = circle2;

        circle = make(nullptr);
        circle2 = make(circle);
        circle->ptr = circle2;

        circle = make(nullptr);
        circle2 = make(circle);
        circle->ptr = circle2;

        circle = make(nullptr);
        circle2 = make(circle);
        circle->ptr = circle2;
    }

    sd::MemoryManager::instance().garbageCollect();

    EXPECT_LE(1, destructionCnt());
}

TEST_F(MemoryManagerTest, ManagerShouldCollectSomeObjects)
{
    auto circle = make(nullptr);
    auto circle2 = make(circle);
    circle->ptr = circle2;
    {

        auto toRemove = make(nullptr);
        auto toRemove2 = make(toRemove);
        toRemove->ptr = toRemove2;

        toRemove = make(nullptr);
        toRemove2 = make(toRemove);
        toRemove->ptr = toRemove2;

        toRemove = make(nullptr);
        toRemove2 = make(toRemove);
        toRemove->ptr = toRemove2;
    }

    sd::MemoryManager::instance().garbageCollect();

    EXPECT_FALSE(wasDestructed({circle, circle2}));
    EXPECT_LE(1, destructionCnt());
}

TEST_F(MemoryManagerTest, ManagerShouldNotCollectAutomaticallySomeObjects)
{
    auto limit = 500 * 1024 / sizeof(ExampleClass);
    for (int i = 0; i < limit; i++) // allocate ~0.5 MB
    {
        make();
    }

    EXPECT_EQ(0, destructionCnt());
}

TEST_F(MemoryManagerTest, ManagerShouldCollectAutomaticallySomeObjects)
{
    auto limit = 1500 * 1024 / sizeof(ExampleClass);
    for (int i = 0; i < limit; i++) // allocate ~1.5 MB
    {
        make();
    }

    EXPECT_LE(1, destructionCnt());
}

TEST_F(MemoryManagerTest, ManagersShouldWorkInSeparateThreads)
{
    auto mainThreadOb = make();

    auto limit = 1500 * 1024 / sizeof(ExampleClass);

    auto runner = [limit](std::vector<ExampleClass *> &destructor) {
        for (int i = 0; i < limit; i++)
        {
            sd::make<ExampleClass>(destructor);
        }
        EXPECT_LE(1, destructor.size());
    };

    std::vector<ExampleClass *> destructorR1;
    std::vector<ExampleClass *> destructorR2;

    std::jthread r1{runner, std::ref(destructorR1)};
    std::jthread r2{runner, std::ref(destructorR2)};

    r1.join();
    r2.join();
    EXPECT_FALSE(wasDestructed({mainThreadOb}));
    EXPECT_EQ(limit, destructorR1.size());
    EXPECT_EQ(limit, destructorR2.size());
    EXPECT_EQ(0, destructionCnt());
}