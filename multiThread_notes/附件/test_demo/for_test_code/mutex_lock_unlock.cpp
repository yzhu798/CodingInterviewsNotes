#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;
void do_stuff()
{
    mtx.lock();
    // ... 做一些事 ...也可能是(抛出异常,return)
    mtx.unlock(); //忘记 mtx.unlock()
}

void do_stuff()
{
  std::unique_lock<std::mutex> lck {mtx};
    // ... 做一些事 ...也可能是(抛出异常,return)
}

 int main(){
    std::thread st1(do_stuff);
    st1.join();
}
