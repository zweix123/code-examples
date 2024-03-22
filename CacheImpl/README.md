+ Ref：
  + [wiki](https://en.wikipedia.org/wiki/Cache_replacement_policies)

# Random

# Queue Based
## FIFO
顾名思义

# Recency Based

## LRU

+ 描述：想象一下，一个队列，有长度上限，对缓存值（一个键值对），会将其放在队首，当队列达到上限时排出队尾元素
+ 实现：肯定不能用一个队列实现，可以用哈希表和双向链表实现，哈希表维护键，其值是链表节点指针，然后拿着这个指针来实现直接移动到头部。当排出时，我们发现从队列去掉队尾是容易的。但是怎么更新哈希表呢？所以需要链表元素也要维护键。
