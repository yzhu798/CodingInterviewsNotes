#include <iostream>
#include <future>
#include <chrono>
#include <thread>

int main()
{
    std::promise<int> promise;
    std::shared_future<int> fut(promise.get_future());
    auto fun1 = [fut](){
        fut.wait(); // waits for the signal from main()
        return fut;
    };
    auto fun2 =  std::async(std::launch::async, fun1);
    auto fun3 =  std::async(std::launch::async, fun1);
    auto fun4 =  std::async(std::launch::async, fun1);
    promise.set_value(1);
    std::cout<< "fun2 wait: " << fun2.get().get() <<" "<<std::endl;
    //    std::cout<< "fun2 wait: " << fun2.get().get() <<" "<<std::endl;
    std::cout<< "fun3 wait: " << fun3.get().get() <<" "<<std::endl;
    std::cout<< "fun4 wait: " << fun4.get().get() <<" "<<std::endl;
}
