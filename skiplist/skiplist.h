/*
以论文和Redis为基础, 使用现在C++实现skip list

+ 跳表: 有序链表(这里使用升序), 接口同平衡树(按值有序)
+ 论文:
  William Pugh
  "Skip Lists: A Probabilistic Alternative to Balanced Trees"
+ Redis的实现与论文的不同
  + 为每个节点添加类似prev的指针, 用于逆序遍历
  + 允许相同的键, 键是char*, 值相同时按照键字典序有序
*/
#pragma once
#include <array>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

// copy from redis
const int ZSKIPLIST_MAXLEVEL = 32; /* Should be enough for 2^64 elements */
// const double ZSKIPLIST_P = 0.25;   /* Skiplist P = 1/4 */
const double ZSKIPLIST_P = 0.5; /* Skiplist P = 1/4 */

inline std::mt19937_64
    rnd(std::chrono::steady_clock::now().time_since_epoch().count());
const int RND_MAX = 114514;

// zslRandomLevel
inline int random_level() {
    static const int threshold = ZSKIPLIST_P * RND_MAX;
    // 这里使用random()函数所能返回的最大值的四分之一为阈值
    int level = 1;
    while ((rnd() % RND_MAX) < threshold) level += 1;
    // 假如随机的数字小于则递增并继续随机, 直到大于等于阈值
    return (level < ZSKIPLIST_MAXLEVEL) ? level : ZSKIPLIST_MAXLEVEL;
    // 所以越低的层级概率约大
    // level = 1: 3/4
    // level = 2: 1/4 * 3/4 = 3 / 16
    // level = 3: 1/4 * 1/4 * 3/4 = 3 / 64
    // ...
    // 可以想象, 当改变ZSKIPLIST_P, 可以调整跳表的"密度"
}

// redis: zskiplist
template<typename K, typename V>
class SkipList {
  private:
    // redis: zskiplistNode
    struct Node {
        // redis: zskiplistLevel
        struct Level {
            Node *forward;
            unsigned int
                span; // 跨度:
                      // 表示在这一层中当前节点到下一个节点要跨过的节点个数

            Level();
        };

        K k; // redis: char* sds;
        V v; // redis: double score;
        Node *backward;
        std::vector<Level> level;

        // redis: zslCreateNode
        Node(K k, V v, int level_count);
    };
    Node *header;
    Node *tail;
    unsigned int length;
    int level; // 除了header节点以外的所有节点的最大值
               // header的层级取ZSKIPLIST_MAXLEVEL
  private:
    // zslDeleteNode
    void
    erase(const std::array<Node *, ZSKIPLIST_MAXLEVEL> &update, Node *node);

  public:
    // redis: zslCreate
    SkipList();
    // redis: zslFree
    ~SkipList();

    // zslInsert
    void insert(K k, V v);
    // zslDelete
    void erase(K k, V v); // 由于允许值相同, 需要删除需要指定键和值
    // zslDeleteByRank, 没有这个, 但是有zslDeleteRangeByRank
    void erase_range(int start, int end);

  public:
    unsigned int get_length() const { return length; }

  public:
    void debug() const; // print and check
};

template<typename K, typename V>
SkipList<K, V>::Node::Level::Level() : forward(nullptr), span(0) {
}

template<typename K, typename V>
SkipList<K, V>::Node::Node(K k, V v, int level_count)
    : k(k), v(v), backward(nullptr), level(level_count) {
}

template<typename K, typename V>
SkipList<K, V>::SkipList()
    : header(new Node(K(), V(), ZSKIPLIST_MAXLEVEL))
    , tail(nullptr) // skip list使用虚拟header的方式, 此时没有有意义的节点,
                    // 故tail是空, 当插入之后, 就是实际的节点了
    , length(0)
    , level(1) {
}

template<typename K, typename V>
SkipList<K, V>::~SkipList() {
}

template<typename K, typename V>
void SkipList<K, V>::insert(K k, V v) {
    std::array<Node *, ZSKIPLIST_MAXLEVEL> update;
    std::fill(update.begin(), update.end(), nullptr);
    std::array<int, ZSKIPLIST_MAXLEVEL> rank;
    std::fill(rank.begin(), rank.end(), 0);

    auto node = header;
    for (int i = level - 1; i >= 0; --i) {
        if (i == level - 1) {
            rank[i] = 0;
        } else {
            rank[i] = rank[i + 1];
        }

        while (node->level[i].forward != nullptr
               && (node->level[i].forward->v < v
                   || (node->level[i].forward->v == v
                       && node->level[i].forward->k < k))) {
            rank[i] += node->level[i].span;
            node = node->level[i].forward;
        }
        update[i] = node;
    }

    const auto rl = random_level();
    if (rl > level) {
        for (int i = level; i < rl; ++i) {
            rank[i] = 0;
            update[i] = header;
            update[i]->level[i].span = length;
        }
        level = rl;
    }

    auto new_node = new Node(k, v, rl);

    for (int i = 0; i < rl; ++i) {
        new_node->level[i].forward = update[i]->level[i].forward;
        update[i]->level[i].forward = new_node;

        //           (rank[0] - rank[i])
        //                |        |             |
        //                |        |    等于1    |
        //                |        |             |
        //                |        |    |new     |old
        //                |        |    |        |
        // xxxxxxxxxxxxxxx@        |    |        |
        //                x        |    |        |
        //                xxxxx@   |    |        |
        //                |    x   |    |        |
        //                |    xxxx|    |        |
        //                |       x@    |        |
        //                |        x    |        |
        //                |        @    |        |
        //                |             |        |
        //                |                      |
        //                update[i]->level[i].span
        new_node->level[i].span =
            update[i]->level[i].span - (rank[0] - rank[i]);
        update[i]->level[i].span = (rank[0] - rank[i]) + 1;
    }

    for (int i = rl; i < level; ++i) {
        update[i]->level[i].span++;
    }

    // 新节点的prev
    if (update[0] == header) {
        new_node->backward = nullptr;
    } else {
        new_node->backward = update[0];
    }

    // 新节点的next的prev
    if (new_node->level[0].forward != nullptr) {
        new_node->level[0].forward->backward = new_node;
    } else {
        tail = new_node;
    }

    ++length;
}

