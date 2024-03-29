/*
+ Ref:
    + [CoolShell · C++ 虚函数表解析](https://coolshell.cn/articles/12165.html)
      > 不知道这个网站还能存在多久
*/

#include "ObjPrint.h"
#include <iostream>

class Base {
  public:
    virtual void f() { std::cout << "Base::f" << std::endl; }
    virtual void g() { std::cout << "Base::g" << std::endl; }
    virtual void h() { std::cout << "Base::h" << std::endl; }
};

void v_table_position() {
    using Fun = void (*)(void);
    Base b;
    println(sizeof(Base));
    println(sizeof(b));
    // println((int *)(&b));
    // println((int *)*(int *)(&b));
    // Fun pF = (Fun) * ((int *)*(int *)(&b));
    // pF();
    /*
预期pF()会输出Base::f，但这是UB

在C++实现虚函数的技术中

有虚函数表, v_table, 其指针在类实例在开始的那个字节
    */
}

int main() {
    v_table_position();
}

/*
在继承中，会将虚函数表继承，不是指针其指针，是完整拷贝一份表

派生类添加新的虚函数则是在该表中添加

如果派生类覆盖基类虚函数则是在该表中修改对应函数

多基类派生呢？派生类会有多个虚表指针
*/
