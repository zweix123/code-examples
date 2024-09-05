// Linear Probing
#pragma once

#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

template<typename T, typename Hasher = std::hash<T>>
class HashTable {
    using wlock = std::unique_lock<std::shared_mutex>;
    using rlock = std::shared_lock<std::shared_mutex>;

    enum class BucketType { Empty, Tombstone };
    using EntryType = std::variant<BucketType, T>;

    constexpr static const double FACTOR = 0.75;
    constexpr static const int MULIPLE = 2;

  public:
    explicit HashTable() : data_(size_, BucketType::Empty), hasher_(Hasher()) {}
    HashTable(const HashTable<T, Hasher> &that) = default;
    HashTable(HashTable<T, Hasher> &&that) = default;
    HashTable<T, Hasher> &operator=(const HashTable<T, Hasher> &that) = default;
    HashTable<T, Hasher> &operator=(HashTable<T, Hasher> &&that) = default;

  private:
    EntryType *find(const T &value) {
        std::size_t hashed = hasher_(value);
        std::size_t index = hashed % data_.size();
        EntryType *tombstone = nullptr;
        while (true) {
            auto now = std::get_if<BucketType>(&data_[index]);
            if (now != nullptr) {
                // 枚举
                if (*now == BucketType::Empty) {
                    // 如果真的是空, 说明后面确实没可能有和当前哈希值一样的桶了
                    return tombstone != nullptr ? tombstone : &data_[index];
                } else if (*now == BucketType::Tombstone) {
                    // 遇到墓碑了, 将其存起来(只存第一个), 继续找
                    if (tombstone == nullptr) {
                        tombstone = &data_[index];
                    }
                }
            } else {
                // 具体的值, 找到有效的值了, 检测并返回
                auto p = std::get_if<T>(&data_[index]);
                if (*p == value) {
                    return &data_[index];
                }
            }
            ++index;
            if (index == data_.size()) index = 0;
        }
        assert(false); // Beak of Dead Loop and not Return
    }
    void adjust() {
        std::vector<EntryType> temp_dummy;
        std::swap(data_, temp_dummy);
        data_.resize(temp_dummy.size() * MULIPLE);
        for (auto ele : temp_dummy) {
            auto p = std::get_if<T>(&ele);
            if (p != nullptr) {
                auto entry = find(*p);
                *entry = *p;
            }
        }
    }

  public:
    bool contains(const T &value) {
        rlock lock(mutex_);
        auto *entry = find(value);
        return std::get_if<T>(entry) != nullptr;
    }
    void insert(const T &value) {
        wlock lock(mutex_);
        if (1.0 * (size_ + 1) / data_.size() > FACTOR) {
            adjust();
        }
        auto *entry = find(value);
        *entry = value;
        ++size_;
    }
    void erase(const T &value) {
        wlock lock(mutex_);
        auto *entry = find(value);
        if (std::get_if<T>(entry) != nullptr) {
            // 该桶的值确实是T类型, 那说明确实找到了
            *entry = BucketType::Tombstone;
            --size_;
        }
    }

  private:
    Hasher hasher_{std::hash<T>{}};
    int size_{8};
    std::vector<EntryType> data_;
    mutable std::shared_mutex mutex_;

  public:
    std::string ToString() const {
        std::stringstream ss;
        ss << "HashTable(";
        int flag = false;
        for (auto &ele : data_) {
            if (!flag) {
                flag = true;
            } else {
                ss << ", ";
            }
            auto p = std::get_if<BucketType>(&ele);
            if (p != nullptr) {
                if (*p == BucketType::Empty) {
                    ss << "Empty";
                } else {
                    assert(*p == BucketType::Tombstone);
                    ss << "TombStone";
                }
            } else {
                auto p = std::get_if<T>(&ele);
                ss << *p;
            }
        }
        ss << "|" << size_ << "/" << data_.size() << ")";
        return ss.str();
    }
    friend std::ostream &
    operator<<(std::ostream &out, const HashTable<T, Hasher> &ht) {
        return out << ht.ToString();
    }
};
static const double FACTOR = 0.75;
static const int MULIPLE = 2;
