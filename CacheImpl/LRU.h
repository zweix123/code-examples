#pragma once

#include <cassert>
#include <list>
#include <optional>
#include <ostream>
#include <unordered_map>

#include "../common/Exception.h"

template<typename K, typename V, typename Hash = std::hash<K>>
class LRU {
  public:
    explicit LRU(std::size_t cap) : cap_(cap) {
        if (cap == 0) { throw AssertException("容量必须大于0."); }
    };

  private:
    void update(const K &k, const V &v) {
        if (dict_.contains(k)) { list_.erase(dict_[k]); }
        list_.push_front({k, v});
        dict_[k] = list_.begin();
    }
    void evict() {
        const auto iterEnd = std::prev(std::end(list_));
        dict_.erase(iterEnd->k_);
        list_.pop_back();
    }

  public:
    void put(const K &k, const V &v) {
        assert(dict_.size() == list_.size());
        update(k, v);
        while (list_.size() > cap_) { evict(); }
    }

    std::optional<V> get(const K &k) {
        assert(dict_.size() == list_.size());
        if (!dict_.contains(k)) { return std::nullopt; }
        std::optional<V> v = dict_[k]->v_;
        update(k, v.value());
        return v;
    }

    friend std::ostream &operator<<(std::ostream &out, const LRU<K, V> &lru) {
        assert(lru.dict_.size() == lru.list_.size());
        out << "[";
        bool first = true;
        for (auto iter = lru.list_.begin(); iter != lru.list_.end(); ++iter) {
            assert(lru.dict_.at(iter->k_) == iter);
            if (first) first = false;
            else
                out << " | ";
            out << "(" << iter->k_ << ", " << iter->v_ << ")";
        }
        out << "]";
        return out;
    }

  private:
    std::size_t cap_;
    struct Node {
        K k_;
        V v_;
    };
    std::list<Node> list_;
    std::unordered_map<K, typename std::list<Node>::iterator, Hash> dict_;
};