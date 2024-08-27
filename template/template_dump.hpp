#pragma once

/**
 * @brief テンプレート（dump）
 * @docs docs/template/template_dump.md
 */

#ifdef LOCAL
  #include <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump
  #define dump(...) cpp_dump(__VA_ARGS__)
  namespace cp = cpp_dump;
  CPP_DUMP_SET_OPTION_GLOBAL(log_label_func, cp::log_label::line());
  CPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 10000);
#else
  #define dump(...)
#endif