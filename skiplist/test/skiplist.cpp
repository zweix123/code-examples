#include "skiplist.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <random>
#include <string>

class SkipListTest : public ::testing::Test {
  protected:
    SkipList<std::string, int> skip_list;

    void SetUp() override {
        // 在每个测试用例开始前执行
    }

    void TearDown() override {
        // 在每个测试用例结束后执行
    }
};

TEST_F(SkipListTest, InsertAndErase) {
    skip_list.insert("a", 1);
    skip_list.insert("b", 2);
    skip_list.insert("c", 3);

    EXPECT_EQ(skip_list.get_length(), 3);

    skip_list.erase("b", 2);
    EXPECT_EQ(skip_list.get_length(), 2);

    skip_list.erase("a", 1);
    EXPECT_EQ(skip_list.get_length(), 1);

    skip_list.erase("c", 3);
    EXPECT_EQ(skip_list.get_length(), 0);
}

TEST_F(SkipListTest, InsertDuplicateValues) {
    skip_list.insert("a", 1);
    skip_list.insert("b", 1);
    skip_list.insert("c", 1);

    EXPECT_EQ(skip_list.get_length(), 3);

    skip_list.erase("b", 1);
    EXPECT_EQ(skip_list.get_length(), 2);
}

TEST_F(SkipListTest, EraseRange) {
    for (int i = 0; i < 10; ++i) {
        skip_list.insert(std::to_string(i), i);
    }

    EXPECT_EQ(skip_list.get_length(), 10);

    skip_list.erase_range(2, 5);
    EXPECT_EQ(skip_list.get_length(), 6);
}

// TEST_F(SkipListTest, RangeQuery) {
//     for (int i = 0; i < 10; ++i) {
//         skip_list.insert(std::to_string(i), i);
//     }

//     auto result = skip_list.range(2, 7);
//     EXPECT_EQ(result.size(), 6);
//     EXPECT_EQ(result[0].first, "2");
//     EXPECT_EQ(result[5].first, "7");
// }

TEST_F(SkipListTest, LargeDataset) {
    const int N = 10000;
    std::vector<std::pair<std::string, int>> data;
    for (int i = 0; i < N; ++i) {
        data.emplace_back(std::to_string(i), i);
    }

    // 随机打乱数据
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(data.begin(), data.end(), g);

    // 插入数据
    for (const auto &[k, v] : data) {
        skip_list.insert(k, v);
    }

    EXPECT_EQ(skip_list.get_length(), N);

    // 随机删除一半的数据
    std::shuffle(data.begin(), data.end(), g);
    for (int i = 0; i < N / 2; ++i) {
        skip_list.erase(data[i].first, data[i].second);
    }

    EXPECT_EQ(skip_list.get_length(), N / 2);
}

TEST_F(SkipListTest, EdgeCases) {
    // 测试空跳表
    EXPECT_EQ(skip_list.get_length(), 0);

    // 测试删除不存在的元素
    skip_list.erase("nonexistent", 0);
    EXPECT_EQ(skip_list.get_length(), 0);

    // 测试范围删除边界情况
    skip_list.insert("a", 1);
    skip_list.erase_range(-1, 1); // 无效范围
    skip_list.debug();
    EXPECT_EQ(skip_list.get_length(), 1);
    skip_list.debug();
    skip_list.erase_range(0, 0); // 有效范围
    skip_list.debug();
    EXPECT_EQ(skip_list.get_length(), 0);
    skip_list.debug();
}

TEST_F(SkipListTest, PerformanceTest) {
    const int N = 100000;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < N; ++i) {
        skip_list.insert(std::to_string(i), i);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "插入" << N << "个元素耗时:" << duration.count() << "毫秒"
              << std::endl;

    EXPECT_EQ(skip_list.get_length(), N);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
