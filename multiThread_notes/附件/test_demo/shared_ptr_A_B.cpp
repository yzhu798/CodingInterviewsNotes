#include <iostream>
#include <memory>

struct A;
struct B;

struct A {
    std::shared_ptr<B> pointer;
    ~A() {
        std::cout << "~A" << std::endl;
    }
};
struct B {
    std::weak_ptr<A> pointer;
    //    std::shared_ptr<A> pointer;
    ~B() {
        std::cout << "~B" << std::endl;
    }
};

int main() {
    auto a = std::make_shared<A>();
    auto b = std::make_shared<B>();
    a->pointer = b;
    b->pointer = a;
}
