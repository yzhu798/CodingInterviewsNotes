#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>
//g++ -std=c++20  -pthread
void thread_proc(std::stop_token st){
  while (!st.stop_requested()){
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  std::cout << "Thread " << std::this_thread::get_id() << " exit" << std::endl;
}

extern "C"
int main(){
  std::jthread thr(&thread_proc); 
  std::this_thread::sleep_for(std::chrono::seconds(3));
  thr.request_stop(); 
  thr.join();
  return 0;
}
