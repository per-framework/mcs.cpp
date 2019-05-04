#include "mcs_v1/lock.hpp"

#include "testing_v1/test.hpp"

#include <algorithm>
#include <condition_variable>
#include <mutex>
#include <thread>

using namespace testing_v1;
using namespace mcs_v1;

auto smoke = test([]() {
  Lock::lock_t lock;
  verify(Lock::holding(lock, []() { return true; }));
});

auto contention = test([]() {
  const size_t n_threads = std::max(std::thread::hardware_concurrency(), 2u);
  const size_t n_ops = 10000;

  std::mutex mutex;
  std::condition_variable condition;

  size_t n_threads_started = 0;
  size_t n_threads_stopped = 0;

  size_t c_ops = 0;
  Lock::lock_t lock;

  for (size_t i = 0; i < n_threads; ++i) {
    std::thread([&]() {
      {
        std::unique_lock<std::mutex> guard(mutex);
        n_threads_started += 1;
        condition.notify_all();
        while (n_threads_started != n_threads)
          condition.wait(guard);
      }

      {
        Lock::holder_t holder;
        for (size_t j = 0; j < n_ops; ++j) {
          Lock::acquire(lock, holder);
          c_ops += 1;
          Lock::release(lock, holder);
        }
      }

      {
        std::unique_lock<std::mutex> guard(mutex);
        n_threads_stopped += 1;
        condition.notify_all();
      }
    }).detach();
  }

  {
    std::unique_lock<std::mutex> guard(mutex);
    while (n_threads_stopped != n_threads)
      condition.wait(guard);
  }

  verify(n_threads * n_ops == c_ops);
});
