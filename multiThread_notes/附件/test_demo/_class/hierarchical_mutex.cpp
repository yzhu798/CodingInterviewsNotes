#include <mutex>
#include <climits>
#include <iostream>

class hierarchical_mutex {
  std::mutex internal_mutex;
  const unsigned long hierarchy_value;     // 当前层级值
  unsigned long previous_hierarchy_value;  // 前一线程的层级值
  // 所在线程的层级值，thread_local表示值存在于线程存储期
  static thread_local unsigned long this_thread_hierarchy_value;
  void check_for_hierarchy_violation() {  // 检查是否违反层级结构
    if (this_thread_hierarchy_value <= hierarchy_value) {
      throw std::logic_error("mutex hierarchy violated");
    }
  }
  void update_hierarchy_value() {
    // 先存储当前线程的层级值（用于解锁时恢复）
    previous_hierarchy_value = this_thread_hierarchy_value;
    // 再把其设为锁的层级值
    this_thread_hierarchy_value = hierarchy_value;
  }

 public:
  explicit hierarchical_mutex(unsigned long value)
      : hierarchy_value(value), previous_hierarchy_value(0) {}
  void lock() {
    check_for_hierarchy_violation();  // 要求线程层级值大于锁的层级值
    internal_mutex.lock();            // 内部锁被锁住
    update_hierarchy_value();         // 更新层级值
  }
  void unlock() {
    if (this_thread_hierarchy_value != hierarchy_value) {
      throw std::logic_error("mutex hierarchy violated");
    }
    // 恢复前一线程的层级值
    this_thread_hierarchy_value = previous_hierarchy_value;
    internal_mutex.unlock();
  }
  bool try_lock() {
    check_for_hierarchy_violation();
    if (!internal_mutex.try_lock()) return false;
    update_hierarchy_value();
    return true;
  }
};

thread_local unsigned long  // 初始化为ULONG_MAX以使构造锁时能通过检查
    hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);

// 设定值来表示层级
hierarchical_mutex high(10000);
hierarchical_mutex mid(6000);
hierarchical_mutex low(5000);

void lf() {  // 最低层函数
  std::scoped_lock l(low);
}

void hf() {
  std::scoped_lock l(high);
  lf();  // 可以调用低层函数
}

void mf() {
  std::scoped_lock l(mid);
  hf();  // 中层调用了高层函数，违反了层次结构
}

using namespace std;
int main() {
  hf();  // OK
  mf();  // 错误
}