#include "OpenAddressing.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <random>
#include <thread>
#include <unordered_set>
#include <vector>

#include <gtest/gtest.h>

template<typename T>
using Set = HashTable<T>;

template<typename K, typename V>
using Map = HashTable<std::pair<K, V>, std::hash<K>>;

TEST(OpenAddressingTest, BasicOperations) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100, 100);
    auto random = [&dis, &gen]() {
        return dis(gen);
    };

    auto my_ht = Set<int>();
    auto std_ht = std::unordered_set<int>();

    for (int i = 0; i < 100; ++i) {
        auto value = random();

        my_ht.insert(value);
        std_ht.insert(value);

        for (int j = -100; j <= 100; ++j) {
            EXPECT_EQ(my_ht.contains(j), std_ht.contains(j));
        }
    }

    for (int i = 0; i < 100; ++i) {
        auto value = random();

        my_ht.erase(value);
        std_ht.erase(value);

        for (int j = -100; j <= 100; ++j) {
            EXPECT_EQ(my_ht.contains(j), std_ht.contains(j));
        }
    }
}

TEST(OpenAddressingTest, BasicOperations_string) {
    constexpr char const *const CHARS = "abcdefghijklmnopqrstuvwxyz"
                                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                        "0123456789";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_length(0, 10);
    std::uniform_int_distribution<> dis_index(0, strlen(CHARS));
    auto random = [&gen, &dis_length, &dis_index, &CHARS]() {
        int lenght = dis_length(gen);
        std::string res;
        for (int i = 0; i < lenght; ++i) res += CHARS[dis_index(gen)];
        return res;
    };
    auto sample = [&gen]<typename T>(const std::vector<T> &vec) { // C++20
        std::uniform_int_distribution<> dis(0, vec.size());
        return vec[dis(gen)];
    };

    auto my_ht = Set<std::string>();
    auto std_ht = std::unordered_set<std::string>();

    std::vector<std::string> values;

    for (int i = 0; i < 100; ++i) {
        auto value = random();
        values.push_back(value);

        my_ht.insert(value);
        std_ht.insert(value);

        for (int j = 0; j < 50; ++j) {
            auto value = random();
            EXPECT_EQ(my_ht.contains(value), std_ht.contains(value));
        }
        for (const auto &value : values) {
            EXPECT_EQ(my_ht.contains(value), std_ht.contains(value));
        }
    }

    {
        auto rng = std::default_random_engine{};
        std::shuffle(values.begin(), values.end(), rng);
    }

    for (int i = 0; i < 50; ++i) {
        auto value = random();

        my_ht.erase(value);
        std_ht.erase(value);

        for (int j = 0; j < 50; ++j) {
            auto value = random();
            EXPECT_EQ(my_ht.contains(value), std_ht.contains(value));
        }
        for (const auto &value : values) {
            EXPECT_EQ(my_ht.contains(value), std_ht.contains(value));
        }
    }

    for (int i = 0; i < 50; ++i) {
        auto value = std::move(values.back());
        values.pop_back();

        my_ht.erase(value);
        std_ht.erase(value);

        for (int j = 0; j < 50; ++j) {
            auto value = random();
            EXPECT_EQ(my_ht.contains(value), std_ht.contains(value));
        }
        for (const auto &value : values) {
            EXPECT_EQ(my_ht.contains(value), std_ht.contains(value));
        }
    }
}

TEST(OpenAddressingTest, MultiThread) {
    const int THREAD_COUNT = 10;
    const int INSERT_COUNT = 100000;
    const int FIND_COUNT = 100000;

    HashTable<int> ht;
    std::vector<std::thread> threads;

    for (int i = 0; i < THREAD_COUNT; ++i) {
        threads.emplace_back([&]() {
            for (int j = 0; j < INSERT_COUNT; ++j) { ht.insert(j); }
        });
    }

    for (auto &t : threads) { t.join(); }

    threads.clear();
    for (int i = 0; i < THREAD_COUNT; ++i) {
        threads.emplace_back([&]() {
            for (int j = 0; j < FIND_COUNT; ++j) { ht.contains(j); }
        });
    }

    for (auto &t : threads) { t.join(); }
}
