# code-examples
用来放置一些小Demo

项目根目录下除`test`目录之外的文件树结构在`test`目录下都有完全一致的，后者为对应位置文件的测试。
通常每个Demo目录下会有`README.md`文件用来说明，故下面只提供索引或者简介。

+ [哈希表](./HashTable/)
  + [一次探测开放寻址哈希表](./HashTable/OpenAddressing.h)

+ [Human Readable Printer](./ObjPrint/)

+ [缓存替换策略](./CacheImpl/)
  + [LRU](./CacheImpl/LRU.h)
  + [LFU](./CacheImpl/LFU.h)
  + [LRU-k](./CacheImpl/README.md#lru-k)

## Talk is cheap. Show me the code.

### 空类实例大小和虚表

+ [sizeof_class.cpp](./example/sizeof_class.cpp)
+ [virtual_function.cpp](./example/virtual_function.cpp)

### 编译器是怎么处理lambda的呢？

+ [lambda_impl.cpp](./example/lambda_impl.cpp)
