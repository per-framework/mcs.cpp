#include "mcs_v1/mcs.hpp"

#include "polyfill_v1/atomic.hpp"

#include "intrinsics_v1/pause.hpp"

void mcs_v1::Private::acquire(lock_t &lock,
                              holder_t &holder,
                              unsigned max_spin_count) {
  holder.state.store(nullptr, std::memory_order_relaxed);
  if (holder_t *predecessor =
          lock.tail.exchange(&holder, std::memory_order_acquire)) {
    std::atomic<state_t> state(SPINNING);
    predecessor->state.store(&state, std::memory_order_release);
    while (max_spin_count) {
      intrinsics_v1::pause();
      if (SPINNING != state.load(std::memory_order_acquire))
        return;
      max_spin_count -= 1;
    }
    if (SPINNING == state.exchange(WAITING, std::memory_order_acq_rel))
      std::atomic_wait(&state, WAITING, std::memory_order_acquire);
  }
}

void mcs_v1::Private::release(lock_t &lock, holder_t &holder) {
  holder_t *pholder = &holder;
  std::atomic<state_t> *state = holder.state.load(std::memory_order_relaxed);
  if (!state) {
    if (lock.tail.compare_exchange_strong(
            pholder, nullptr, std::memory_order_release))
      return;
    do {
      intrinsics_v1::pause();
      state = holder.state.load(std::memory_order_relaxed);
    } while (!state);
  }
  if (WAITING == state->exchange(RELEASED))
    std::atomic_notify_all(state);
}
