#pragma once

#include <atomic>

namespace mcs_v1 {

struct holder;
struct lock;

/// Private implementation details of MCS locks.
class Private {
  friend struct holder;
  friend struct lock;

  class holder_t;
  class lock_t;

  static void acquire(lock_t &lock, holder_t &holder);
  static void release(lock_t &lock, holder_t &holder);
};

} // namespace mcs_v1

class mcs_v1::Private::holder_t {
  friend class Private;
  volatile bool *locked;
};

class mcs_v1::Private::lock_t {
  friend class Private;
  friend struct lock;
  std::atomic<holder_t *> tail;
  lock_t() : tail(nullptr) {}
};
