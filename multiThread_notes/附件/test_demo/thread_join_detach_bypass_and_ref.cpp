#include <iostream>
#include <thread>
#include <chrono>
using namespace std;
using namespace std::chrono;
// https://www.onlinegdb.com/online_c++_compiler
class A{
    int _value = 0;
    std::string _str;
public:
    A(int value,std::string str):_value(value),
        _str(str){
        std::cout<<"A("<<str<<")"<<endl;
    }

    ~A(){
        std::cout<<"~A("<<_str<<")"<<endl;
    }
    std::string str() const {
        return _str;
    }
    int value() const {
        return _value;
    }
};

void PrintExitInfo(system_clock::time_point startTimer)
{
    auto EndTimer = high_resolution_clock::now();
    duration<double, std::milli> fp_ms = EndTimer - startTimer;
    std::cout << "Exiting thread =" << std::this_thread::get_id() << "\tduration:"<< fp_ms.count() << " ms"<<endl;
}

void func_ref(A & obj){
    system_clock::time_point  startTimer  = std::chrono::high_resolution_clock::now();
    std::cout << "[ref:]starting sub thread =" << std::this_thread::get_id()<<"\tstr =" << obj.str()<<endl;
    std::this_thread::sleep_for(std::chrono::seconds(obj.value()));
    std::cout << "[ref:]stop func_ref thread =" << std::this_thread::get_id()<<"\tstr =" << obj.str()<<endl;
    PrintExitInfo(startTimer);
}

void func_pass(A obj){
    system_clock::time_point  startTimer  = std::chrono::high_resolution_clock::now();
    std::cout << "[pass:]starting sub thread =" << std::this_thread::get_id()<<"\tstr =" << obj.str()<<endl;
    std::this_thread::sleep_for(std::chrono::seconds(obj.value()));
    std::cout << "[pass:]stop func_pass thread =" << std::this_thread::get_id()<<"\tstr =" << obj.str()<<endl;
    PrintExitInfo(startTimer);
}

void test_join_func_ref(int value = 2){
    A obj(2,std::string("hello test_join_func_ref"));
    std::thread thread(func_ref,std::ref(obj));
    thread.join();   // foo_join所在线程,等待线程thread完成执行
    //保证局部变量obj在线程thread结束后才被销毁.
}
void test_join_func_pass(int value = 2){
    A obj(2,std::string("hello test_join_func_pass"));
    std::thread thread(func_pass,obj);
    thread.join();   // foo_join所在线程,等待线程thread完成执行
    //保证局部变量obj在线程thread结束后才被销毁.
}

void test_detach_func_pass(int value = 2){
    A obj(value,std::string("hello test_detach_func_pass"));
    std::thread thread(func_pass,obj);
    thread.detach();
    //变量obj被销毁,thread可能还在运行.
}
void test_detach_func_ref(int value = 2){
    A obj(value,std::string("hello test_detach_func_ref"));
    std::thread thread(func_ref,std::ref(obj));
    thread.detach();   // 允许线程独立于foo_detach所在线程执行
    //变量obj被销毁,thread可能还在运行.
}
void test_terminate_func_pass(int value = 2){
    A obj(value,std::string("hello test_terminate_func_pass"));
    std::thread thread(func_pass,obj);
    //thread的析构函数会调用std::terminate终止程序,obj未被销毁.
}

void test_terminate_func_ref(int value = 2){
    A obj(value,std::string("hello test_terminate_func_ref"));
    std::thread thread(func_ref,std::ref(obj));
    //thread的析构函数会调用std::terminate终止程序,obj未被销毁.
}


int main()
{
    std::thread::id masterThread; // masterThread
    masterThread = std::this_thread::get_id();
    std::cout << "starting main thread =" << masterThread <<endl;
    system_clock::time_point startTimer = std::chrono::high_resolution_clock::now();

    //terminate called without an active exception
    //    test_terminate_func_ref(1);
    //    test_terminate_func_pass(1);

    //crashed
    test_detach_func_ref(1);

    // ok
    test_detach_func_pass(2);
    //    test_join_func_pass(2);
    //    test_join_func_ref(1);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    PrintExitInfo(startTimer);
    return 0;
}

