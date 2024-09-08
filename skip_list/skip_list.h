/*
impl redis skip list in modern C++

+ skip list from William Pugh's paper
  "Skip Lists: A Probabilistic Alternative to Balanced Trees"
  https://www.cl.cam.ac.uk/teaching/0506/Algorithms/skiplists.pdf
+ wiki: https://en.wikipedia.org/wiki/Skip_list
  看完wiki的图片就知道了, 其节点仍然是只有最底层的链接,
  只是没有节点里面有层级列表, 然后指向对应的节点(也是最底层的节点).
*/
#pragma once
#include <tuple>
#include <vector>

// copy from redis
const int ZSKIPLIST_MAXLEVEL = 32; /* Should be enough for 2^64 elements */
const double ZSKIPLIST_P = 0.25;   /* Skiplist P = 1/4 */
const int ZSKIPLIST_MAX_SEARCH = 10;

template<typename K, typename V>
class SkipList { // zskiplist
  private:
    struct Node {
        struct Level {
            Node *next;       // redis: forward
            unsigned int num; // redis: span, 跨度, ...TODO

            Level() : next(nullptr), num(0) {}
        };

        K k;        // redis: sds ele;typedef char *sds;
        V v;        // redis: double score;
        Node *back; // redis: backward, 即当前节点的前一个, 用于逆序遍历
        std::vector<Level> level;

        Node(K k, V v, int level_count) // zslCreateNode
            : k(k), v(v), back(nullptr), level(level_count) {}
    };
    Node *head; // redis: header
    Node *tail; // redis: tail
    unsigned int length;
    int level; // 目前表内节点的最大层数, 使用int是因为作为std::vector的size
  public:
    SkipList() // zslCreate
        : head(new Node(K{}, V{}, ZSKIPLIST_MAXLEVEL))
        , tail(nullptr)
        , length(0)
        , level(1) {
        // head进行初始化, 键和值并不关键, 但是level是拉满的
        // 进一步初始化Node, back是nil, level是参数
        // 进一步初始化Level, 都是零值
        // tail是空
        // 虽然head的level是满的, 但是skiplist里的level是1
        // > 也合理, head要保证兼容所有的level
    }
    ~SkipList(); // zslFree

    void insert(K k, V v); // zslInsert
    void
    remove(K k, V v); // zslDelete/zslDeleteNode/zslDeleteRangeByScore,
                      // 为什么删除需要value? 因为redis允许不同score的相同member
    void remove(int rank); // zslDeleteRangeByRank
    int get(K k, V v);     // zslGetRank/zslGetElementByRank
    std::vector<std::tuple<K, V>>
    get(K start_key,
        V start_value,
        K end_key,
        V end_value); // zslFirstInRange/zslLastInRange
};
