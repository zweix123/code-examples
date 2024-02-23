#include "OpenAddressing.h"
#include <cassert>
#include <thread>
#include <vector>

int main() {
    {
        auto ht = HashTable<int>();

        ht.insert(1);

        assert(ht.contain(1));

        assert(!ht.contain(2));
        ht.insert(2);
        assert(ht.contain(2));

        std::vector<int> dummy = {3, 42};
        for (auto ele : dummy) { ht.insert(ele); }
        for (auto ele : dummy) { assert(ht.contain(ele)); }

        assert(!ht.contain(6));
        for (auto ele : dummy) {
            ht.erase(ele);
            assert(!ht.contain(ele));
        }
    }
    {
        HashTable<int> ht;
        assert(ht.contain(1) == false);
        assert(ht.contain(2) == false);
        assert(ht.contain(3) == false);

        ht.insert(1);
        ht.insert(2);
        ht.insert(3);

        ht.erase(2);

        assert(ht.contain(1) == true);
        assert(ht.contain(2) == false);
    }
    {
        HashTable<std::string> ht;
        assert(ht.contain("hello") == false);
        assert(ht.contain("world") == false);

        ht.insert("hello");
        ht.insert("world");
    }
    {
        const int THREAD_COUNT = 10;
        const int INSERT_COUNT = 100000;
        const int FIND_COUNT = 100000;

        HashTable<int> ht;
        std::vector<std::thread> threads;

        // 创建多个线程并发执行插入操作
        for (int i = 0; i < THREAD_COUNT; ++i) {
            threads.emplace_back([&]() {
                for (int j = 0; j < INSERT_COUNT; ++j) { ht.insert(j); }
            });
        }

        // 等待所有线程执行完成
        for (auto &t : threads) { t.join(); }

        // 创建多个线程并发执行查找操作
        threads.clear();
        for (int i = 0; i < THREAD_COUNT; ++i) {
            threads.emplace_back([&]() {
                for (int j = 0; j < FIND_COUNT; ++j) { ht.contain(j); }
            });
        }

        // 等待所有线程执行完成
        for (auto &t : threads) { t.join(); }
    }

    return 0;
}