// #pragma once

#include "../common/Exception.h"
#include <cassert>
#include <list>
#include <optional>
#include <ostream>
#include <unordered_map>
#include <unordered_set>

template<typename K, typename V, typename Hash = std::hash<K>>
class LFU {
  public:
    explicit LFU(std::size_t cap) : cap_{cap}, minFreq_{1} {
        if (cap == 0) {
            throw AssertException("容量必须大于0.");
        }
    }

  private:
    void update_min_freq() {
        minFreq_ = 1000000;
        for (const auto &[k, _] : freqDict_) {
            minFreq_ = std::min(minFreq_, k);
        }
    }
    void update(const K &k, const V &v, unsigned int freq) {
        if (keyDict_.contains(k)) {
            const auto iter = keyDict_[k];
            freqDict_[freq].erase(iter);
            if (freqDict_[freq].empty()) {
                freqDict_.erase(freq);
                ++minFreq_;
            }
            keyDict_.erase(k);
        }
        ++freq;
        minFreq_ = std::min(minFreq_, freq);
        freqDict_[freq].push_front({k, v, freq});
        keyDict_[k] = freqDict_[freq].begin();
    }
    void evict() {
        assert(!freqDict_[minFreq_].empty());
        const auto iterEnd = std::prev(std::end(freqDict_[minFreq_]));
        keyDict_.erase(iterEnd->k_);
        freqDict_[minFreq_].pop_back();
        if (freqDict_[minFreq_].empty()) {
            ++minFreq_;
        }
    }

  public:
    void put(const K &k, const V &v) {
        if (keyDict_.contains(k)) {
            // 在缓存中，更新
            update(k, v, keyDict_[k]->freq_);
        } else {
            // 不在缓存中，
            if (keyDict_.size() == cap_) {
                // 达到上限，需要淘汰
                evict();
                update(k, v, 0);
            } else {
                update(k, v, 0);
                // 不需要淘汰，直接插入
            }
        }
    }
    std::optional<V> get(const K &k) {
        if (!keyDict_.contains(k)) {
            return std::nullopt;
        }
        std::optional<V> v = keyDict_[k]->v_;
        unsigned int freq = keyDict_[k]->freq_;

        update(k, v.value(), freq);
        return v;
    }

    friend std::ostream &operator<<(std::ostream &out, const LFU &lfu) {
        for (const auto &[k, _] : lfu.freqDict_) {
            assert(k >= lfu.minFreq_);
        }
        out << "LFU(";
        out << "MinFreq = " << lfu.minFreq_ << std::endl;
        std::unordered_set<K> keyTemp;
        bool firstLine = true;
        for (const auto &[k, v] : lfu.freqDict_) {
            if (firstLine) firstLine = false;
            else
                out << std::endl;
            out << "  frep = " << k << ": ";
            bool firstEle = true;
            for (const auto &ele : v) {
                if (firstEle) firstEle = false;
                else
                    out << ", ";
                assert(ele.freq_ == k);
                assert(!keyTemp.contains(ele.k_));
                assert(lfu.keyDict_.contains(ele.k_));
                keyTemp.insert(ele.k_);
                out << "(" << ele.k_ << ", " << ele.v_ << ")";
            }
        }
        assert(keyTemp.size() == lfu.keyDict_.size());
        out << ")";
        return out;
    }

  private:
    std::size_t cap_;
    struct Node {
        K k_;
        V v_;
        unsigned int freq_;
    };
    std::unordered_map<K, typename std::list<Node>::iterator, Hash> keyDict_;
    std::unordered_map<unsigned int, std::list<Node>> freqDict_;
    unsigned int minFreq_;
};