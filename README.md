# code-examples

项目根目录下（除了`test`目录）文件树结构与`test`目录下文件树结构一致，后者对应位置文件为前者的测试。


+ [哈希表](./HashTable/)
  + [一次探测开放寻址哈希表](./HashTable/OpenAddressing.h)

+ [Human Readable Printer](./ObjPrint/): 是head only的，其中部分代码和编译器版本和语言标准版本有关，根据自己需要将其放在单独的分开的代码中。如果您也使用，请选择这个部分。具体的，将`ObjPinrt.h`和适合自己场景的另外头文件，比如`to_string_CPP17.h`，一并放在自己项目的include目录下，并修改`ObjPrint.h`中include另一个头文件的代码，即可使用。效果见[测试](./test/ObjPrint/ObjPrint.cpp)
  + Ref: [C++ dbg macro](https://github.com/sharkdp/dbg-macro) | [Peng print](https://github.com/archibate/babyjson-demo/blob/master/print.h)
