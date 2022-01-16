

[TOC]



# 0 进程与线程

## 0.0 术语

> 参考:
>
> [1] [note/操作系统.md at master · arkingc/note · GitHub](https://github.com/arkingc/note/blob/master/操作系统/操作系统.md#1进程的定义) 
>
> [2] [多线程的同步与互斥（互斥锁、条件变量、读写锁、自旋锁、信号量）_青萍之末的博客-CSDN博客_线程互斥锁](https://blog.csdn.net/daaikuaichuan/article/details/82950711) 

- **进程/process** : 是操作系统进行**资源分配**的基本单位

	> 进程是“内存中正在运行的程序”。`Linux` 操作系统通过`fork()` 系统调用产生.

- **线程** :是一个基本的CPU执行单元 ,是**调度**的基本单位

  > 通过**共享地址空间**，从而可以高效地共享数据。

  背景:

  >  在多任务操作系统中，同时运行的**多个任务可能**： 
  >
  > - 都需要**访问/使用同一种资源**；
  > - 多个任务之间有依赖关系，某个任务的运行**依赖**于另一个任务。

-  **临界资源**: 是一次仅允许一个进程使用的共享资源.

- **临界区**：一段**代码**,执行代码的进程将访问**临界资源**.

- **原子性操作**：指令序列对外不可分割结果只有,成功和什么也不做。 

-   **死锁**：一组互相竞争资源的线程因**循环等待**，导致“永久”**阻塞**的现象 

死锁条件：

1. **互斥**：一次只有一个进程可以使用一个资源
2. **占有且等待**：当一个进程等待其他进程时，继续占有已经分配的资源
3. **不可抢占**：不能强行抢占进程已占有的资源
4. **循环等待**：存在一个封闭的进程链，使得每个进程至少占有此链中下一个进程所需的一个资源

[![img](myThread.assets/os-4-1.png)](https://github.com/arkingc/note/blob/master/pic/os-4-1.png)

## 0.1 通信(同步)方式

> 通信方面：**进程间**通信需要借助操作系统，**线程间**直接读/写进程数据段。

**进程**: 推荐使用

- **套接字( socket )**

  > TCP 是双向的
  >
  > 进程独占
  >
  > 操作系统会自动回收
  >
  > TCP 是字节流协议，只能顺序读取，有写缓冲

**线程** :推荐使用 **互斥器(mutex)**与 **条件变量(condition variable)** 

- **互斥锁(mutex)**
- 互斥器(mutex) 恐怕是使用得最多最简单的的同步原语.


  - 保护了临界区，一个时刻最多只能有一个线程在临界区内活动

- **条件变量(condition variable)** 

  - 是一个或多个线程等待某个布尔表达式为真，
    即等待别的线程“唤醒”它。
  
  - 条件变量是非常底层的同步原语，很少直接使用.

# 1 线程管理的基础

> - 启动新线程
> - 等待线程与分离线程
> - 线程唯一标识符

## 1.0 std::thread前置知识

- 线程都有入口函数,入口`main`/`foo` 函数,两者会同时运行.
- 线程会在函数结束时完会退出.
- 使用[join](https://en.cppreference.com/w/cpp/thread/thread/join)即可保证局部变量在线程结束后才被销毁.
- [join](https://en.cppreference.com/w/cpp/thread/thread/join)过的[std::thread](https://en.cppreference.com/w/cpp/thread/thread) joinable()为false,故不能再次被[join](https://en.cppreference.com/w/cpp/thread/thread/join)
- 使用[detach](https://en.cppreference.com/w/cpp/thread/thread/detach)分离线程会让线程在后台运行，线程分离后与主线程无法直接交互，也不能被[join](https://en.cppreference.com/w/cpp/thread/thread/join)
- 分离线程称为守护线程，即没有任何显式接口并运行在后台的线程，其特点是长时间运行。
- 线程运行过程中发生异常，之后调用的[join](https://en.cppreference.com/w/cpp/thread/thread/join)会被忽略,为此需要捕获异常并在处理异常时调用[join](https://en.cppreference.com/w/cpp/thread/thread/join)
- 使用RAII类来管理[std::thread](https://en.cppreference.com/w/cpp/thread/thread)

## 1.1 启动线程std::thread

- 将函数添加为[std::thread](https://en.cppreference.com/w/cpp/thread/thread)的参数即可启动线程
- 线程启动后,销毁前要调用[join](https://en.cppreference.com/w/cpp/thread/thread/join)或[detach](https://en.cppreference.com/w/cpp/thread/thread/detach)，否则[std::thread](https://en.cppreference.com/w/cpp/thread/thread)的析构函数会调用[std::terminate](https://en.cppreference.com/w/cpp/error/terminate)终止程序.

```C++
#include <iostream>
#include <thread>
#include <chrono>
void hello(){
    std::cout << "Hello thread:"<<  std::this_thread::get_id()<<std::endl;
}
int main(){
    hello();
    std::thread t(hello);  
    t.join();

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "exit main"<<std::endl;
}
```

## 1.2 为线程函数传递参数

- 有参数的函数也能传给[std::thread](https://en.cppreference.com/w/cpp/thread/thread)，参数的默认实参会被忽略
- [std::thread](https://en.cppreference.com/w/cpp/thread/thread)会无视参数的引用类型，因此需要使用[std::ref](https://en.cppreference.com/w/cpp/utility/functional/ref)来生成一个引用包裹对象以传入引用类型
- 也可以传递类成员函数
- 如果参数是move-only对象则需要使用[std::move](https://en.cppreference.com/w/cpp/utility/move)

```c++
#include <iostream>
#include <thread>
#include <chrono>
void func_byPass(int value = 1){  std::cout << __func__ <<":"<< value <<std::endl;}
void Func_ByRef(int& value) {
    std::cout << __func__ <<":"<< value <<std::endl;
}
struct AA {
    void Me_Func_(int i) { std::cout << __func__ <<":"<< i <<std::endl; }
};
void FuncPoint(std::unique_ptr<int> p){
    std::cout << __func__ <<":"<< *p<<std::endl;
}
int main()
{
    int value = 1;
    std::thread t1(func_byPass,value);
    t1.join();

    std::thread t2(Func_ByRef,std::ref(value));
    t2.join();

    AA obj;
    std::thread t3(&AA::Me_Func_,&obj,100);
    t3.join();

    std::unique_ptr<int> p(new int(43));
    std::thread t4(FuncPoint, std::move(p));
    t4.join();
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

```

## 1.3 转移线程所有权

```c++
void some_function();
void some_other_function();
std::thread t1(some_function);            // 1,some_function与t1关联
std::thread t2=std::move(t1);            // 2,some_function与t2关联
t1=std::thread(some_other_function);    // 3,临时thread,隐形调用move
std::thread t3;                            // 4,默认构造函数,无执行线程关联
t3=std::move(t2);                        // 5
//     t1 == > some_function
//t3 = t2 == > some_function
//直接调用std::terminate()终止程序继续运行
t1=std::move(t3);                        // 6 赋值操作将使程序崩溃
// std::terminate()是noexcept函数)是为了保证与std::thread的析构函数的行为一致。
//需要在线程对象被析构前，显式的等待线程完成，或者分离它；进行赋值时也需要满足这些条件
```

## 1.4 线程标识

```c++
std::thread::id master_thread;
void some_core_part_of_algorithm()
{
  if(std::this_thread::get_id()==master_thread)
  {
    do_master_thread_work();
  }
  do_common_work();
}
std::cout<<std::this_thread::get_id();
//输出结果是依赖于具体实现，C++标准的唯一要求就是要保证ID比较结果相等的线程，必须有相同的输出。
```

## 1.5 std::mem_fn

>  std::mem_fn的作用就是将**类的成员函数**转换为一个**可调用对象** .
>
> include<functional>

```c++
void do_work(unsigned id);
void f()
{
  std::vector<std::thread> threads;
  for(unsigned i=0; i < 20; ++i){
    threads.push_back(std::thread(do_work,i)); // 产生线程
  } 
  std::for_each(threads.begin(),threads.end(),
                  std::mem_fn(&std::thread::join)); // 对每个线程调用join()
}
```

# 2 线程间共享数据

> - 共享数据带来的问题
> - 使用互斥量`mutex`保护数据
> - 数据保护的替代方案

## 2.0 线程间共享数据存在的问题

> 条件竞争 `race condition` :  一个任务的输出依赖于不受控制的事件出现顺序.
>
> 数据竞争`data race` :并发的去修改同一数据,导致数据被破坏 数据竞争是未定义行为。
>
> 出现条件竞争`race condition`的问题时很难复现，因此编程时需要使用大量复杂操作来避免.

### 2.0.0 经典的race condition

 **局部静态变量是数据竞争的一种常见来源**。 

```c++
int get_id() {
  static int id = 1;
  return id++;
}
```

```C++
//先检查非空再获取栈顶元素，在单线程中是安全的
std::stack<int> s；
if (!s.empty())
{
  int n = s.top(); //1
  s.pop();         //2
}
//case1: AB检查非空后,线程A先pop,会导致线程B的top出错.
//case2: AB检查非空后,线程A和线程B同时执行完1,同一个值被处理了2次,看起来没问题,且不好定位bug.
```

**思考: **为什么不直接让pop返回栈顶元素?

假设有一个`stack<vector<int>>`，拷贝vector时需要在堆上分配内存，如果系统负载严重或资源有限，vector的**拷贝构造函数**就会抛出[std::bad_alloc](https://en.cppreference.com/w/cpp/memory/new/bad_alloc)异常,

**返回值**这个动作是**最后执行**，stack在返回前已经弹出了元素，但如果拷贝返回值时抛出异常，就会导致弹出的数据丢失（从栈上移除但拷贝失败）。

```c++
//伪代码
stack移除 A
拷贝构造A ==> 抛出std::bad_alloc异常,打断流程.
拷贝返回值A
返回结果
```

下面思考几种把top和pop合为一步的方法。

**case A 传入参数获取结果值**

```C++
std::vector<int> res;
s.pop(res);
//需要构造一个栈元素类型的实例
//1. 资源消耗大
//2. 可能元素不支持 =赋值操作
//3. 可能构造对象的时候要参数,
```

**caseB 将元素类型classA设置不抛异常的拷贝或移动构造函数**

```c++
stack<vector<classA>> s;
```

**case C** 返回指向弹出元素的指针，指针可以自由拷贝且不会抛异常。这需要管理对象的内存分配，使用[std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr)是个不错的选择.**但这个方案的开销太大，尤其是对于内置类型**.

**case D** 结合方案AB或者AC实现一个线程安全的stack

* 之前锁的粒度（锁保护的数据量大小）太小，保护操作覆盖不周全，这里的粒度就较大，覆盖了大量操作。但并非粒度越大越好，如果锁粒度太大，过多线程请求竞争占用资源时，并发的性能提升就被抵消掉了
* 如果**给定操作需要多个mutex时，就会引入一个新的潜在问题，即死锁**

```c++
#include <exception>
#include <memory>
#include <mutex>
#include <stack>

struct emptyStack : std::exception
{
  const char* what() const noexcept
  {
    return "empty stack!";
  }
};

template<typename T>
class A {
 private:
  std::stack<T> s;
  mutable std::mutex m;
 public:
  A() : s(std::stack<T>()) {}
  
  A(const A& rhs)
  {
    std::lock_guard<std::mutex> l(rhs.m);
    s = rhs.s;
  }
  
  A& operator=(const A&) = delete;
  
  void push(T n)
  {
    std::lock_guard<std::mutex> l(m);
    s.push(std::move(n));
  }
  
  std::shared_ptr<T> pop() // 返回一个指向栈顶元素的指针
  {
    std::lock_guard<std::mutex> l(m);
    if (s.empty()) throw emptyStack();
    const std::shared_ptr<T> res(std::make_shared<T>(std::move(s.top())));
    s.pop();
    return res;
  }
  
  void pop(T& n) // 传引用获取结果
  {
    std::lock_guard<std::mutex> l(m);
    if (s.empty()) throw emptyStack();
    n = std::move(s.top());
    s.pop();
  }
  
  bool empty() const
  {
    std::lock_guard<std::mutex> l(m);
    return s.empty();
  }
};
```



## 2.1 使用mutex保护共享数据

* 使用mutex在访问共享数据前加锁，访问结束后解锁。一个线程用特定的mutex锁定后，其他线程必须等待该线程的mutex解锁才能访问共享数据.
* C++可用[std::mutex](https://en.cppreference.com/w/cpp/thread/mutex)来创建mutex，可通过[std::mutex::lock](https://en.cppreference.com/w/cpp/thread/mutex/lock)加锁，通过[std::unlock](https://en.cppreference.com/w/cpp/thread/mutex/unlock)解锁,但一般不直接使用.
* [std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard)是一个用[std::mutex](https://en.cppreference.com/w/cpp/thread/mutex)构造的RAII模板类(C++11)

### 2.1.0 一些问题

 **避免未释放的锁定**

```c++
mutex mtx;
​
void do_stuff()
{
    mtx.lock();
    // ... 做一些事 ...
    mtx.unlock();
}
```

 **下面将导致死锁**

```
// 线程 1
lock_guard<mutex> lck1(m1);
lock_guard<mutex> lck2(m2);
​
// 线程 2
lock_guard<mutex> lck2(m2);
lock_guard<mutex> lck1(m1);
```

解决

> ### 用 `std::lock()` 或 `std::scoped_lock` 来获得多个 `mutex`
>
> ```c++
> 代之以使用 lock()：
> 
> // 线程 1
> lock(m1, m2);
> lock_guard<mutex> lck1(m1, defer_lock);
> lock_guard<mutex> lck2(m2, defer_lock);
> ​
> // 线程 2
> lock(m2, m1);
> lock_guard<mutex> lck2(m2, defer_lock);
> lock_guard<mutex> lck1(m1, defer_lock);
> //或者（这样更佳，但仅为 C++17）：
> 
> // 线程 1
> scoped_lock<mutex, mutex> lck1(m1, m2);
> ​
> // 线程 2
> scoped_lock<mutex, mutex> lck2(m2, m1);
> //这样，thread1 和 thread2 的作者们仍然未在 mutex 的顺序上达成一致，但顺序不再是问题了。
> ```

* ```c++
  #include <list>
  #include <mutex>
  #include <algorithm>
  
  std::list<int> v;
  std::mutex m;
  
  void f(int n)
  {
    std::lock_guard<std::mutex> l(m); 
    //std::lock_guard l(m); //c++17 引入了类模板实参推断,简写
    v.emplace_back(n);
  }
  
  bool listContains(int n)
  {
    std::lock_guard<std::mutex> l(m);
    return std::find(std::begin(v), std::end(v), n) != std::end(v);
  }
  ```

* C++17 提供加强版的[std::scoped_lock](https://en.cppreference.com/w/cpp/thread/scoped_lock)，它可以接受任意数量的[std::mutex](https://en.cppreference.com/w/cpp/thread/mutex)，可完全取代[std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard)

  ```c++
  #include <mutex>
  std::mutex m1,m2;
  std::scoped_lock g(m1, m2);
  ```

  > **注意**
  >
  > 一般mutex和要保护的数据一起放在类中，定义为private数据成员。
  >
  > 若某个成员函数返回指向**数据成员的指针或引用**，则通过这个**指针的访问行为不会被mutex限制**，因此需要谨慎设置接口，确保mutex能锁住数据

* 锁的粒度太小，保护操作覆盖不周全。

* 如果锁粒度太大，过多线程请求竞争占用资源时，并发的性能提升就被抵消掉了.

* 如果给定操作**需要多个mutex时**，引入**潜在问题----------死锁**.

### 2.1.0 死锁

> * 死锁的四个必要条件：**互斥、占有且等待、不可抢占、循环等待**
>
> * 避免死锁**多个mutex以相同顺序上锁**，总是先锁A再锁B，但这并不适用所有情况
>
> * [std::lock](https://en.cppreference.com/w/cpp/thread/lock)解决了此问题，它可以**一次性锁住多个mutex，并且没有死锁风险**
>
> * [std::lock](https://en.cppreference.com/w/cpp/thread/lock)可能抛异常，此时就不会上锁，因此具有原子性**要么都锁住，要么都不锁**
>
> * 如果支持C++17，最易最优的同时上锁方法是使用[std::scoped_lock](https://en.cppreference.com/w/cpp/thread/scoped_lock)
>
> * ```c++
>   // 线程 1
>   scoped_lock<mutex, mutex> lck1(m1, m2);
>   // 线程 2
>   scoped_lock<mutex, mutex> lck2(m2, m1);
>   ```
> ```
> 
> ```

### 2.1.1 解决死锁

* 第一个建议是，一个线程**已经获取一个锁时就不要获取第二个**。如果每个线程只有一个锁，锁上就不会产生死锁（但除了互斥锁，其他方面也可能造成死锁，比如即使无锁，线程间相互等待也可能造成死锁）

* 第二个建议是，**持有锁时避免调用用户提供的代码**。用户提供的代码可能做任何时，包括获取锁，如果持有锁时调用用户代码获取锁，就会违反第一个建议，并造成死锁。但有时调用用户代码是无法避免的

* 第三个建议是，**按固定顺序获取锁**。如果必须获取多个锁且不能用[std::lock](https://en.cppreference.com/w/cpp/thread/lock)同时获取，最好在每个线程上用固定顺序获取。上面的例子虽然是按固定顺序获取锁，但如果不同时加锁就会出现死锁，对于这种情况的建议是额外规定固定的调用顺序

* 第四个建议是**使用层次锁，如果一个锁被低层持有，就不允许再上锁**

  ```c++
  // 设定值来表示层级
  hierarchical_mutex high(10000);
  hierarchical_mutex mid(6000);
  hierarchical_mutex low(5000);
  
  void lf() // 最低层函数
  {
    std::scoped_lock l(low);
  }
  
  void hf()
  {
    std::scoped_lock l(high);
    lf(); // 可以调用低层函数
  }
  
  void mf()
  {
    std::scoped_lock l(mid);
    hf(); // 中层调用了高层函数，违反了层次结构
  }
  ```

  - **实现hierarchical_mutex**

  ```c++
  class A {
    std::mutex m; // 内部锁
    int val; // 当前层级值
    int pre; // 用于保存前一线程层级值
    static thread_local int tVal; // tVal存活于一个线程周期
   public:
    explicit A(int x) : val(x), pre(0) {}
    void lock()
    {
      if (tVal > val)
      { // 存储线程层级值tVal到pre后将其更新为锁的层级值val
        m.lock();
        pre = tVal;
        tVal = val;
      }
      else
      {
        throw std::logic_error("mutex hierarchy violated");
      }
    }
    void unlock()
    { // 恢复线程层级值为pre
      if (tVal != val)
      {
        throw std::logic_error("mutex hierarchy violated");
      }
      tVal = pre;
      m.unlock();
    }
    bool try_lock()
    {
      if (tVal > val)
      {
        if (!m.try_lock()) return false;
        pre = tVal;
        tVal = val;
        return true;
      }
      else
      {
        throw std::logic_error("mutex hierarchy violated");
      }
    }
  };
  
  thread_local int A::tVal(INT_MAX); // 保证初始构造std::scoped_lock正常
  ```


## 2.2 其他保护共享数据的可选方式

### 2.2.0 保护共享数据的初始化

- 一个极端但常见的情况是，共享数据在并发访问和初始化都需要保护，但之后要隐式同步。数据初始化后上锁只是为了保护初始化过程，但这会不必要地影响性能
- 延迟初始化在单线程中很常见

```cpp
std::shared_ptr<A> P;
void f(){
  if (!p){
    p.reset(new A); // 在多线程中这里需要保护
  }
  p->doSomething();
}
```

* 但在多线程直接上锁会导致不必要的线程资源阻塞

```c++
std::shared_ptr<A> p;
std::mutex m;

void f(){
  std::unique_lock<std::mutex> l(m); // 所有线程会在此处阻塞
  if (!p){
    p.reset(new A);
  }
  l.unlock();
  p->doSomething();
}
```

* 很多人能想到一个更好的方法是双重检查锁模式

```cpp
void f(){
  if (!p) {// 这里没被锁保护，会与其他线程中被锁保护的reset竞争
    std::scoped_lock l(m);
    if (!p){
      p.reset(new A);
    }
  }
  p->doSomething();
}
//第一次的检查没上锁，可能与其他线程中被保护的reset操作产生竞争。
p.reset(new A);
// 1. 为A对象分配一片内存
// 2. 在分配的内存上调用A的构造函数，构造一个A对象
// 3. 返回该内存的指针，让p指向该内存
// 编译器不一定按23顺序执行，可能32
```

- 为了处理race condition，C++标准库提供了[std::once_flag](https://en.cppreference.com/w/cpp/thread/once_flag)和[std::call_once](https://en.cppreference.com/w/cpp/thread/call_once)

```c++
std::shared_ptr<A> p;
std::once_flag flag;

void init()
{
  p.reset(new A);
}

void f()
{
  std::call_once(flag, init);  //比使用mutex的开销更小
  p->doSomething();
}
```

- static变量的初始化存在潜在的race condition：变量声明为static时，声明后就完成了初始化，一个线程完成了初始化，其他线程仍会抢着定义这个变量。

```c++
class A {
public:
    static A& getInstance();
    A(const A&) = delete;
    A& operator(const A&) = delete;
private:
    A() = default;
    ~A() = default;
};

A& A::getInstance()
{//C++11规定static变量的初始化只完全发生在一个线程中.
    static A instance; // C++11 ,线程安全的初始化
    return instance;
}
```

### 2.2.1 保护不常更新的数据结构

* 有些**数据需要经常访问但更新频率很低**，如果用[std::mutex](https://en.cppreference.com/w/cpp/thread/mutex)保护数据有些过度（大量读的操作也会因锁而影响性能），这就需要用上读写锁（reader-writer mutex），它允许多个线程并发读但仅一个线程写
* C++17提供了[std::shared_mutex](https://en.cppreference.com/w/cpp/thread/shared_mutex)和[std::shared_timed_mutex](https://en.cppreference.com/w/cpp/thread/shared_timed_mutex)（C++14），后者比前者提供了更多操作，但前者性能更高。C++11没有提供读写锁，为此可使用[boost::shared_mutex](https://www.boost.org/doc/libs/1_71_0/doc/html/thread/synchronization.html#thread.synchronization.mutex_types.shared_mutex)
* 读写锁并不是万能的，其性能与处理器数量及读写线程的负载有关
* C++14提供了[std::shared_lock](https://en.cppreference.com/w/cpp/thread/shared_lock)，用法和[std::unique_lock](https://en.cppreference.com/w/cpp/thread/unique_lock)相同，此外[std::shared_lock](https://en.cppreference.com/w/cpp/thread/shared_lock)还允许多线程同时获取共享锁，因此一般用[std::shared_lock](https://en.cppreference.com/w/cpp/thread/shared_lock)锁定读，[std::unique_lock](https://en.cppreference.com/w/cpp/thread/unique_lock)锁定写

```cpp
class A {
private:
    mutable std::shared_mutex m; //C++17
    int n = 0;
public:
    int read(){
        std::shared_lock<std::shared_mutex> l(m); //C++14,锁定读
        return n;
    }
    void write(){
        std::unique_lock<std::shared_mutex> l(m); //C++14,锁定写
        ++n;
    }
};
```

### 2.2.2 递归锁

* 一个线程已经获取[std::mutex](https://en.cppreference.com/w/cpp/thread/mutex)（即已上锁）后再次上锁就会产生未定义行为

```c++
std::mutex m;

void f(){
    m.lock();
    m.unlock();
}

void g(){
    m.lock();
    f();
    m.unlock();
}

int main(){
    std::thread t(g);
    t.join(); // 产生未定义行为
}
```

* 为了允许这种情况，C++提供了[std::recursive_mutex](https://en.cppreference.com/w/cpp/thread/recursive_mutex)，它可以在一个线程上多次获取锁，但在其他线程获取锁之前必须释放所有的锁
* 多数情况下，如果需要递归锁，说明代码设计存在问题。比如一个类的每个成员函数都会上锁，一个成员函数调用另一个成员函数，就可能多次上锁，这种情况用递归锁就可以避免产生未定义行为。但显然这个设计本身是有问题的，更好的办法提取其中一个函数作为**private成员并且不上锁**，其他成员先上锁再调用该函数

# 3. 同步的并发操作

> - 带有future的等待
> - 在限定时间内等待
> - 使用同步操作简化代码

















refs:



https://stackoverflow.com/questions/40550730/how-to-implement-timeout-for-function-in-c

