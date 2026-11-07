#pragma once

#include <bits/stdc++.h>
using namespace std;

#if __cplusplus > 202002L && defined(__STDCPP_FLOAT128_T__)
#include <stdfloat>
using f128 = std::float128_t;
#elif defined(__SIZEOF_FLOAT128__)
using f128 = __float128;
#else
#error "f128 requires a compiler with IEEE binary128 support, such as GCC on x86_64."
#endif
#define LIBRARY_HAS_FLOAT128 1

