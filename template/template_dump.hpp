#pragma once

#include "template_types.hpp"

/**
 * @brief テンプレート（dump）
 * @docs docs/template/template_dump.md
 */

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
CPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 1000);
#define local(...) __VA_ARGS__
#define oj(...)
#define local_oj(a, b) (a)
#else
#define dump(...)
#define local(...)
#define oj(...) __VA_ARGS__
#define local_oj(a, b) (b)
#endif

template <class T, class Sequence>
vc<T> content(queue<T, Sequence> que)
{
  vc<T> res;
  while (!que.empty())
  {
    res.eb(que.front());
    que.pop();
  }
  return res;
}
template <class T, class Sequence, class Compare>
vc<T> content(priority_queue<T, Sequence, Compare> pque)
{
  vc<T> res;
  while (!pque.empty())
  {
    res.eb(pque.top());
    pque.pop();
  }
  return res;
}
