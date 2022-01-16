#include <iostream>
#include <chrono>
#include <thread>

int main(){
    //    using namespace std::chrono;
    auto  startTimer  = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::chrono::system_clock::time_point
            EndTimer = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = EndTimer - startTimer;
    std::cout << "duration:"<< fp_ms.count() << " ms"<<std::endl;
}
