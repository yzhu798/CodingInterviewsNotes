#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <memory>
#include <chrono>
std::mutex mutex;
std::condition_variable cond;
std::deque<int> queue;
int curCount = 0;
bool stop = false;
void test_emit_signal(){
    {
        std::unique_lock lock(mutex);
        curCount --;
        queue.push_back(curCount);
    }//lock.unlock();
    cond.notify_one();
}

void test_case(int count)
{
    for(int i = 0; i< count && (!stop) ; i++){
        test_emit_signal();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
void test_wait (int StopValue = 0){
    auto thread_id = std::this_thread::get_id();
    for(;;){
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::unique_lock lock(mutex);
        cond.wait(lock ,[](){return stop || !queue.empty();});
        // 这一步会原子地unlock mutex 并进入blocking，不会与enqueue 死锁
        if(stop && queue.empty()){
            std::cout << "thread_id:"<<thread_id << " complete stop!" <<std::endl;
            return;
        }
        int value = queue.front();
        queue.pop_front();
        std::cout << "thread_id:"<<thread_id <<" got a value from test_signal:" << value<< std::endl;
        //模拟stop
        if(value == StopValue){
            stop = true;
            std::cout << "value:"<<value << " thread_id:"<<thread_id
                      << "send stop signal!" <<std::endl;
        }
    }
}
int main()
{
    curCount = 40;
    int  stopValue = 0;
    std::thread t1(test_case,20);
    std::thread t2(test_case,20);

    std::thread t3(test_wait,stopValue);
    std::thread t4(test_wait,stopValue);
    std::thread t5(test_wait,stopValue);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

}
