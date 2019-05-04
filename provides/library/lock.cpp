#include "mcs_v1/lock.hpp"
#include "config.hpp"

#include "intrinsics_v1/pause.hpp"

void mcs_v1::Lock::acquire(lock_t &lock, holder_t &holder) {
  holder.locked = nullptr;
  if (holder_t *predecessor = lock.tail.exchange(&holder)) {
    volatile bool locked = true;
    predecessor->locked = &locked;
    do {
      intrinsics::pause();
    } while (locked);
  }
}

void mcs_v1::Lock::release(lock_t &lock, holder_t &holder) {
  holder_t *pholder = &holder;
  if (!holder.locked) {
    if (lock.tail.compare_exchange_strong(pholder, nullptr))
      return;
    do {
      intrinsics::pause();
    } while (!holder.locked);
  }
  *holder.locked = false;
}
