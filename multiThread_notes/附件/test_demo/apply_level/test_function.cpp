#include <iostream>
#include <memory>
#include <functional>
void f(int i) { std::cout << i<<std::endl; }
struct X {
    void operator()(int n) const{
        std::cout << n <<std::endl;
    }
};
struct A {
    int i = 1;
    void f(int n) { std::cout << n <<std::endl; }
};

int main()
{
    std::function<void(int)> f1 = f; //存储函数
    std::function<void(int)> f2 = X(); //存储函数对象
    std::function<void(int)> f3 = [] (int i) { std::cout << i<<std::endl; }; //存储 lambda
    std::function<void(int)> f4 = std::bind(f, std::placeholders::_1); //存储 bind 对象
    std::function<void(A&, int)> f5 = &A::f;   //存储成员函数
    std::function<int(const A&)> f6 = &A::i;   //存储数据成员

    f1(1);
    f2(2);
    f3(3);
    f4(4);
    struct A a;

    f5(a,5);
    f6(a);
    std::cout << "f6 =" <<f6(a) <<std::endl;
}
