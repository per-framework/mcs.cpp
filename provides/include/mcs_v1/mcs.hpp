#pragma once

#include "mcs_v1/config.hpp"
#include "mcs_v1/synopsis.hpp"

#include "dumpster_v1/finally.hpp"

inline mcs_v1::Private::lock_t::lock_t() : tail(nullptr) {}

inline void mcs_v1::lock::acquire(holder &holder, unsigned max_spin_count) {
  Private::acquire(*this, holder, max_spin_count);
}

inline void mcs_v1::lock::release(holder &holder) {
  Private::release(*this, holder);
}

template <class Action>
std::invoke_result_t<Action> mcs_v1::lock::holding(Action &&action,
                                                   unsigned max_spin_count) {
  holder holder;
  acquire(holder, max_spin_count);
  auto releaser = dumpster::finally([&]() { release(holder); });
  return action();
}
