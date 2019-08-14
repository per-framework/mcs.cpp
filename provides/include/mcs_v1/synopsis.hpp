#pragma once

#include "mcs_v1/private.hpp"

#include <type_traits>

namespace mcs_v1 {

// mcs.hpp =====================================================================

/// Type of lock holder objects.
struct holder : Private::holder_t {
  /// Holder constructor is empty - no initialization is necessary.
  holder() = default;

  /// Holders are not CopyConstructible.
  holder(const holder &) = delete;

  /// Holders are not CopyAssignable.
  holder &operator=(const holder &) = delete;
};

// Holders are no larger than pointers.
static_assert(sizeof(holder) <= sizeof(void *));

/// Type of lock objects.
struct lock : Private::lock_t {
  /// Constructs a lock by zero initializing it.
  lock() = default;

  /// Locks are not CopyConstructible.
  lock(const lock &) = delete;

  /// Locks are not CopyAssignable.
  lock &operator=(const lock &) = delete;

  /// Default maximum spin count before bailing out to atomic wait.
  static constexpr unsigned default_max_spin_count = 1000;

  /// Acquires the lock.  The `holder` must be allocated by the caller, kept
  /// alive while the lock is held, and passed to `release` to release the lock.
  /// A unique holder per live acquire is required.
  void acquire(holder &holder,
               unsigned max_spin_count = default_max_spin_count);

  /// Releases the lock.  The `holder` must be the same instance that was passed
  /// in a matching call to `acquire`.
  void release(holder &holder);

  /// Invokes the action holding the lock.
  template <class Action>
  std::invoke_result_t<Action>
  holding(Action &&action, unsigned max_spin_count = default_max_spin_count);
};

// Locks are no larger than pointers.
static_assert(sizeof(lock) <= sizeof(void *));

} // namespace mcs_v1
