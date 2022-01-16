// 可直接使用std::thread构造的thread_guard
#include <iostream>
#include <thread>
#include <utility>

class scoped_thread {
  std::thread t;

 public:
  explicit scoped_thread(std::thread x) : t(std::move(x)) {
    if (!t.joinable()) {
      throw std::logic_error("no thread");
    }
  }
  ~scoped_thread() { t.join(); }
  scoped_thread(const scoped_thread&) = delete;
  scoped_thread& operator=(const scoped_thread&) = delete;
};

void f() { std::cout << "OK"; }

int main() { scoped_thread t(std::thread{f}); }
