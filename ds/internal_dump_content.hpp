#pragma once

#include "../template/template_all_but_modint.hpp"

#ifdef LOCAL
namespace library_dump_detail
{
  struct TupleFields
  {
    template <class T>
    static const T &values(const T &value) { return value; }
  };

  struct IntervalFields : TupleFields
  {
    inline static constexpr char name[] = "Interval";
    inline static constexpr array<string_view, 3> labels{"l", "r", "val"};
  };

  struct IntervalMapFields
  {
    inline static constexpr char name[] = "Interval";
    inline static constexpr array<string_view, 3> labels{"l", "r", "x"};
    template <class T>
    static auto values(const T &value) { return std::tie(value.l, value.r, value.x); }
  };

  struct PointFields : TupleFields
  {
    inline static constexpr char name[] = "Point";
    inline static constexpr array<string_view, 3> labels{"x", "y", "val"};
  };

  struct GroupFields : TupleFields
  {
    inline static constexpr char name[] = "Group";
    inline static constexpr array<string_view, 2> labels{"value", "indices"};
  };

  struct MapFields : TupleFields
  {
    inline static constexpr char name[] = "Entry";
    inline static constexpr array<string_view, 2> labels{"key", "value"};
  };

  template <class Fields, class T>
  struct Entry
  {
    T value;
  };

  // 参照を返すイテレータと、値を返すイテレータの両方をそのまま包む。
  template <class Fields, class Range>
  struct Content
  {
    const Range &range;
    using BaseIterator = decltype(declval<const Range &>().begin());
    struct Iterator
    {
      using iterator_category = input_iterator_tag;
      using value_type = Entry<Fields, decltype(*declval<const BaseIterator &>())>;
      using difference_type = ptrdiff_t;
      using reference = value_type;
      using pointer = void;
      BaseIterator it;
      value_type operator*() const { return {*it}; }
      Iterator &operator++() { ++it; return *this; }
      Iterator operator++(int) { auto old = *this; ++*this; return old; }
      bool operator==(const Iterator &other) const { return !(it != other.it); }
      bool operator!=(const Iterator &other) const { return it != other.it; }
    };
    Iterator begin() const { return {range.begin()}; }
    Iterator end() const { return {range.end()}; }
  };
}

namespace cpp_dump::_detail
{
  template <class Fields, class T>
  inline string export_object_generic(
      const library_dump_detail::Entry<Fields, T> &value, const string &indent,
      size_t last_line_length, size_t current_depth, bool fail_on_newline,
      const export_command &command)
  {
    string class_name = es::class_name(Fields::name);
    _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON1;
    size_t index = 0;
    std::apply([&](const auto &...member)
          { (append_output(Fields::labels[index++], member), ...); },
          Fields::values(value.value));
    _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2;
  }

  template <class Fields, class Owner, class GetContent>
  inline string export_named_content(
      const Owner &, const GetContent &get_content, const string &indent,
      size_t last_line_length, size_t current_depth, bool fail_on_newline,
      const export_command &command)
  {
    string class_name = es::class_name(get_typename<Owner>());
    _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON1;
    auto &&content = get_content();
    using Range = remove_reference_t<decltype(content)>;
    append_output("content()", (library_dump_detail::Content<Fields, Range>{content}));
    _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2;
  }
} // namespace cpp_dump::_detail
#endif
