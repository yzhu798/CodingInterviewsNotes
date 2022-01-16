#include <iostream>
#include <thread>
#include <chrono>
// g++ -std=c++11 -pthread
// https://en.cppreference.com/w/cpp/thread 

void func_byPass(int value = 1){
    std::thread::id pid = std::this_thread::get_id();
    std::cout <<"pid:"<< pid<< __func__ <<":"<< value <<std::endl;
}
void Func_ByRef(int& value) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::thread::id pid = std::this_thread::get_id();
    std::cout <<"pid:"<< pid<< __func__ <<":"<< value <<std::endl;
}

// Define a lamda expression
auto f = [](int value) {
    // Do Something
    std::thread::id pid = std::this_thread::get_id();
    std::cout <<"pid:"<< pid<< __func__ <<":"<< value <<std::endl;
};



struct AA {
    void Me_Func_(int value) {
        std::thread::id pid = std::this_thread::get_id();
        std::cout <<"pid:"<< pid<< __func__ <<":"<< value <<std::endl;
    }
};
void FuncPoint(std::unique_ptr<int> p){
    std::cout << __func__ <<":"<< *p<<std::endl;
}
int main()
{

    std::thread t0(f,1);
    t0.join();

    std::thread t1(func_byPass,2);
    t1.join();
    int value = 3;
    std::thread t2(Func_ByRef,std::ref(value));
    t2.join();
    {//thread的析构函数会调用std::terminate终止程序,obj未被销毁.
        std::thread t22(func_byPass,4);
        t22.join();
    }

    AA obj;
    std::thread t3(&AA::Me_Func_,&obj,100);
    t3.join();

    std::unique_ptr<int> p(new int(43));
    std::thread t4(FuncPoint, std::move(p));
    t4.join();


    std::this_thread::sleep_for(std::chrono::seconds(1));
}
