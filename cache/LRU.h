#pragma once

#include <cassert>
#include <list>
#include <optional>
#include <ostream>
#include <unordered_map>

#include "../common/exception.h"

template<typename K, typename V, typename Hash = std::hash<K>>
class LRU {
  public:
    explicit LRU(std::size_t cap) : cap_(cap) {
        if (cap == 0) {
            throw AssertException("容量必须大于0.");
        }
    };
    ~LRU() {
        while (!list_.empty()) {
            evict();
        }
    }

  private:
    void update(const K &k, V *vp) {
        if (dict_.contains(k)) {
            list_.erase(dict_[k]);
        }
        list_.push_front({k, vp});
        dict_[k] = list_.begin();
    }
    void evict() {
        const auto iterEnd = std::prev(std::end(list_));
        dict_.erase(iterEnd->k_);
        delete list_.back().vp_;
        list_.pop_back();
    }

  public:
    void put(const K &k, const V &v) {
        assert(dict_.size() == list_.size());
        V *nvp = new V(v);
        update(k, nvp);
        while (list_.size() > cap_) {
            evict();
        }
    }

    std::optional<V> get(const K &k) {
        assert(dict_.size() == list_.size());
        if (!dict_.contains(k)) {
            return std::nullopt;
        }
        V *vp = dict_[k]->vp_;
        update(k, vp);

        return *vp;
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
            out << "(" << iter->k_ << ", " << *iter->vp_ << ")";
        }
        out << "]";
        return out;
    }

  private:
    std::size_t cap_;
    struct Node {
        K k_;
        V *vp_; // 减小拷贝, 每次对LRU Cache的访问都会修改里面的元素,
                // 这里会有比较多的拷贝. 而不仅仅是get是的这一次基本的拷贝.
    };
    std::list<Node> list_;
    std::unordered_map<K, typename std::list<Node>::iterator, Hash> dict_;
};