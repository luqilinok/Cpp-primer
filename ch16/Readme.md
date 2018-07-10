ex16_01

当调用一个函数模板时，编译器会利用给定的函数实参来推断模板实参，用此实际实参代替模板参数来创建出一个新的“实例”，也就是一个真正可以调用的函数，这个过程称为实例化

ex16_03

在gcc-4.8.1中，对两个Sales_data对象调用compare函数模板，编译器会报告如下错误。原因是compare是用<运算符来比较两个对象的，需要类型T实现定义<运算符，但是Sales_data类没有定义<运算符，所以会报错
error:no match for 'operator<'(operand types are 'const Sales_data') and 'const Sales_data'

ex16_06

begin应返回数组首元素的指针，因此是return &a[0],end返回尾后指针，因此在begin上加上数组大小N即可，模板定义请看ex16_06.h

ex16_08

泛型编程的一个目标就是令算法是“通用的”，这样就能够适应不同的类型，所有标准库容器都定义了==和!=运算符，但是只有少量定义了<运算符，因此，尽量使用!=而不是<,可以减少算法适用容器的限制

ex16_09

函数模板是可以实例化出特定函数的模板，类模板是可以实例化出特定类的模板；
从形式上来说，函数模板与普通函数类似，只要是以关键字template开始，后接模板参数列表；类模板与普通类的关系类似；
在使用上，编译器会根据调用来推断函数模板的模板参数的类型，类模板实例化特定类就必须显式地指定模板参数

ex16_10

当使用一个类模板时，必须显式地提供模板实参列表，编译器将它们绑定到模板参数，来替换类模板定义中模板参数出现的地方，这样就实例化出一个特定的类，我们随后使用的其实是这个特定的类

ex16_11

类模板的名字不是一个类型名，类模板用来实例化类型，而一个实例化的类型总是包含模板参数的，因此，在上述代码中直接使用ListItem是错误的，应该使用ListItem<element>,这才是一个类型的名字；
  另一方面，在类模板自己的作用域中，可以直接使用模板名而不提供实参，即上述代码中，类内的List<elemType>可以简化为List

ex16_13

因为函数模板的实例化只处理特定类型，因此，对于相等和关系运算符，对每个BlobPtr实例与相同类型实例化的运算符建立一对一的友好关系即可

ex16_17

当用来声明模板类型的参数时，typename和class是完全等价的，都表明模板参数时一个类型。在C++最初引入模板时，是使用class的，但是为了避免与类定义中的class弄混，引入了typename关键字，从字面上看，typename还暗示了模板类型参数不必是一个类类型，因此，现在更建议使用typename
  当我们希望通知编译器一个名字表示类类型的时候，必须使用关键字typename，而不能使用class
  
ex16_18

（a）是非法的，必须指出U是类型参数还是非类型参数；
（b）是非法的，在作用域中，模板参数名不能重用，这里重用T作为函数参数名了
（c）是非法的，inline智能放在模板参数列表之后，即template<typename T> inline T foo(T,unsigned int*);
（d）是非法的，因为没有指定函数模板返回类型
（e）是合法的，在模板作用域中，类型参数Ctype屏蔽了之前定义的类型别名Ctype