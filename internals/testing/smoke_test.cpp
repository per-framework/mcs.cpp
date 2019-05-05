#include "mcs_v1/lock.hpp"

#include "testing_v1/test.hpp"

using namespace testing_v1;
using namespace mcs_v1;

auto smoke_test = test([]() {
  Lock::lock_t lock;
  verify(Lock::holding(lock, []() { return true; }));
});
