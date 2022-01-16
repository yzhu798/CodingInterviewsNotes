#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

class spinlock_mutex {
  std::atomic_flag flag = ATOMIC_FLAG_INIT;

 public:
  void lock() {
    while (flag.test_and_set(std::memory_order_acquire)) {
    }
  }
  void unlock() { flag.clear(std::memory_order_release); }
};

spinlock_mutex m;

void f(int n) {
  for (int i = 0; i < 100; ++i) {
    m.lock();
    std::cout << "Output from thread " << n << '\n';
    m.unlock();
  }
}

int main() {
  std::vector<std::thread> v;
  for (int i = 0; i < 10; ++i) {
    v.emplace_back(f, i);
  }
  for (auto& x : v) {
    x.join();
  }
}