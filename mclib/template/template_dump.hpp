#pragma once

#ifdef LOCAL
  #include <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump
  #define dump(...) cpp_dump(__VA_ARGS__)
#else
  #define dump(...)
#endif