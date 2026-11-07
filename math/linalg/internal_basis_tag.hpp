#pragma once

namespace internal
{
template <class Tag>
struct BasisTag
{
  using Type = Tag;
  Tag tag;
  explicit BasisTag(Tag tag) : tag(std::move(tag)) {}
  bool accepts(const Tag *threshold) const { return !threshold || !(tag < *threshold); }
};

template <>
struct BasisTag<void>
{
  using Type = monostate;
  explicit BasisTag(monostate = {}) {}
  bool accepts(const monostate *) const { return true; }
};
} // namespace internal
