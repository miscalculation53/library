#pragma once

#include "template_types.hpp"

#ifdef LOCAL
// cpp-dump 内の再帰的な出力からも、map 用のオーバーロードを参照できるようにする。
namespace cpp_dump::_detail
{
  struct export_command;
#define _p_LIBRARY_DECLARE_MAP_EXPORT(Map)                                  \
  template <class... Args>                                                 \
  string export_var(const std::Map<Args...> &, const string &, size_t,       \
                    size_t, bool, const export_command &);
  _p_LIBRARY_DECLARE_MAP_EXPORT(map)
  _p_LIBRARY_DECLARE_MAP_EXPORT(multimap)
  _p_LIBRARY_DECLARE_MAP_EXPORT(unordered_map)
  _p_LIBRARY_DECLARE_MAP_EXPORT(unordered_multimap)
#undef _p_LIBRARY_DECLARE_MAP_EXPORT
}

#include <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump

namespace cpp_dump::options
{
  inline string map_key_color = "\x1b[36m";
}

namespace cpp_dump::_detail
{
  template <class It, class Format>
  struct map_values_for_dump
  {
    It first, last;
    const Format &format;
    map_values_for_dump(It first, It last, const Format &format) : first(first), last(last), format(format) {}
    struct iterator
    {
      It it;
      const Format &format;
      decltype(auto) operator*() const { return format(it->second); }
      iterator &operator++() { ++it; return *this; }
      bool operator!=(const iterator &other) const { return it != other.it; }
    };
    iterator begin() const { return {first, format}; }
    iterator end() const { return {last, format}; }
  };

  inline string color_map_key(const string &text)
  {
    if (!use_es() || options::map_key_color.empty()) return text;
    string plain;
    for (size_t i = 0; i < text.size();)
    {
      if (text[i] == '\x1b' && i + 1 < text.size() && text[i + 1] == '[')
      {
        size_t end = i + 2;
        while (end < text.size() && (isdigit(static_cast<unsigned char>(text[end])) || text[end] == ';')) ++end;
        if (end < text.size() && text[end] == 'm')
        {
          i = end + 1;
          continue;
        }
      }
      plain += text[i++];
    }
    return es::apply(options::map_key_color, plain);
  }

  template <class Map, class Format>
  string export_library_map(
      const Map &map, const string &indent, size_t last_line_length,
      size_t current_depth, bool fail_on_newline, const export_command &command,
      const Format &format)
  {
    if (map.empty()) return es::bracket("{ }", current_depth);
    if (current_depth >= options::max_depth)
      return es::bracket("{ ", current_depth) + es::op("...") + es::bracket(" }", current_depth);

    const auto &key_command = command.next_for_map_key();
    const auto &value_command = command.next_for_map_value();
    const size_t next_depth = current_depth + 1;
    auto map_wrapper = [&]()
    {
      if constexpr (is_multimap<Map>) return _multimap_wrapper(map);
      else return _map_dummy_wrapper(map);
    }();

    bool multiline = options::cont_indent_style == types::cont_indent_style_t::always;
    if (options::cont_indent_style == types::cont_indent_style_t::when_nested)
      multiline = is_multimap<Map> || is_iterable_like<typename Map::key_type> || is_iterable_like<typename Map::mapped_type>;
    if (options::cont_indent_style == types::cont_indent_style_t::when_non_tuples_nested)
      multiline = is_multimap<Map>
          || (is_iterable_like<typename Map::key_type> && !is_tuple<typename Map::key_type>)
          || (is_iterable_like<typename Map::mapped_type> && !is_tuple<typename Map::mapped_type>);

    for (;; multiline = true)
    {
      if (multiline && fail_on_newline) return "\n";
      const string element_indent = multiline ? indent + "  " : indent;
      string output = es::bracket(multiline ? "{" : "{ ", current_depth);
      bool first = true, retry = false;
      auto skipped_map = command.create_skip_container(map_wrapper);
      for (const auto &[ellipsis, it, index] : skipped_map)
      {
        if (!first) output += es::op(multiline ? "," : ", ");
        first = false;
        if (multiline) output += "\n" + element_indent;
        if (ellipsis) output += es::op("...");
        else
        {
          const auto &[key, value] = *it;
          auto render = [&](const auto &x, const export_command &child_command)
          {
            return export_var(format(x), element_indent,
                              get_last_line_length(output, last_line_length),
                              next_depth, !multiline, child_command);
          };
          output += color_map_key(render(key, key_command));
          if constexpr (is_multimap<Map>)
            output += es::member(" (" + to_string(map.count(key)) + ")");
          output += es::op(": ");
          if constexpr (is_multimap<Map>)
          {
            auto [begin, end] = map.equal_range(key);
            // この view は所有しない。描画中だけ元の map を参照する。
            auto values = map_values_for_dump(begin, end, format);
            output += export_var(values, element_indent,
                                 get_last_line_length(output, last_line_length),
                                 next_depth, !multiline, value_command);
          }
          else output += render(value, value_command);
        }
        if (!multiline && (has_newline(output) || last_line_length + get_length(output) + 2 > options::max_line_width))
        {
          retry = true;
          break;
        }
      }
      if (retry) continue;
      return output + (multiline ? "\n" + indent : " ") + es::bracket("}", current_depth);
    }
  }

#define _p_LIBRARY_DEFINE_MAP_EXPORT(Map)                                                \
  template <class... Args>                                                             \
  string export_var(const std::Map<Args...> &value, const string &indent,                \
                    size_t last_line_length, size_t current_depth,                     \
                    bool fail_on_newline, const export_command &command)              \
  {                                                                                   \
    return export_library_map(value, indent, last_line_length, current_depth,           \
                              fail_on_newline, command,                               \
                              [](const auto &x) -> const auto & { return x; });        \
  }
  _p_LIBRARY_DEFINE_MAP_EXPORT(map)
  _p_LIBRARY_DEFINE_MAP_EXPORT(multimap)
  _p_LIBRARY_DEFINE_MAP_EXPORT(unordered_map)
  _p_LIBRARY_DEFINE_MAP_EXPORT(unordered_multimap)
#undef _p_LIBRARY_DEFINE_MAP_EXPORT
}
#endif
