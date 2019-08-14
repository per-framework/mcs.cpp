# [≡](#contents) [MCS.C++](#) [![Gitter](https://badges.gitter.im/per-framework/community.svg)](https://gitter.im/per-framework/community) [![Build Status](https://travis-ci.org/per-framework/mcs.cpp.svg?branch=v1)](https://travis-ci.org/per-framework/mcs.cpp) [![Code Coverage](https://img.shields.io/codecov/c/github/per-framework/mcs.cpp/v1.svg)](https://codecov.io/gh/per-framework/mcs.cpp/branch/v1)

A lock implementation for C++. Based on the MCS lock described in

<dl>
<dt><a href="http://web.mit.edu/6.173/www/currentsemester/readings/R06-scalable-synchronization-1991.pdf">Algorithms for Scalable Synchronization on Shared-Memory Multiprocessors</a></dt>
<dd>John M. Mellor-Crummey and Michael L. Scott</dd>
</dl>

this simple variation bails out of simple spinning to `std::atomic_wait` and
uses `std::atomic_notify_all` when appropriate.

See [`synopsis.hpp`](provides/include/mcs_v1/synopsis.hpp) for the API.
