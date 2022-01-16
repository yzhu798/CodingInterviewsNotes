#include <iostream>
#include <thread>

class thread_guard {
  std::thread& t;
 public:
  explicit thread_guard(std::thread& x) : t(x) {}
  ~thread_guard() {
    if (t.joinable()) {
      t.join();
    }
  }
  thread_guard(const thread_guard&) = delete;
  thread_guard& operator=(const thread_guard&) = delete;
};

void f() { std::cout << "OK"; }
int main() {
  std::thread t{f};
  thread_guard g{t};
}
