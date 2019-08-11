#include "mcs_v1/mcs.hpp"

#include "testing_v1/test.hpp"

#include <algorithm>
#include <condition_variable>
#include <mutex>
#include <thread>

using namespace testing_v1;
using namespace mcs_v1;

auto contention_test = test([]() {
  const size_t n_threads =
      std::clamp(std::thread::hardware_concurrency(), 2u, 8u);
  const size_t n_ops = 100000;

  std::mutex mutex;
  std::condition_variable condition;

  size_t n_threads_started = 0;
  size_t n_threads_stopped = 0;

  size_t c_ops = 0;
  lock lock;

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
        holder holder;
        for (size_t j = 0; j < n_ops; ++j) {
          lock.acquire(holder);
          c_ops += 1;
          lock.release(holder);
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