template<typename K, typename V>
void SkipList<K, V>::erase(
    const std::array<Node *, ZSKIPLIST_MAXLEVEL> &update, Node *node) {
    for (int i = 0; i < level; ++i) {
        if (update[i]->level[i].forward == node) {
            update[i]->level[i].span += node->level[i].span - 1;
            update[i]->level[i].forward = node->level[i].forward;
        } else {
            update[i]->level[i].span -= 1;
        }
    }

    if (node->level[0].forward != nullptr) {
        node->level[0].forward->backward = node->backward;
    } else {                  // node就是最后一个
        assert(tail == node); // 防御性编程一波
        tail = node->backward;
    }

    while (level > 1 && header->level[level - 1].forward == nullptr) {
        --level;
    }

    delete node;
    --length;
}

template<typename K, typename V>
void SkipList<K, V>::erase(K k, V v) {
    std::array<Node *, ZSKIPLIST_MAXLEVEL> update;
    std::fill(update.begin(), update.end(), nullptr);

    auto node = header;
    for (int i = level - 1; i >= 0; --i) {
        while (node->level[i].forward != nullptr
               && (node->level[i].forward->v < v
                   || (node->level[i].forward->v == v
                       && node->level[i].forward->k < k))) {
            node = node->level[i].forward;
        }
        update[i] = node;
    }

    node = node->level[0].forward;
    if (node != nullptr && node->v == v && node->k == k) {
        erase(update, node);
    }
}

template<typename K, typename V>
void SkipList<K, V>::erase_range(int start, int end) {
    if (start < 0 || start >= length || end < 0 || end >= length
        || start > end) {
        return;
    }

    std::array<Node *, ZSKIPLIST_MAXLEVEL> update;
    std::fill(update.begin(), update.end(), nullptr);

    unsigned int traversed = 0;

    auto node = header;
    for (int i = level - 1; i >= 0; --i) {
        while (node->level[i].forward != nullptr
               && (traversed + node->level[i].span < start)) {
            traversed += node->level[i].span;
            node = node->level[i].forward;
        }
        update[i] = node;
    }

    if (node != header) {
        traversed++;
    }
    node = node->level[0].forward;
    while (node != nullptr && traversed <= end) {
        auto next = node->level[0].forward;
        erase(update, node);
        ++traversed;
        node = next;
    }
}

template<typename K, typename V>
void SkipList<K, V>::debug() const {
    std::vector<Node *> nodes;
    std::vector<std::string> strings;
    nodes.reserve(length);
    strings.reserve(length);

    auto node2string = [](Node *node) -> std::string {
        std::stringstream ss;
        ss << "[" << node->k << " " << node->v << "]";
        return ss.str();
    };

    int width = 0;

    for (auto node = header->level[0].forward; node != nullptr;
         node = node->level[0].forward) {
        nodes.push_back(node);
        strings.push_back(node2string(node));
        width += strings.back().size();
    }

    assert(nodes.size() == strings.size());
    const auto n = nodes.size();

    width += n + 20; // 5 + 9 + 6 = 20
    static const std::string title = "skip list";
    assert(width > title.size());

    std::cout << std::string(width / 2 - title.size() / 2, '=') << title
              << std::string(
                     width - (width / 2 - title.size() / 2) - title.size(), '=')
              << std::endl;

    for (int l = level - 1; l >= 0; --l) {
        std::cout << "[" << std::setw(2) << std::setfill(' ') << l << "] ";
        std::cout << "[header]-";

        std::vector<std::pair<int, Node *>>
            current_level_nodes; // [index, node]
        for (int i = 0; i < n; ++i) {
            if (l < nodes[i]->level.size()) { // 该节点在该层有节点
                std::cout << strings[i] << "-";

                if (!current_level_nodes.empty()) {
                    const auto &[index, node] = current_level_nodes.back();
                    assert(nodes[index]->level[l].span == i - index);
                    assert(node->level[l].backward = nodes[i]);
                    assert(nodes[i]->level[l].forward = node);
                }
                current_level_nodes.push_back({i, nodes[i]});
            } else {
                std::cout << std::string(strings[i].size(), '-') << "-";
            }
        }

        std::cout << "[null]" << std::endl;
    }

    std::cout << std::string(width, '=') << std::endl;
}
