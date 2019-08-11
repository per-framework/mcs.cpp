#pragma once

#include "mcs_v1/config.hpp"
#include "mcs_v1/synopsis.hpp"

#include "dumpster_v1/finally.hpp"

inline void mcs_v1::lock::acquire(holder &holder) {
  Private::acquire(*this, holder);
}

inline void mcs_v1::lock::release(holder &holder) {
  Private::release(*this, holder);
}

template <class Action>
std::invoke_result_t<Action> mcs_v1::lock::holding(Action &&action) {
  holder holder;
  acquire(holder);
  auto releaser = dumpster::finally([&]() { release(holder); });
  return action();
}
