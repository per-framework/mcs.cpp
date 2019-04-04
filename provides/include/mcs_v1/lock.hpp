#pragma once

#include "mcs_v1/config.hpp"
#include "mcs_v1/lock_synopsis.hpp"

#include "dumpster_v1/finally.hpp"

#include <atomic>

struct mcs_v1::Lock::holder_t {
  holder_t() {
    // No initialization necessary
  }

  holder_t(const holder_t &) = delete;
  holder_t &operator=(const holder_t &) = delete;

private:
  friend struct Lock;

  volatile holder_t *next;
  volatile bool locked;
};

struct mcs_v1::Lock::lock_t {
  lock_t() : tail(nullptr) {}

  lock_t(const lock_t &) = delete;
  lock_t &operator=(const lock_t &) = delete;

private:
  friend struct Lock;

  std::atomic<holder_t *> tail;
};

template <class Action>
std::invoke_result_t<Action> mcs_v1::Lock::holding(lock_t &lock,
                                                   Action &&action) {
  holder_t holder;
  acquire(lock, holder);
  auto releaser = dumpster::finally([&]() { release(lock, holder); });
  return action();
}
