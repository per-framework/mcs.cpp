#include "mcs_v1/mcs.hpp"

#include "testing_v1/test.hpp"

using namespace testing_v1;
using namespace mcs_v1;

auto smoke_test = test([]() {
  lock lock;
  verify(lock.holding([]() { return true; }));
});
