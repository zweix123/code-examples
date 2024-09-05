#include "obj_print.h"
#include <cassert>

class EmptyClass {};
class HasIntClass {
    int a;
};

void instance_empty_class() {
    println(sizeof(EmptyClass));
    println(sizeof(int));
    println(sizeof(HasIntClass));
    /*
sizeof(EmptyClass) = 1
sizeof(int) = 4
sizeof(HasIntClass) = 4

C++标准规定空类实例**至少**一个字节，用于区分该类的不同实例

但是并不意味着“这一个字节”任何类都是有的，编译器可以优化
    */
    EmptyClass a, b;
    assert(&a != &b);
    EmptyClass *pa = new EmptyClass();
    EmptyClass *pb = new EmptyClass();
    assert(pa != pb);
    delete pa;
    delete pb;
}

class BaseClass {
  public:
    void func(){};
};
class DerivedClass : BaseClass {
  public:
    void func(){};
};

class HasVirFuncBaseClass {
  public:
    virtual void func(){};
};
class HasVirFuncDerivedClass : HasVirFuncBaseClass {
  public:
    void func() override{};
};

void instance_inherit() {
    println(sizeof(BaseClass));
    println(sizeof(DerivedClass));
    println(sizeof(HasVirFuncBaseClass));
    println(sizeof(HasVirFuncDerivedClass));
    /*
sizeof(BaseClass) = 1
sizeof(DerivedClass) = 1
sizeof(HasVirFuncBaseClass) = 8
sizeof(HasVirFuncDerivedClass) = 8
后面是虚表指针，同样优化只保留指针
    */
}

int main() {
    instance_empty_class();
    instance_inherit();
}