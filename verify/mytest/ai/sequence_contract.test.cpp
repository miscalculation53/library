#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/bbst/lazy_sequence.hpp"

namespace contract_fixture
{
long long checks = 0;
void require(bool ok, int line, const char *expression)
{
  ++checks;
  if (!ok) throw runtime_error(string("line ") + to_string(line) + ": " + expression);
}
#define REQUIRE(x) ::contract_fixture::require(bool(x), __LINE__, #x)

struct Word
{
  string text;
  Word() = delete;
  explicit Word(string text) : text(std::move(text)) {}
  explicit Word(int value) : text(to_string(value)) {}
};
struct WordMonoid
{
  using S = Word;
  static S e() { return S(string{}); }
  static S op(const S &a, const S &b) { return S(a.text + b.text); }
};
struct Assign
{
  bool active;
  char letter;
  Assign() = delete;
  Assign(bool active, char letter) : active(active), letter(letter) {}
};
struct WordAction : WordMonoid
{
  using F = Assign;
  static F id() { return F(false, 0); }
  static F composition(const F &f, const F &g) { return f.active ? f : g; }
  static S mapping(const F &f, const S &s)
  {
    return f.active ? S(string(s.text.size(), f.letter)) : s;
  }
};
using Plain = SequenceTree<WordMonoid>;
using Acted = LazySequenceTree<WordAction>;

template<class Tree, class = void> struct HasApply : false_type {};
template<class Tree> struct HasApply<Tree, void_t<
    decltype(declval<Tree &>().apply(0, declval<const typename Tree::F &>())),
    decltype(declval<Tree &>().apply(0, 0, declval<const typename Tree::F &>()))>> : true_type {};
template<class Tree> constexpr bool has_apply = HasApply<Tree>::value;
template<class Tree, bool Enabled, class = void> struct HasExplicitApply : false_type {};
template<class Tree, bool Enabled> struct HasExplicitApply<Tree, Enabled, void_t<
    decltype(declval<Tree &>().template apply<Enabled>(0, declval<const typename Tree::F &>())),
    decltype(declval<Tree &>().template apply<Enabled>(0, 0, declval<const typename Tree::F &>()))>> : true_type {};
template<class Tree, bool Enabled> constexpr bool has_explicit_apply = HasExplicitApply<Tree, Enabled>::value;
template<class Tree, class A, bool Lazy>
void check_types()
{
  static_assert(is_same_v<Tree, bbst_detail::SequenceTreeImpl<A, Lazy>>);
  static_assert(is_same_v<typename Tree::Node, bbst_detail::TreapNode<A, true, Lazy>>);
  static_assert(is_same_v<typename Tree::S, typename A::S>);
  static_assert(is_same_v<typename Tree::F, typename Tree::Node::F>);
  static_assert(is_same_v<typename Tree::S, typename Tree::Node::S>);
  if constexpr (Lazy) static_assert(is_same_v<typename Tree::F, typename A::F>);
  static_assert(!is_copy_constructible_v<Tree> && !is_copy_assignable_v<Tree>);
  static_assert(is_nothrow_move_constructible_v<Tree> && is_nothrow_move_assignable_v<Tree>);
  static_assert(is_same_v<decltype(declval<const Tree &>().size()), int>);
  static_assert(is_same_v<decltype(declval<const Tree &>().empty()), bool>);
  static_assert(is_same_v<decltype(declval<const Tree &>().get(0)), Word>);
  static_assert(is_same_v<decltype(declval<const Tree &>().prod(0, 0)), Word>);
  static_assert(is_same_v<decltype(declval<const Tree &>().all_prod()), Word>);
  static_assert(is_same_v<decltype(declval<const Tree &>().content()), vector<Word>>);
  static_assert(is_same_v<decltype(declval<Tree &>().split(0)), Tree>);
  static_assert(is_same_v<decltype(declval<Tree &>().extract(0, 0)), Tree>);
  static_assert(is_constructible_v<Tree, const vector<int> &>);
  static_assert(!is_convertible_v<const vector<int> &, Tree>);
  static_assert(has_apply<Tree> == Lazy);
  static_assert(has_explicit_apply<Tree, true> == Lazy);
  static_assert(!has_explicit_apply<Tree, false>);
  static_assert(!is_default_constructible_v<Word> && !is_default_constructible_v<Assign>);
  static_assert(!is_trivially_destructible_v<Word>);
}

string fold(const vector<string> &a, int l, int r)
{
  string answer;
  for (int i = l; i < r; ++i) answer += a[i];
  return answer;
}
template<class Tree>
void check_words(const Tree &t, const vector<string> &a)
{
  REQUIRE(t.size() == int(a.size()));
  REQUIRE(t.empty() == a.empty());
  REQUIRE(t.all_prod().text == fold(a, 0, a.size()));
  auto content = t.content();
  REQUIRE(content.size() == a.size());
  for (int i = 0; i < t.size(); ++i)
  {
    REQUIRE(content[i].text == a[i]);
    REQUIRE(t.get(i).text == a[i]);
  }
  for (int l = 0; l <= t.size(); ++l)
    for (int r = l; r <= t.size(); ++r)
      REQUIRE(t.prod(l, r).text == fold(a, l, r));
}
template<class Tree> Tree implicit_list()
{
  return {Word("ab"), Word("c"), Word("de"), Word("f")};
}
template<class Tree, bool Lazy>
void test_construction_moves_and_operations()
{
  check_types<Tree, conditional_t<Lazy, WordAction, WordMonoid>, Lazy>();
  Tree empty;
  check_words(empty, {});
  auto t = implicit_list<Tree>();
  vector<string> a{"ab", "c", "de", "f"};
  check_words(t, a);

  Tree converted(vector<int>{7, 11, 13});
  check_words(converted, {"7", "11", "13"});
  istringstream stream("17 19 23 29");
  istream_iterator<int> first(stream), last;
  Tree single_pass(first, last);
  check_words(single_pass, {"17", "19", "23", "29"});

  Tree moved(std::move(t));
  check_words(t, {});
  check_words(moved, a);
  t.insert(0, Word("reused"));
  check_words(t, {"reused"});
  t = std::move(moved);
  check_words(t, a);
  check_words(moved, {});
  moved.insert(0, Word("again"));
  check_words(moved, {"again"});
  auto *self = &t;
  t = std::move(*self);
  check_words(t, a);

  t.insert(0, Word("left")); a.insert(a.begin(), "left");
  t.insert(t.size(), Word("right")); a.push_back("right");
  t.set(2, Word("middle")); a[2] = "middle";
  t.erase(1); a.erase(a.begin() + 1);
  t.reverse(1, 4); reverse(a.begin() + 1, a.begin() + 4);
  t.rotate(0, 2, 4); rotate(a.begin(), a.begin() + 2, a.begin() + 4);
  t.move(1, 3, 0);
  vector<string> cut(a.begin() + 1, a.begin() + 3);
  a.erase(a.begin() + 1, a.begin() + 3);
  a.insert(a.begin(), cut.begin(), cut.end());
  if constexpr (Lazy)
  {
    t.template apply<true>(1, t.size(), Assign(true, 'x'));
    for (int i = 1; i < int(a.size()); ++i) a[i].assign(a[i].size(), 'x');
    t.apply(2, Assign(true, 'y')); a[2].assign(a[2].size(), 'y');
    t.apply(0, t.size(), WordAction::id());
    t.insert(2, Word("fresh")); a.insert(a.begin() + 2, "fresh");
    t.reverse(0, t.size()); reverse(a.begin(), a.end());
  }
  check_words(t, a);

  auto tail = t.split(2);
  check_words(t, vector<string>(a.begin(), a.begin() + 2));
  check_words(tail, vector<string>(a.begin() + 2, a.end()));
  t.concat(std::move(tail));
  check_words(tail, {});
  tail.insert(0, Word("separate"));
  check_words(tail, {"separate"});
  check_words(t, a);
  auto middle = t.extract(1, 3);
  check_words(middle, vector<string>(a.begin() + 1, a.begin() + 3));
  a.erase(a.begin() + 1, a.begin() + 3);
  check_words(t, a);
  t.erase(1, min(3, t.size()));
  a.erase(a.begin() + 1, a.begin() + min(3, int(a.size())));
  check_words(t, a);

  for (int p = 0; p <= t.size(); ++p)
  {
    t.erase(p, p); t.reverse(p, p); t.move(p, p, t.size());
    t.rotate(p, p, p);
    auto none = t.extract(p, p);
    check_words(none, {});
    t.concat(std::move(none));
    if constexpr (Lazy) t.apply(p, p, Assign(true, 'q'));
    check_words(t, a);
  }
  auto all = t.split(0);
  check_words(t, {}); check_words(all, a);
  t.concat(std::move(all));
  auto none = t.split(t.size());
  check_words(none, {}); check_words(t, a);
  t.clear(); t.clear(); check_words(t, {});
}

struct ReadNested
{
  shared_ptr<const Acted> target;
  shared_ptr<Acted> other;
  vector<string> expected;
  size_t limit;
  unique_ptr<int> marker = make_unique<int>(1);
  ReadNested(shared_ptr<const Acted> target, shared_ptr<Acted> other, vector<string> expected, size_t limit)
      : target(std::move(target)), other(std::move(other)), expected(std::move(expected)), limit(limit) {}
  ReadNested(const ReadNested &) = delete;
  bool operator()(Word &value) const
  {
    string before = value.text;
    const auto &t = *target;
    REQUIRE(t.get(0).text == expected[0]);
    REQUIRE(t.all_prod().text == fold(expected, 0, expected.size()));
    REQUIRE(t.prod(1, t.size()).text == fold(expected, 1, expected.size()));
    auto now = t.content();
    for (int i = 0; i < t.size(); ++i) REQUIRE(now[i].text == expected[i]);
    const size_t inner_limit = 4;
    int right = 0;
    while (right < t.size() && fold(expected, 0, right + 1).size() <= inner_limit) ++right;
    int left = t.size();
    while (left > 0 && fold(expected, left - 1, t.size()).size() <= inner_limit) --left;
    REQUIRE(t.max_right_ok(0, [&](Word &x)
    {
      REQUIRE(t.get(t.size() - 1).text == expected.back());
      return x.text.size() <= inner_limit;
    }) == right);
    REQUIRE(t.min_left_ok(t.size(), [&](Word &x)
    {
      REQUIRE(t.prod(0, 0).text.empty());
      return x.text.size() <= inner_limit;
    }) == left);
    other->set(0, Word("unrelated"));
    REQUIRE(other->get(0).text == "unrelated");
    REQUIRE(value.text == before);
    return value.text.size() <= limit;
  }
};
static_assert(!is_copy_constructible_v<ReadNested>);
void test_callbacks()
{
  auto t = make_shared<Acted>(implicit_list<Acted>());
  vector<string> expected{"ab", "c", "de", "f"};
  t->apply(0, t->size(), Assign(true, 'x'));
  for (auto &x : expected) x.assign(x.size(), 'x');
  t->reverse(0, t->size()); reverse(expected.begin(), expected.end());
  auto other = make_shared<Acted>(vector<Word>{Word("unrelated")});
  for (int l = 0; l <= t->size(); ++l)
    for (int r = l; r <= t->size(); ++r)
    {
      ReadNested predicate(t, other, expected, fold(expected, l, r).size());
      REQUIRE(t->max_right_ok(l, predicate) == r);
      REQUIRE(t->min_left_ok(r, predicate) == l);
    }
  check_words(*t, expected);
  auto always = [](Word &) { return true; };
  REQUIRE(t->max_right_ok(t->size(), always) == t->size());
  REQUIRE(t->min_left_ok(0, always) == 0);
  Acted empty;
  REQUIRE(empty.max_right_ok(0, always) == 0);
  REQUIRE(empty.min_left_ok(0, always) == 0);
}

struct Minimum
{
  int value;
  Minimum() = delete;
  explicit Minimum(int value) : value(value) {}
};
struct MinAssign
{
  using S = Minimum;
  struct F
  {
    bool active;
    int value;
    F() = delete;
    F(bool active, int value) : active(active), value(value) {}
  };
  static S e() { return S(numeric_limits<int>::max()); }
  static S op(const S &x, const S &y) { return S(min(x.value, y.value)); }
  static F id() { return F(false, 0); }
  static F composition(const F &f, const F &g) { return f.active ? f : g; }
  static S mapping(const F &f, const S &x) { return f.active ? S(f.value) : x; }
};
void test_nonunital_mapping()
{
  using Tree = LazySequenceTree<MinAssign>;
  const int infinity = numeric_limits<int>::max();
  MinAssign::F assign(true, 7);
  REQUIRE(MinAssign::mapping(assign, MinAssign::e()).value == 7);
  Tree t;
  t.apply(0, 0, assign);
  REQUIRE(t.all_prod().value == infinity && t.prod(0, 0).value == infinity);
  t.insert(0, Minimum(20)); t.insert(1, Minimum(30)); t.insert(2, Minimum(40));
  t.apply(0, 3, assign);
  t.reverse(0, 3);
  for (int p = 0; p <= t.size(); ++p) REQUIRE(t.prod(p, p).value == infinity);
  t.insert(1, Minimum(90));
  REQUIRE(t.get(1).value == 90);
  REQUIRE(t.all_prod().value == 7);
  auto part = t.extract(1, 3);
  REQUIRE(part.get(0).value == 90 && part.get(1).value == 7);
  t.concat(std::move(part));
  REQUIRE(part.all_prod().value == infinity);
  t.erase(0, t.size());
  REQUIRE(t.all_prod().value == infinity);
  t.apply(0, 0, assign);
  REQUIRE(t.prod(0, 0).value == infinity);
}

struct Token
{
  static inline int alive = 0;
  int value;
  explicit Token(int value) : value(value) { ++alive; }
  ~Token() { --alive; }
};
struct SharedValue
{
  vector<shared_ptr<Token>> values;
  SharedValue() = delete;
  explicit SharedValue(vector<shared_ptr<Token>> values) : values(std::move(values)) {}
};
struct SharedMonoid
{
  using S = SharedValue;
  static S e() { return S(vector<shared_ptr<Token>>{}); }
  static S op(const S &x, const S &y)
  {
    auto values = x.values;
    values.insert(values.end(), y.values.begin(), y.values.end());
    return S(std::move(values));
  }
};
using SharedTree = SequenceTree<SharedMonoid>;
weak_ptr<Token> insert_token(SharedTree &t, int id)
{
  auto p = make_shared<Token>(id);
  weak_ptr<Token> weak = p;
  t.insert(t.size(), SharedValue({p}));
  return weak;
}
void check_ids(const SharedTree &t, initializer_list<int> ids)
{
  auto content = t.content();
  REQUIRE(content.size() == ids.size());
  int i = 0;
  for (int id : ids)
  {
    REQUIRE(content[i].values.size() == 1);
    REQUIRE(content[i].values[0]->value == id);
    ++i;
  }
}
void test_split_concat_and_value_lifetimes()
{
  REQUIRE(Token::alive == 0);
  weak_ptr<Token> a, b, c, d, e, f;
  SharedTree survivor;
  {
    SharedTree original;
    a = insert_token(original, 1); b = insert_token(original, 2); c = insert_token(original, 3);
    survivor = original.split(1);
    check_ids(original, {1}); check_ids(survivor, {2, 3});
  }
  REQUIRE(a.expired() && !b.expired() && !c.expired());
  {
    SharedTree donor;
    d = insert_token(donor, 4); e = insert_token(donor, 5);
    survivor.concat(std::move(donor));
    REQUIRE(donor.empty());
    f = insert_token(donor, 6);
    donor.clear();
    REQUIRE(f.expired());
  }
  check_ids(survivor, {2, 3, 4, 5});
  REQUIRE(!d.expired() && !e.expired());
  survivor.reverse(0, 4);
  auto extracted = survivor.extract(1, 3);
  check_ids(extracted, {4, 3});
  survivor.clear();
  REQUIRE(b.expired() && e.expired() && !c.expired() && !d.expired());
  extracted.erase(0);
  REQUIRE(d.expired() && !c.expired());
  SharedTree destination;
  auto previous = insert_token(destination, 99);
  destination = std::move(extracted);
  REQUIRE(previous.expired() && extracted.empty());
  check_ids(destination, {3});
  destination.clear();
  REQUIRE(c.expired() && Token::alive == 0);

  SharedTree receiver;
  weak_ptr<Token> g, h;
  {
    SharedTree origin;
    g = insert_token(origin, 7); h = insert_token(origin, 8);
    receiver = origin.extract(0, 2);
    origin = SharedTree();
    REQUIRE(origin.empty());
  }
  check_ids(receiver, {7, 8});
  auto clone = SharedTree(receiver.content());
  receiver.clear();
  REQUIRE(!g.expired() && !h.expired());
  clone.clear();
  REQUIRE(g.expired() && h.expired() && Token::alive == 0);
}

struct SharedAction : WordMonoid
{
  struct F
  {
    shared_ptr<Token> token;
    F() = delete;
    explicit F(shared_ptr<Token> token) : token(std::move(token)) {}
  };
  static F id() { return F(nullptr); }
  static F composition(const F &f, const F &g) { return f.token ? f : g; }
  static S mapping(const F &f, const S &x)
  {
    return f.token ? S(string(x.text.size(), char(f.token->value))) : x;
  }
};
void test_action_lifetimes()
{
  static_assert(!is_default_constructible_v<SharedAction::F>);
  static_assert(!is_trivially_destructible_v<SharedAction::F>);
  REQUIRE(Token::alive == 0);
  using Tree = LazySequenceTree<SharedAction>;
  weak_ptr<Token> first, second;
  {
    Tree t = implicit_list<Tree>();
    {
      auto action = make_shared<Token>('k'); first = action;
      t.apply(0, t.size(), SharedAction::F(action));
    }
    {
      auto action = make_shared<Token>('z'); second = action;
      t.apply(1, 3, SharedAction::F(action));
    }
    t.reverse(0, t.size());
    check_words(t, {"k", "zz", "z", "kk"});
    auto split = t.split(2);
    t.clear();
    check_words(split, {"z", "kk"});
    split.clear();
  }
  REQUIRE(first.expired() && second.expired() && Token::alive == 0);
}

struct alignas(64) AlignedWord
{
  static inline int live = 0;
  string text;
  array<unsigned char, 384> payload{};
  AlignedWord() = delete;
  void check_address() const { REQUIRE(reinterpret_cast<uintptr_t>(this) % alignof(AlignedWord) == 0); }
  explicit AlignedWord(string text) : text(std::move(text)) { check_address(); ++live; }
  AlignedWord(const AlignedWord &x) : text(x.text), payload(x.payload) { check_address(); ++live; }
  AlignedWord(AlignedWord &&x) : text(std::move(x.text)), payload(x.payload) { check_address(); ++live; }
  AlignedWord &operator=(const AlignedWord &) = default;
  AlignedWord &operator=(AlignedWord &&) = default;
  ~AlignedWord() { --live; }
};
struct alignas(128) AlignedAssign
{
  static inline int live = 0;
  shared_ptr<Token> token;
  AlignedAssign() = delete;
  void check_address() const { REQUIRE(reinterpret_cast<uintptr_t>(this) % alignof(AlignedAssign) == 0); }
  explicit AlignedAssign(shared_ptr<Token> token) : token(std::move(token)) { check_address(); ++live; }
  AlignedAssign(const AlignedAssign &x) : token(x.token) { check_address(); ++live; }
  AlignedAssign(AlignedAssign &&x) : token(std::move(x.token)) { check_address(); ++live; }
  AlignedAssign &operator=(const AlignedAssign &) = default;
  AlignedAssign &operator=(AlignedAssign &&) = default;
  ~AlignedAssign() { --live; }
};
struct AlignedAction
{
  using S = AlignedWord;
  using F = AlignedAssign;
  static S e() { return S(string{}); }
  static S op(const S &x, const S &y) { return S(x.text + y.text); }
  static F id() { return F(nullptr); }
  static F composition(const F &f, const F &g) { return f.token ? f : g; }
  static S mapping(const F &f, const S &x)
  {
    return f.token ? S(string(x.text.size(), char(f.token->value))) : x;
  }
};
void test_alignment_and_unused_storage()
{
  static_assert(alignof(AlignedWord) == 64 && sizeof(AlignedWord) >= 384);
  static_assert(alignof(AlignedAssign) == 128);
  static_assert(!is_default_constructible_v<AlignedWord> && !is_default_constructible_v<AlignedAssign>);
  REQUIRE(AlignedWord::live == 0 && AlignedAssign::live == 0 && Token::alive == 0);
  {
    SequenceTree<AlignedAction> ordinary;
    REQUIRE(AlignedWord::live == 0 && AlignedAssign::live == 0);
    ordinary.insert(0, AlignedWord("left"));
    ordinary.insert(1, AlignedWord("right"));
    ordinary.reverse(0, 2);
    REQUIRE(ordinary.get(0).text == "right" && ordinary.all_prod().text == "rightleft");
    ordinary.clear();
    REQUIRE(AlignedWord::live == 0 && AlignedAssign::live == 0);
  }
  weak_ptr<Token> resource;
  {
    LazySequenceTree<AlignedAction> t;
    REQUIRE(AlignedWord::live == 0 && AlignedAssign::live == 0);
    t.insert(0, AlignedWord("abc"));
    t.insert(1, AlignedWord("de"));
    t.insert(2, AlignedWord("f"));
    {
      auto action = make_shared<Token>('a'); resource = action;
      t.apply(0, 3, AlignedAssign(action));
    }
    t.reverse(0, 3);
    REQUIRE(t.get(0).text == "a" && t.get(1).text == "aa");
    REQUIRE(t.all_prod().text == "aaaaaa");
    auto part = t.split(1);
    t.clear();
    REQUIRE(part.prod(0, 2).text == "aaaaa");
    part.insert(1, AlignedWord("fresh"));
    REQUIRE(part.get(1).text == "fresh");
    part.erase(0);
    auto content = part.content();
    REQUIRE(content.size() == 2 && content[0].text == "fresh" && content[1].text == "aaa");
    content.clear();
    part.clear();
    REQUIRE(AlignedWord::live == 0 && AlignedAssign::live == 0);
  }
  REQUIRE(resource.expired() && AlignedWord::live == 0 && AlignedAssign::live == 0 && Token::alive == 0);
}


// Existing API subset: copyable values, explicitly deleted moves, and ADL swap.
// Deliberately avoids vector<S>/content and APIs outside the baseline subset.
struct CopyAdlSubsetValue
{
  int value;
  explicit CopyAdlSubsetValue(int value) : value(value) {}
  CopyAdlSubsetValue(const CopyAdlSubsetValue &) = default;
  CopyAdlSubsetValue &operator=(const CopyAdlSubsetValue &) = default;
  CopyAdlSubsetValue(CopyAdlSubsetValue &&) = delete;
  CopyAdlSubsetValue &operator=(CopyAdlSubsetValue &&) = delete;
  friend void swap(CopyAdlSubsetValue &x, CopyAdlSubsetValue &y) noexcept
  {
    int previous = x.value; x.value = y.value; y.value = previous;
  }
};
struct CopyAdlSubsetMonoid
{
  using S = CopyAdlSubsetValue;
  static S e() { return S(0); }
  static S op(const S &x, const S &y) { return S(x.value + y.value); }
};
void test_existing_copy_adl_subset()
{
  using S = CopyAdlSubsetValue;
  using Tree = SequenceTree<CopyAdlSubsetMonoid>;
  static_assert(is_copy_constructible_v<S> && is_copy_assignable_v<S> && is_swappable_v<S>);
  static_assert(!is_move_constructible_v<S> && !is_move_assignable_v<S>);
  Tree t(vector<int>{1, 2, 3});
  REQUIRE(t.get(1).value == 2 && t.all_prod().value == 6);
  t.set(1, S(9));
  t.insert(1, S(4));
  t.erase(2);
  REQUIRE(t.size() == 3 && t.get(0).value == 1 && t.get(1).value == 4 && t.get(2).value == 3);
  REQUIRE(t.all_prod().value == 8);
  t.reverse(0, 3);
  REQUIRE(t.get(0).value == 3 && t.get(2).value == 1);
  auto other = t.split(1);
  REQUIRE(t.size() == 1 && other.size() == 2 && other.get(0).value == 4);
  t.concat(std::move(other));
  REQUIRE(other.empty() && t.all_prod().value == 8);
  auto middle = t.extract(1, 2);
  REQUIRE(middle.get(0).value == 4 && t.size() == 2);
  middle.clear();
  t.insert(1, S(4));
  Tree moved(std::move(t));
  REQUIRE(t.empty());
  t.insert(0, S(7));
  t = std::move(moved);
  REQUIRE(moved.empty() && t.get(0).value == 3 && t.get(1).value == 4 && t.get(2).value == 1);
  t.clear();
  REQUIRE(t.empty() && t.all_prod().value == 0);
}

struct CopyActionSubset
{
  int a, b;
  CopyActionSubset() = delete;
  CopyActionSubset(int a, int b) : a(a), b(b) {}
  CopyActionSubset(const CopyActionSubset &) = default;
  CopyActionSubset &operator=(const CopyActionSubset &) = default;
  CopyActionSubset(CopyActionSubset &&) = delete;
  CopyActionSubset &operator=(CopyActionSubset &&) = delete;
};
struct CopyActionSubsetMonoid
{
  using S = pair<int, int>;
  using F = CopyActionSubset;
  static S e() { return {0, 0}; }
  static S op(const S &x, const S &y) { return {x.first + y.first, x.second + y.second}; }
  // This return qualification is part of the verified existing subset.
  static const F id() { return F(1, 0); }
  static F composition(const F &f, const F &g) { return F(f.a * g.a, f.a * g.b + f.b); }
  static S mapping(const F &f, const S &x) { return {f.a * x.first + f.b * x.second, x.second}; }
};
void test_existing_copy_action_subset()
{
  using F = CopyActionSubset;
  using Tree = LazySequenceTree<CopyActionSubsetMonoid>;
  static_assert(is_copy_constructible_v<F> && is_copy_assignable_v<F>);
  static_assert(!is_move_constructible_v<F> && !is_move_assignable_v<F>);
  Tree t({{1, 1}, {2, 1}, {3, 1}, {4, 1}});
  t.apply(0, 4, F(2, 1));
  t.apply(0, 4, F(3, -1));
  t.insert(2, {5, 1});
  REQUIRE(t.get(0).first == 8 && t.get(2).first == 5 && t.all_prod().first == 73);
  auto other = t.split(3);
  t.clear();
  other.apply(0, other.size(), F(0, 7));
  other.apply(0, other.size(), F(2, 1));
  other.reverse(0, other.size());
  REQUIRE(other.content()[1].first == 15);
  other.erase(0);
  other.set(0, {9, 1});
  REQUIRE(other.prod(0, 1).first == 9);
}


// Existing action subset: assignment is available, construction from F is not.
struct AssignOnlySubsetAction
{
  int level;
  explicit AssignOnlySubsetAction(int level) : level(level) {}
  AssignOnlySubsetAction(const AssignOnlySubsetAction &) = delete;
  AssignOnlySubsetAction(AssignOnlySubsetAction &&) = delete;
  AssignOnlySubsetAction &operator=(const AssignOnlySubsetAction &) = default;
};
struct AssignOnlySubsetMonoid
{
  using S = int;
  using F = AssignOnlySubsetAction;
  static S e() { return 0; }
  static S op(S x, S y) { return max(x, y); }
  static F id() { return F(0); }
  static const F &composition(const F &f, const F &g) { return f.level >= g.level ? f : g; }
  static S mapping(const F &f, S x) { return max(f.level, x); }
};
void test_existing_assign_only_action_subset()
{
  using F = AssignOnlySubsetAction;
  static_assert(!is_copy_constructible_v<F> && !is_move_constructible_v<F>);
  static_assert(is_copy_assignable_v<F>);
  LazySequenceTree<AssignOnlySubsetMonoid> t({1, 2, 3, 4});
  t.apply(0, 4, F(7));
  t.apply(0, 4, F(9));
  REQUIRE(t.get(2) == 9 && t.all_prod() == 9);
  t.insert(1, 2);
  REQUIRE(t.get(1) == 2 && t.prod(1, 2) == 2);
  t.reverse(0, t.size());
  REQUIRE(t.get(3) == 2 && t.all_prod() == 9);
  auto other = t.split(2);
  t.clear();
  other.apply(0, other.size(), F(11));
  REQUIRE(other.all_prod() == 11);
  other.clear();
  REQUIRE(other.empty() && other.prod(0, 0) == 0);
}


// Existing API subsets with no S copy assignment; set is deliberately excluded.
struct NoCopyAssignSubsetValue
{
  int value;
  explicit NoCopyAssignSubsetValue(int value) : value(value) {}
  NoCopyAssignSubsetValue(const NoCopyAssignSubsetValue &) = default;
  NoCopyAssignSubsetValue(NoCopyAssignSubsetValue &&) = default;
  NoCopyAssignSubsetValue &operator=(const NoCopyAssignSubsetValue &) = delete;
  NoCopyAssignSubsetValue &operator=(NoCopyAssignSubsetValue &&) = default;
};
struct NoCopyAssignSubsetMonoid
{
  using S = NoCopyAssignSubsetValue;
  static S e() { return S(0); }
  static S op(const S &x, const S &y) { return S(x.value + y.value); }
};
struct NoCopyAssignSubsetAction : NoCopyAssignSubsetMonoid
{
  using F = int;
  static F id() { return 1; }
  static F composition(F f, F g) { return f * g; }
  static S mapping(F f, const S &x) { return S(f * x.value); }
};
void test_existing_no_copy_assignment_value_subsets()
{
  using S = NoCopyAssignSubsetValue;
  static_assert(is_copy_constructible_v<S> && is_move_constructible_v<S> && is_move_assignable_v<S>);
  static_assert(!is_copy_assignable_v<S>);
  SequenceTree<NoCopyAssignSubsetMonoid> t(vector<int>{1, 2, 3});
  REQUIRE(t.get(1).value == 2 && t.all_prod().value == 6);
  t.insert(1, S(4));
  t.erase(2);
  t.reverse(0, t.size());
  auto values = t.content();
  REQUIRE(values.size() == 3 && values[0].value == 3 && values[1].value == 4 && values[2].value == 1);
  REQUIRE(t.prod(0, 3).value == 8);
  auto part = t.extract(1, 2);
  REQUIRE(part.get(0).value == 4);
  t.concat(std::move(part));
  REQUIRE(part.empty() && t.get(2).value == 4);

  LazySequenceTree<NoCopyAssignSubsetAction> acted(vector<int>{1, 2, 3});
  acted.apply(0, 3, 2);
  acted.apply(1, 3, 3);
  acted.reverse(0, 3);
  REQUIRE(acted.get(0).value == 18 && acted.get(1).value == 12 && acted.get(2).value == 2);
  acted.insert(1, S(5));
  REQUIRE(acted.prod(0, 4).value == 37);
  acted.apply(0, 1, 2);
  REQUIRE(acted.prod(0, 4).value == 55);
  auto content = acted.content();
  REQUIRE(content.size() == 4 && content[0].value == 36 && content[1].value == 5);
}

int run()
{
  test_construction_moves_and_operations<Plain, false>();
  test_construction_moves_and_operations<Acted, true>();
  test_callbacks();
  test_nonunital_mapping();
  test_split_concat_and_value_lifetimes();
  test_action_lifetimes();
  test_alignment_and_unused_storage();
  test_existing_copy_adl_subset();
  test_existing_copy_action_subset();
  test_existing_assign_only_action_subset();
  test_existing_no_copy_assignment_value_subsets();
  cout << "Hello World\n";
  return 0;
}
} // namespace contract_fixture

int main()
{
  try { return contract_fixture::run(); }
  catch (const exception &e)
  {
    cerr << "sequence_contract_fixture FAIL " << e.what() << '\n';
    return 1;
  }
}
