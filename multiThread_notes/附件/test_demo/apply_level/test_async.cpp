#include <chrono>
#include <iostream>
#include <stdexcept>
#include <future>
#include <thread>
int f(int x){
    if (x < 0){
        throw std::out_of_range("x < 0");
    }
    return x;
}

struct A {// 成员函数
    int f(int value){return value;}
    int operator()(int value){return value;}
};

int main(){
    std::thread t(f,1); // How to got the return value of f?
    t.join();
    std::future<int> ft = std::async(f,1);//异步任务
    std::cout << ft.get()<<" "; //可阻塞线程,并获取值 1
//    std::cout << ft.get(); //再次调用出错

    std::future<int> ft2 = std::async(f,-1);//异步任务
    int x = ft2.get(); // 抛出已存储的异常
    std::cout << x;
    A a;
    auto ft1 = std::async(&A::f, &a, 42); // 调用p->f(42)，p是指向x的指针
    std::cout <<ft1.get()<<" ";
    auto ft5 = std::async(&A::f, a, 42); // 调用tmpa.f(42)，tmpa是a的副本
    std::cout <<ft5.get()<<" ";
    auto ft3 = std::async(A(), 42); // 调用tmpa(42)，tmpa由A的移动构造函数获得
    std::cout <<ft3.get()<<" ";
    auto ft4 = std::async(std::ref(a), 42); // 调用a(42)
    std::cout <<ft4.get()<<" ";


}
