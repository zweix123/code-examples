/*
+ Ref:
    + [只为那传说中美丽的草原 ·
C++11中的lambda，std::function以及std:bind](https://paul.pub/cpp-lambda-function-bind/)
*/

#include "obj_print.h"

int main() {
    auto func_no_cap = [](int a, int b) {
        return a + b;
    };
    println(sizeof(func_no_cap));
    /*
> sizeof(func_no_cap) = 1

相当于空类
```cpp
class _xxx_ {
  public:
    int operator() (int a, int b) const {
        return a + b;
    }
};
```
空类实例的大小自然是1字节
    */
    int x = 0;
    auto func_cap = [&x](int a, int b) {
        return x + a + b;
    };
    println(sizeof(func_cap));
    /*
> sizeof(func_cap) = 8

同样合理, 相当于
```cpp
class _xxx_ {
  public:
    _xxx_(int &x): x_{x} {}
    int operator(int a, int b) {
        return x + a + b;
    }
  private:
    int& x;
};
```
    */
}
