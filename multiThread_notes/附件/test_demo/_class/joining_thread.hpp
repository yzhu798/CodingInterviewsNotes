// 在std::thread基础上添加了析构函数
#include <thread>
#include <utility>

class joining_thread {
  std::thread t;

 public:
  joining_thread() noexcept = default;

  template <typename T, typename... Ts>
  explicit joining_thread(T&& f, Ts&&... args)
      : t(std::forward<T>(f), std::forward<Ts>(args)...) {}

  explicit joining_thread(std::thread x) noexcept : t(std::move(x)) {}
  joining_thread(joining_thread&& rhs) noexcept : t(std::move(rhs.t)) {}

  joining_thread& operator=(joining_thread&& rhs) noexcept {
    if (joinable()) join();
    t = std::move(rhs.t);
    return *this;
  }

  joining_thread& operator=(std::thread rhs) noexcept {
    if (joinable()) join();
    t = std::move(rhs);
    return *this;
  }

  ~joining_thread() noexcept {
    if (joinable()) join();
  }

  void swap(joining_thread&& rhs) noexcept { t.swap(rhs.t); }
  std::thread::id get_id() const noexcept { return t.get_id(); }
  bool joinable() const noexcept { return t.joinable(); }
  void join() { t.join(); }
  void detach() { t.detach(); }
  std::thread& as_thread() noexcept { return t; }
  const std::thread& as_thread() const noexcept { return t; }
};