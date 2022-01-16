#include <iostream>
#include <functional>
#include <future>
#include <thread>
#include <chrono>
#include <iostream>
void test_set_promise(std::promise<int>& promiseObj) {
    std::cout << "In a thread, making data...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    promiseObj.set_value(35); //set_value or set_value_at_thread_exit
    //    promiseObj.set_value_at_thread_exit(9);
}

void test_get_promise(std::promise<int>& promiseObj) {
    std::future<int> futureObj = promiseObj.get_future();
    futureObj.wait();
    std::cout <<" get_promise value ="<< futureObj.get() << std::endl;
}
void test_future_error() {
    std::future<int> fu;
    std::cout << fu.get() << std::endl;
}


int main() {
    std::promise<int> promiseObj;
    std::thread t1(test_get_promise, std::ref(promiseObj));
    std::thread t2(test_set_promise, std::ref(promiseObj));
    t1.join();
    t2.join();
    //    test_future_error();
    
    return 0;
}
