#include <thread>
#include <iostream>
#include <atomic>
#include <mutex>
#include <memory>

std::mutex m;
void func_1(int count){
    static int mMoney =0 ;
    for(int i =0 ;i<count ;i++){
     //   std::lock_guard<std::mutex> lock(m);
        ++mMoney;
    }
    std::cout << mMoney <<" ";
}

int main() {


    std::thread t1(func_1,100000);
    std::thread t2(func_1,200000);
    std::thread t3(func_1,300000);
    std::thread t4(func_1,400000);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    return 0;
}
