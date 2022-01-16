#include <iostream>
#include <thread>
#include <mutex>

class mutex {
public:
  void lock();
  void unlock();
protected:
  uint32_t key{}; //bits 0-30: thread_handle, bit 31: hasWaiters_flag
};

class recursive_mutex : public mutex {
public:
  void lock() {
    uint32_t handle = current_thread_native_handle(); //obtained from TLS memory in most OS
    if ((key & 0x7FFFFFFF) == handle) { // Impossible to return true unless you own the mutex.
      uses++; // we own the mutex, just increase uses.
    } else {
      mutex::lock(); // we don't own the mutex, try to obtain it.
      uses = 1;
    }
  }

  void unlock() {
    // asserts for debug, we should own the mutex and uses > 0
    --uses;
    if (uses == 0) {
      mutex::unlock();
    }
  }
private:
  uint32_t uses{}; // no need to be atomic, can only be modified in exclusion and only interesting read is on exclusion.
};

