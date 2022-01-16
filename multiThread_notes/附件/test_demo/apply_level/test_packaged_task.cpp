#include <iostream>
#include <stdexcept>
#include <future>
int f(int x){
    if (x < 0){
        throw std::out_of_range("x < 0");
    }
    return x;
}
void task_lambda(int value){
    std::packaged_task<int(int)> task(f);
    std::future<int> result = task.get_future();
    task(value); //调用std::packaged_task对象，将std::future设为就绪
    std::cout << "task_lambda:\t" << result.get() << '\n';
}
void task_future_error(){
    std::packaged_task<int(int)> task;
    //future::get抛出future_error异常
    std::future<int> result = task.get_future();
}
int main(){
    //    task_future_error();
    int value = 1;
    task_lambda(value);
    value = -1;
    task_lambda(value);
}
