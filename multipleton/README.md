# multipleton

怎么让单例模式实现restart?

比如为[这个典型的单例模式](https://refactoring.guru/design-patterns/singleton/go/example#example-1)的实现添加一个接口, 使之当调用之后, 后面再调用getInstance会重新走到`singleInstance = &single`代码.
