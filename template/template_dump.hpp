#pragma once

/**
 * @brief テンプレート（dump）
 * @docs docs/template/template_dump.md
 */

#include "template/template_types.hpp"

#ifdef LOCAL
  #include <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump
  namespace cpp_dump::_detail
  {
    inline string export_var(
        const i128 &x, const string &indent, size_t last_line_length,
        size_t current_depth, bool fail_on_newline, const export_command &command
    ) {
      return export_var(i128tos(x), indent, last_line_length, current_depth, fail_on_newline, command);
    }
  } // namespace cpp_dump::_detail
  #define dump(...) cpp_dump(__VA_ARGS__)
  namespace cp = cpp_dump;
  CPP_DUMP_SET_OPTION_GLOBAL(log_label_func, cp::log_label::line());
  CPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 10000);
#else
  #define dump(...)
#endif