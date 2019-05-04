#pragma once

#include <type_traits>

namespace mcs_v1 {

// lock.hpp ====================================================================

/// Non-recursive MCS spin lock.
struct Lock {
  /// Type of lock holder objects.
  struct holder_t;

  /// Type of lock objects.
  struct lock_t;

  /// Acquires the lock.  The `holder` must be allocated by the caller, kept
  /// alive while the lock is held, and passed to `release` to release the lock.
  /// A unique holder per live acquire is required.
  static void acquire(lock_t &lock, holder_t &holder);

  /// Releases the lock.
  static void release(lock_t &lock, holder_t &holder);

  /// Invokes the action holding the lock.
  template <class Action>
  static std::invoke_result_t<Action> holding(lock_t &lock, Action &&action);
};

} // namespace mcs_v1
