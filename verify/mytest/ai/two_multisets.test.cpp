#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/two_multisets.hpp"
#include "ds/bbst/ordered_multiset.hpp"
#include "algebra/bit.hpp"

mt19937 rng(817293);
int pick(int n) { return rng() % n; }

template <class Q, class = void>
struct CanErase : false_type {};
template <class Q>
struct CanErase<Q, void_t<decltype(declval<Q &>().erase(0))>> : true_type {};
template <class Q, class = void>
struct CanBack : false_type {};
template <class Q>
struct CanBack<Q, void_t<decltype(declval<const Q &>().back())>> : true_type {};
template <class Q, class = void>
struct CanFront : false_type {};
template <class Q>
struct CanFront<Q, void_t<decltype(declval<const Q &>().front())>> : true_type {};

template <class C>
void test_container()
{
  using Q = PriorityContainer<C, GroupAddSub<typename C::value_type>>;
  constexpr bool BackFirst = !Q::supports_front;
  auto q = Q::with_compare(less<int>{});
  multiset<int> ref;
  for (int step = 0; step < 5000; ++step)
  {
    int kind = pick(5), x = pick(21) - 10;
    if (ref.empty() || kind < 2) { q.push(x); ref.insert(x); }
    else if (kind == 2)
    {
      if constexpr (BackFirst)
      {
        auto it = prev(ref.end());
        int x = *it;
        if (pick(2)) assert(q.extract_back() == x);
        else q.pop_back();
        ref.erase(it);
      }
      else
      {
        int x = *ref.begin();
        if (pick(2)) assert(q.extract_front() == x);
        else q.pop_front();
        ref.erase(ref.begin());
      }
    }
    else if (kind == 3)
    {
      if constexpr (Q::supports_erase)
      {
        x = *next(ref.begin(), pick(ref.size()));
        q.erase(x);
        ref.erase(ref.find(x));
      }
    }
    else if constexpr (Q::supports_front && Q::supports_back)
    {
      if constexpr (BackFirst)
      {
        int x = *ref.begin();
        if (pick(2)) assert(q.extract_front() == x);
        else q.pop_front();
        ref.erase(ref.begin());
      }
      else
      {
        auto it = prev(ref.end());
        int x = *it;
        if (pick(2)) assert(q.extract_back() == x);
        else q.pop_back();
        ref.erase(it);
      }
    }
    assert(q.size() == (int)ref.size());
    assert(q.empty() == ref.empty());
    assert(q.all_prod() == accumulate(ref.begin(), ref.end(), 0LL));
    if (!ref.empty())
    {
      if constexpr (Q::supports_front) assert(q.front() == *ref.begin());
      if constexpr (Q::supports_back) assert(q.back() == *ref.rbegin());
    }
    if (step % 149 == 0)
    {
      vector<int> content(ref.begin(), ref.end());
      assert(q.content() == content);
      assert(q.size() == (int)ref.size());
      assert(q.all_prod() == accumulate(ref.begin(), ref.end(), 0LL));
    }
    if constexpr (Q::supports_contains)
    {
      assert(q.contains(x) == (ref.count(x) != 0));
      assert(!q.try_erase(100));
    }
    if (step % 521 == 0) { q.clear(); ref.clear(); }
  }
}

template <class Compare, template <class...> class Base>
void test_partition()
{
  using Q = TwoMultisets<int, Base, PartitionBySize, GroupAddSub<int>, Compare>;
  Q q(PartitionBySize{0});
  vector<int> a;
  int k = 0;
  Compare cmp;
  for (int step = 0; step < 6000; ++step)
  {
    int kind = pick(7);
    if (a.empty() || kind < 3)
    {
      int x = pick(31) - 15;
      q.insert(x);
      a.push_back(x);
    }
    else if (kind == 3)
    {
      if constexpr (Q::supports_erase)
      {
        int p = pick(a.size()), x = a[p];
        q.erase(x);
        a.erase(a.begin() + p);
      }
    }
    else if (kind == 4)
    {
      k = pick(a.size() + 10);
      q.set_condition(PartitionBySize{k});
    }
    else if (kind == 5 && q.left().size())
    {
      sort(a.begin(), a.end(), cmp);
      int p = min(k, (int)a.size()) - 1;
      assert(q.extract_left() == a[p]);
      a.erase(a.begin() + p);
    }
    else if (kind == 6 && q.right().size())
    {
      sort(a.begin(), a.end(), cmp);
      int p = min(k, (int)a.size());
      assert(q.extract_right() == a[p]);
      a.erase(a.begin() + p);
    }
    sort(a.begin(), a.end(), cmp);
    int n = min(k, (int)a.size());
    assert(q.left().size() == n && q.size() == (int)a.size());
    assert(q.left().all_prod() == accumulate(a.begin(), a.begin() + n, 0LL));
    assert(q.right().all_prod() == accumulate(a.begin() + n, a.end(), 0LL));
    if (n) assert(q.left().back() == a[n - 1]);
    if (n < (int)a.size()) assert(q.right().front() == a[n]);
    if constexpr (Q::LeftContainer::supports_front)
      if (n) assert(q.left().front() == a.front());
    if constexpr (Q::RightContainer::supports_back)
      if (n < (int)a.size()) assert(q.right().back() == a.back());
    if (step % 211 == 0)
    {
      vector<int> left(a.begin(), a.begin() + n), right(a.begin() + n, a.end());
      assert(q.left().content() == left && q.right().content() == right);
    }
    if constexpr (Q::supports_contains)
    {
      assert(q.contains(7) == (find(a.begin(), a.end(), 7) != a.end()));
      assert(!q.try_erase(100));
    }
    if (step % 701 == 0) { q.clear(); a.clear(); }
  }
}

template <class Q>
void test_median()
{
  Q q;
  vector<int> a;
  for (int step = 0; step < 5000; ++step)
  {
    if (a.empty() || pick(3))
    {
      int x = pick(101) - 50;
      q.insert(x);
      a.push_back(x);
    }
    else
    {
      int p = pick(a.size());
      q.erase(a[p]);
      a.erase(a.begin() + p);
    }
    sort(a.begin(), a.end());
    int n = (a.size() + 1) / 2;
    assert(q.left().size() == n);
    if (n) assert(q.left().back() == a[n - 1]);
    if (n < (int)a.size()) assert(q.right().front() == a[n]);
  }
}

template <template <class...> class Base>
void test_right_size_condition()
{
  int k = 2;
  auto rule = [](int k) { return [k](const TwoMultisetsState<int> &s) { return s.right_size >= min(k, s.size()); }; };
  auto q = make_two_multisets<int, Base, GroupAddSub<int>>(rule(k));
  vector<int> a;
  for (int step = 0; step < 2500; ++step)
  {
    int kind = pick(4);
    if (a.empty() || kind < 2)
    {
      int x = pick(41) - 20;
      q.insert(x); a.push_back(x);
    }
    else if (kind == 2 && decltype(q)::supports_erase)
    {
      if constexpr (decltype(q)::supports_erase)
      {
        int p = pick(a.size());
        q.erase(a[p]); a.erase(a.begin() + p);
      }
    }
    else
    {
      sort(a.begin(), a.end());
      int n = (int)a.size() - min(k, (int)a.size());
      if (n && (q.right().empty() || pick(2)))
      {
        assert(q.extract_left() == a[n - 1]);
        a.erase(a.begin() + n - 1);
      }
      else
      {
        assert(q.extract_right() == a[n]);
        a.erase(a.begin() + n);
      }
    }
    if (step % 41 == 0) { k = pick(8); q.set_condition(rule(k)); }
    sort(a.begin(), a.end());
    int n = (int)a.size() - min(k, (int)a.size());
    assert(q.left().content() == vector<int>(a.begin(), a.begin() + n));
    assert(q.right().content() == vector<int>(a.begin() + n, a.end()));
    assert(q.left().all_prod() == accumulate(a.begin(), a.begin() + n, 0));
    assert(q.right().all_prod() == accumulate(a.begin() + n, a.end(), 0));
  }
}

struct Product
{
  using S = ll;
  static S e() { return 1; }
  static S op(S a, S b) { return a * b; }
};

struct SumMonoid
{
  using S = ll;
  static S e() { return 0; }
  static S op(S a, S b) { return a + b; }
};

template <class M>
void test_tree()
{
  using T = typename M::S;
  OrderedMultisetTree<T, M> t;
  vector<T> a;
  for (int step = 0; step < 4000; ++step)
  {
    if (a.empty() || (a.size() < 35 && pick(2)))
    {
      int x = pick(3);
      t.push(x);
      a.push_back(x);
    }
    else
    {
      int x = pick(5);
      auto it = find(a.begin(), a.end(), x);
      assert(t.erase(x) == (it != a.end()));
      if (it != a.end()) a.erase(it);
    }
    sort(a.begin(), a.end());
    assert(t.content() == a);
    for (int x = -1; x < 5; ++x)
    {
      assert(t.order_of_key(x) == lower_bound(a.begin(), a.end(), x) - a.begin());
      assert(t.upper_order_of_key(x) == upper_bound(a.begin(), a.end(), x) - a.begin());
      assert(t.count(x) == count(a.begin(), a.end(), x));
    }
    int l = pick(a.size() + 1), r = pick(a.size() + 1);
    if (l > r) swap(l, r);
    auto fold = [&](int l, int r)
    {
      auto s = M::e();
      for (int i = l; i < r; ++i) s = M::op(s, a[i]);
      return s;
    };
    assert(t.prod_by_order(l, r) == fold(l, r));
    assert(t.all_prod() == fold(0, a.size()));
    auto right = t.split_by_order(l);
    assert(t.all_prod() == fold(0, l) && right.all_prod() == fold(l, a.size()));
    t.join(std::move(right));
    assert(right.empty() && t.content() == a);
  }
  auto moved = std::move(t);
  assert(t.empty() && moved.content() == a);
  t.push(10);
  assert(t.front() == 10);
}

struct Record { int key; ll weight; };
struct RecordCompare
{
  bool descending;
  bool operator()(const Record &a, const Record &b) const { return descending ? a.key > b.key : a.key < b.key; }
};
void test_conditions_and_monoid()
{
  using G = GroupAddSub<ll>;
  auto q = make_two_multisets<ll, multiset, G>(PartitionBySize{2});
  q.insert(30); q.insert(10); q.insert(20);
  assert(q.left().all_prod() == 30 && q.right().all_prod() == 30);
  q.erase(10);
  assert(q.left().all_prod() == 50);
  auto simple = make_two_multisets<ll, priority_queue, G>(PartitionBySize{2});
  simple.insert(30); simple.insert(10); simple.insert(20);
  assert(simple.left().all_prod() == 30 && simple.right().all_prod() == 30);
  auto only_size = make_two_multisets<int>(PartitionBySize{2});
  for (int x : {4, 1, 3}) only_size.insert(x);
  assert(only_size.left().back() == 3 && only_size.left().size() == 2);
  auto rule = [](int k) { return [k](const auto &s) { return s.left_size <= k; }; };
  auto captured = make_two_multisets<int>(rule(2));
  for (int x : {5, 1, 3}) captured.insert(x);
  captured.set_condition(rule(1));
  assert(captured.left().size() == 1 && captured.left().back() == 1);
  captured.set_condition(rule(3));
  assert(captured.left().size() == 3 && captured.left().back() == 5);

  using Cmp = function<bool(ll, ll)>;
  Cmp descending = [](ll a, ll b) { return a > b; };
  TwoMultisets<ll, OrderedMultisetTree, PartitionBySize, G, Cmp>
      native(PartitionBySize{2}, descending);
  native.insert(30); native.insert(10); native.insert(20);
  assert(native.left().back() == 20 && native.left().all_prod() == 50 && native.right().all_prod() == 10);

  using Min = MonoidMin<int>;
  test_partition<less<int>, OrderedMultisetTree>();
  TwoMultisets<int, OrderedMultisetTree, PartitionBySize, Min> mins(PartitionBySize{3});
  for (int x : {7, 2, 5, 2, 9, 1}) mins.insert(x);
  assert(mins.left().size() == 3 && mins.left().all_prod() == 1 && mins.right().all_prod() == 5);
  mins.erase(1); mins.erase(2);
  assert(mins.left().all_prod() == 2 && mins.right().all_prod() == 9);

  TwoMultisets<ll, OrderedMultisetTree, PartitionBySize, Product> products(PartitionBySize{2});
  for (int x : {2, 0, 3, 2}) products.insert(x);
  assert(products.left().all_prod() == 0 && products.right().all_prod() == 6);
  products.erase(0);
  assert(products.left().all_prod() == 4 && products.right().all_prod() == 3);

  TwoMultisets<Record, multiset, PartitionByMedian, GroupTrivial, RecordCompare>
      records(PartitionByMedian{}, RecordCompare{true});
  records.insert({1, 0}); records.insert({5, 0}); records.insert({3, 0});
  assert(records.left().back().key == 3);

  TwoMultisets<Record, OrderedMultisetTree, PartitionByMedian, GroupTrivial, RecordCompare>
      record_tree(PartitionByMedian{}, RecordCompare{true});
  record_tree.insert({1, 10}); record_tree.insert({5, 50}); record_tree.insert({3, 30});
  assert(record_tree.left().back().key == 3);
  record_tree.erase({3, 30});
  assert(record_tree.left().back().key == 5 && record_tree.right().front().key == 1);
}

template <class Q>
void test_sum_condition_for()
{
  Q q(PartitionBySum<ll>{0});
  vector<int> a;
  ll limit = 0;
  for (int step = 0; step < 3000; ++step)
  {
    if (a.empty() || pick(3))
    {
      int x = pick(21);
      q.insert(x); a.push_back(x);
    }
    else
    {
      int p = pick(a.size());
      q.erase(a[p]); a.erase(a.begin() + p);
    }
    if (step % 7 == 0) { limit = pick(501); q.set_condition(PartitionBySum<ll>{limit}); }
    sort(a.begin(), a.end());
    ll sum = 0;
    int n = 0;
    while (n < (int)a.size() && sum + a[n] <= limit) sum += a[n++];
    assert(q.left().size() == n && q.left().all_prod() == sum);
  }
}

void test_sum_condition()
{
  test_sum_condition_for<SumTwoMultisets<ll, multiset, PartitionBySum<ll>>>();
  test_sum_condition_for<TwoMultisets<ll, OrderedMultisetTree, PartitionBySum<ll>, SumMonoid>>();
  auto condition = [](const TwoMultisetsState<ll> &s) { return s.left_prod <= s.right_prod; };
  auto q2 = make_two_multisets<ll, priority_queue, GroupAddSub<ll>>(condition);
  for (int x : {1, 2, 4, 8}) q2.insert(x);
  assert(q2.left().size() == 3 && q2.left().all_prod() == 7 && q2.right().all_prod() == 8);
  SumTwoMultisets<ll> bounded;
  static_assert(is_same_v<typename SumTwoMultisets<ll>::S, ll>);
  static_assert(is_same_v<typename SumTwoMultisets<signed char>::S, signed char>);
  static_assert(is_same_v<typename SumTwoMultisets<unsigned short>::S, unsigned short>);
  ll x = numeric_limits<ll>::max() / 8;
  for (int i = 0; i < 7; ++i) bounded.insert(x);
  assert(bounded.left().all_prod() == x * 4 && bounded.all_prod() == x * 7);
  for (int i = 0; i < 7; ++i) bounded.erase(x);
  assert(bounded.empty() && bounded.all_prod() == 0);
}

void test_initialization_and_move()
{
  PriorityContainer<multiset<ll>, GroupAddSub<ll>> initial(multiset<ll>{1, 3, 3});
  assert(initial.all_prod() == 7 && initial.size() == 3);
  auto copy = initial;
  copy.erase(3);
  assert(copy.all_prod() == 4 && initial.all_prod() == 7);
  auto moved = std::move(initial);
  assert(moved.all_prod() == 7 && initial.empty() && initial.all_prod() == 0);
  initial.push(4);
  initial.push(2);
  assert(initial.all_prod() == 6 && initial.front() == 2);
  moved = std::move(initial);
  assert(moved.all_prod() == 6 && initial.empty() && initial.all_prod() == 0);
  moved.erase(*moved.container().begin());
  assert(moved.all_prod() == 4 && moved.size() == 1);
  moved.push(*moved.container().begin());
  assert(moved.all_prod() == 8 && moved.size() == 2);

  PriorityContainer<priority_queue<ll>, GroupAddSub<ll>> alias_heap;
  alias_heap.push(5);
  for (int i = 0; i < 500; ++i) alias_heap.push(alias_heap.container().top());
  assert(alias_heap.all_prod() == 2505);

  vector<ll> a{1, 3, 3};
  ErasablePriorityQueue<ll> raw(a.begin(), a.end());
  raw.erase(3);
  PriorityContainer<ErasablePriorityQueue<ll>, GroupAddSub<ll>> heap(std::move(raw));
  assert(heap.all_prod() == 4 && heap.size() == 2);
  DoubleEndedPriorityQueue<int, greater<int>> ends(a.begin(), a.end());
  assert(ends.get_min() == 3 && ends.get_max() == 1);

  using Tree = OrderedMultisetTree<ll, Product>;
  PriorityContainer<Tree> native(Tree(a.begin(), a.end()));
  assert(native.all_prod() == 9 && native.size() == 3);
  auto other = std::move(native);
  assert(other.all_prod() == 9 && native.empty() && native.all_prod() == 1);
  native.push(2);
  assert(native.all_prod() == 2);

  function<bool(int, int)> cmp = [](int a, int b) { return a % 17 < b % 17; };
  auto reversed = reverse_compare(cmp);
  auto restored = reverse_compare(reversed);
  assert(reversed(9, 2) && restored(2, 9));
}

void test_deque_and_stateful_heap()
{
  PriorityContainer<MonotonePriorityDeque<int>, GroupAddSub<int>> q;
  for (int x : {0, 3, -2, 5, -4}) q.push(x);
  assert(q.all_prod() == 2 && q.front() == -4 && q.back() == 5);
  assert(q.content() == vector<int>({-4, -2, 0, 3, 5}));
  assert(q.extract_front() == -4 && q.extract_back() == 5);
  assert(q.all_prod() == 1);
  using Cmp = function<bool(int, int)>;
  Cmp cmp = [](int a, int b) { return a > b; };
  using EP = MinErasablePriorityQueue<int, Cmp>;
  auto e = PriorityContainer<EP>::with_compare(cmp);
  for (int x : {1, 4, 2, 4}) e.push(x);
  e.erase(2); e.erase(4);
  assert(e.extract_front() == 4 && e.extract_front() == 1 && e.empty());
  e.clear(); e.push(3); e.push(5);
  assert(e.front() == 5);
  using DP = DoubleEndedPriorityQueue<int, Cmp>;
  auto d = PriorityContainer<DP>::with_compare(cmp);
  for (int x : {2, 7, 4, 7}) d.push(x);
  d.erase(7);
  assert(d.extract_front() == 7 && d.extract_back() == 2 && d.front() == 4);
  d.clear(); d.push(1); d.push(9);
  assert(d.front() == 9 && d.back() == 1);

  using HP = MaxPriorityQueue<int, Cmp>;
  auto h = PriorityContainer<HP, GroupAddSub<int>>::with_compare(cmp);
  for (int x : {2, 7, 4}) h.push(x);
  assert(h.back() == 2 && h.all_prod() == 13);
  assert(h.extract_back() == 2);
  h.pop_back();
  assert(h.back() == 7 && h.all_prod() == 7);

  TwoMultisets<int, MonotonePriorityDeque> median;
  for (int i = 0; i < 2000; ++i)
  {
    median.insert(i);
    assert(median.left().back() == i / 2);
  }
}

void test_fixed_end_types()
{
  vector<int> a{3, 1, 3, 2};
  MinPriorityQueue<int> lo(a.begin(), a.end());
  MaxPriorityQueue<int> hi(a.begin(), a.end());
  assert(lo.content() == vector<int>({1, 2, 3, 3}) && hi.content() == lo.content());
  assert(lo.extract_front() == 1 && hi.extract_back() == 3);
  lo.pop_front(); hi.pop_back();
  assert(lo.front() == 3 && hi.back() == 2);

  MinErasablePriorityQueue<int> erasable_lo(a.begin(), a.end());
  MaxErasablePriorityQueue<int> erasable_hi(a.begin(), a.end());
  erasable_lo.erase(3); erasable_hi.erase(3);
  assert(erasable_lo.content() == vector<int>({1, 2, 3}) && erasable_hi.content() == erasable_lo.content());
  assert(erasable_lo.extract_front() == 1 && erasable_hi.extract_back() == 3);
  erasable_lo.clear(); erasable_hi.clear();
  erasable_lo.push(5); erasable_hi.push(5);
  assert(erasable_lo.front() == 5 && erasable_hi.back() == 5);

  using Cmp = function<bool(int, int)>;
  Cmp cmp = [offset = 7](int x, int y) { return (x + offset) % 17 < (y + offset) % 17; };
  vector<int> keyed{2, 7, 12, 16};
  MinPriorityQueue<int, Cmp> keyed_lo(keyed.begin(), keyed.end(), cmp);
  MaxPriorityQueue<int, Cmp> keyed_hi(keyed.begin(), keyed.end(), cmp);
  sort(keyed.begin(), keyed.end(), cmp);
  assert(keyed_lo.front() == keyed.front() && keyed_hi.back() == keyed.back());
  assert(keyed_lo.content() == keyed && keyed_hi.content() == keyed);
}

template <class T, class Compare>
using DequeHeap = priority_queue<T, deque<T>, reverse_compare_t<Compare>>;

void test_backend_alias()
{
  auto q = make_two_multisets<int, DequeHeap, GroupAddSub<int>>(PartitionBySize{2}, greater<int>{});
  for (int x : {-5, 4, 3, 7}) q.insert(x);
  assert(q.left().all_prod() == 11 && q.right().all_prod() == -2);
  q.set_condition(PartitionBySize{1});
  assert(q.left().back() == 7 && q.left().all_prod() == 7);
  int x = q.extract_left();
  assert(x == 7 && q.left().back() == 4 && q.left().all_prod() == 4);
}

using WeightedItem = pair<int, ll>;
struct WeightProjection
{
  ll scale = 1, bias = 0;
  ll operator()(const WeightedItem &x) const { return scale * x.second + bias; }
};

template <template <class...> class Base, class M = GroupAddSub<ll>>
void test_projected_partition()
{
  // 集約値は比較用キーと独立。C++17 のキャプチャ付きラムダも使う。
  auto project = [scale = 3LL](const WeightedItem &x) { return scale * x.second; };
  ll limit = 0;
  auto q = make_two_multisets<WeightedItem, Base, M>(
      PartitionBySum<ll>{limit}, less<WeightedItem>{}, project);
  vector<WeightedItem> a;
  auto remove = [&](const WeightedItem &x) { a.erase(find(a.begin(), a.end(), x)); };
  for (int step = 0; step < 1500; ++step)
  {
    int kind = pick(4);
    if (a.empty() || (a.size() < 60 && kind == 0))
    {
      WeightedItem x{pick(21) - 10, pick(31)};
      q.insert(x); a.push_back(x);
    }
    else if (kind == 1)
    {
      if constexpr (decltype(q)::supports_erase)
      {
        auto x = a[pick(a.size())];
        q.erase(x); remove(x);
      }
    }
    else if (kind == 2 && !q.left().empty())
    {
      auto x = q.left().back();
      assert(q.extract_left() == x);
      remove(x);
    }
    else if (!q.right().empty())
    {
      auto x = q.right().front();
      assert(q.extract_right() == x);
      remove(x);
    }
    if (step % 7 == 0)
    {
      limit = pick(801);
      q.set_condition(PartitionBySum<ll>{limit});
    }
    sort(a.begin(), a.end());
    int n = 0;
    ll left_sum = 0, total = 0;
    while (n < (int)a.size() && left_sum + project(a[n]) <= limit) left_sum += project(a[n++]);
    for (const auto &x : a) total += project(x);
    assert(q.left().content() == vector<WeightedItem>(a.begin(), a.begin() + n));
    assert(q.right().content() == vector<WeightedItem>(a.begin() + n, a.end()));
    assert(q.left().all_prod() == left_sum && q.right().all_prod() == total - left_sum);
    assert(q.all_prod() == total);
    if (step % 131 == 0) { q.clear(); a.clear(); }
  }
}

void test_projection_objects()
{
  static_assert(is_same_v<decltype(Identity{}(declval<const ll &>())), const ll &>);
  ll original = 3;
  Identity{}(original) = 5;
  assert(original == 5);

  using Member = decltype(&WeightedItem::second);
  using Sum = SumTwoMultisets<WeightedItem, multiset, PartitionBySize, less<WeightedItem>, Member>;
  static_assert(is_same_v<typename Sum::S, ll>);
  Sum sum(PartitionBySize{2}, less<WeightedItem>{}, &WeightedItem::second);
  sum.insert({3, 100}); sum.insert({1, 20}); sum.insert({2, 7});
  assert(sum.left().all_prod() == 27 && sum.right().all_prod() == 100);
  sum.erase({1, 20});
  assert(sum.left().all_prod() == 107);

  auto records = make_two_multisets<Record, multiset, GroupAddSub<ll>>(
      PartitionBySize{2}, RecordCompare{true}, &Record::weight);
  records.insert({1, 100}); records.insert({5, 10}); records.insert({3, 20});
  assert(records.left().all_prod() == 30 && records.left().back().key == 3);

  using Cached = PriorityContainer<multiset<WeightedItem>, GroupAddSub<ll>, WeightProjection>;
  Cached initial(multiset<WeightedItem>{{1, 5}, {2, 7}, {2, 7}}, WeightProjection{2, 3});
  assert(initial.all_prod() == 47);
  auto copy = initial;
  copy.erase(*copy.container().begin());
  copy.push(*copy.container().begin());
  assert(copy.all_prod() == 51 && initial.all_prod() == 47);
  auto moved = std::move(initial);
  initial.push({0, 1});
  assert(moved.all_prod() == 47 && initial.all_prod() == 5);
  moved = std::move(initial);
  assert(moved.all_prod() == 5 && initial.all_prod() == 0);
  moved.clear(); moved.push({4, 2});
  assert(moved.all_prod() == 7);

  using Heap = PriorityContainer<priority_queue<WeightedItem>, GroupAddSub<ll>, Member>;
  auto heap = Heap::with_compare(less<WeightedItem>{}, &WeightedItem::second);
  heap.push({5, 7});
  for (int i = 0; i < 500; ++i) heap.push(heap.container().top());
  assert(heap.all_prod() == 3507);
  heap.pop_back();
  assert(heap.all_prod() == 3500 && heap.extract_back() == WeightedItem(5, 7));
  assert(heap.all_prod() == 3493);

  using Tree = OrderedMultisetTree<WeightedItem, SumMonoid, less<WeightedItem>, WeightProjection>;
  vector<WeightedItem> a{{1, 5}, {2, 7}, {2, 7}, {4, 3}};
  Tree tree(a.begin(), a.end(), less<WeightedItem>{}, WeightProjection{2, 3});
  assert(tree.all_prod() == 56 && tree.prod_by_order(1, 3) == 34);
  auto right = tree.split_by_order(2);
  assert(tree.all_prod() == 30 && right.all_prod() == 26);
  tree.push({0, 1}); right.push({5, 2});
  tree.join(std::move(right));
  assert(tree.all_prod() == 68 && right.empty());
  right.push({0, 1});
  assert(right.all_prod() == 5);
  PriorityContainer<Tree> native(std::move(tree));
  static_assert(is_same_v<typename decltype(native)::projection_type, WeightProjection>);
  native.push({6, 4});
  assert(native.all_prod() == 79);
  auto other = std::move(native);
  native.push({0, 1});
  assert(other.all_prod() == 79 && native.all_prod() == 5);
  other = std::move(native);
  assert(other.all_prod() == 5 && native.empty());
  native.push({0, 2});
  assert(native.all_prod() == 7);

  auto mins = make_two_multisets<WeightedItem, OrderedMultisetTree, MonoidMin<ll>>(
      PartitionBySize{2}, less<WeightedItem>{}, &WeightedItem::second);
  mins.insert({1, 90}); mins.insert({2, 40}); mins.insert({3, 5});
  assert(mins.left().all_prod() == 40 && mins.right().all_prod() == 5);
  mins.erase({2, 40});
  assert(mins.left().all_prod() == 5 && mins.right().empty());

  int calls = 0;
  auto unused = [&](const WeightedItem &x) { ++calls; return x.second; };
  auto plain = make_two_multisets<WeightedItem>(PartitionByMedian{}, less<WeightedItem>{}, unused);
  auto plain_tree = make_two_multisets<WeightedItem, OrderedMultisetTree>(
      PartitionByMedian{}, less<WeightedItem>{}, unused);
  for (auto x : a) { plain.insert(x); plain_tree.insert(x); }
  plain.erase(a[0]); plain_tree.erase(a[0]);
  assert(calls == 0);
}

#ifdef LOCAL
template <template <class...> class Base, class Compare = less<int>>
void test_dump()
{
  SumTwoMultisets<int, Base, PartitionBySize, Compare> q(PartitionBySize{2});
  for (int x : {3, 1, 2, 4}) q.insert(x);
  if constexpr (decltype(q)::supports_erase) { q.insert(99); q.erase(99); }
  const auto left = q.left().content(), right = q.right().content();
  const auto left_prod = q.left().all_prod(), right_prod = q.right().all_prod();
  auto text = cp::export_var(q);
  assert(text.find("left= " + cp::export_var(left)) != string::npos);
  assert(text.find("right= " + cp::export_var(right)) != string::npos);
  assert(text.find("left_prod= " + to_string(left_prod)) != string::npos);
  assert(text.find("right_prod= " + to_string(right_prod)) != string::npos);
  assert(cp::export_var(q.left()).find(cp::export_var(left)) != string::npos);
  assert(q.left().content() == left && q.right().content() == right);
  assert(q.left().all_prod() == left_prod && q.right().all_prod() == right_prod);

  q.clear();
  text = cp::export_var(q);
  assert(text.find("left= " + cp::export_var(vector<int>{})) != string::npos);
  assert(text.find("right= " + cp::export_var(vector<int>{})) != string::npos);

  TwoMultisets<int, Base> plain;
  plain.insert(3); plain.insert(1);
  text = cp::export_var(plain);
  assert(text.find("left= " + cp::export_var(vector<int>{1})) != string::npos);
  assert(text.find("right= " + cp::export_var(vector<int>{3})) != string::npos);
  assert(text.find("left_prod") == string::npos);

  vector<SumTwoMultisets<int, Base, PartitionBySize>> nested;
  nested.emplace_back(PartitionBySize{1});
  nested.back().insert(2); nested.back().insert(5);
  text = cp::export_var(nested);
  assert(text.find("left= " + cp::export_var(vector<int>{2})) != string::npos);
  assert(text.find("right= " + cp::export_var(vector<int>{5})) != string::npos);
}

void test_projected_dump()
{
  auto q = make_two_multisets<WeightedItem, OrderedMultisetTree, GroupAddSub<ll>>(
      PartitionBySize{1}, less<WeightedItem>{}, &WeightedItem::second);
  q.insert({1, 20}); q.insert({2, 7});
  auto compact = [](string text)
  {
    text.erase(remove_if(text.begin(), text.end(), [](unsigned char c) { return isspace(c); }), text.end());
    return text;
  };
  auto text = compact(cp::export_var(q));
  assert(text.find("left=" + compact(cp::export_var(vector<WeightedItem>{{1, 20}}))) != string::npos);
  assert(text.find("right=" + compact(cp::export_var(vector<WeightedItem>{{2, 7}}))) != string::npos);
  assert(text.find("left_prod=20") != string::npos && text.find("right_prod=7") != string::npos);
}
#endif

int main()
{
  static_assert(!CanErase<PriorityContainer<MinPriorityQueue<int>>>::value);
  static_assert(!CanBack<PriorityContainer<MinPriorityQueue<int>>>::value);
  static_assert(CanErase<PriorityContainer<multiset<int>>>::value);
  static_assert(CanBack<PriorityContainer<DoubleEndedPriorityQueue<int>>>::value);
  static_assert(CanFront<MinPriorityQueue<int>>::value && !CanBack<MinPriorityQueue<int>>::value);
  static_assert(!CanFront<MaxPriorityQueue<int>>::value && CanBack<MaxPriorityQueue<int>>::value);
  static_assert(CanFront<MinErasablePriorityQueue<int>>::value && !CanBack<MinErasablePriorityQueue<int>>::value);
  static_assert(!CanFront<MaxErasablePriorityQueue<int>>::value && CanBack<MaxErasablePriorityQueue<int>>::value);
  using MaxHeap = PriorityContainer<MaxPriorityQueue<int>, GroupAddSub<int>>;
  static_assert(!CanFront<MaxHeap>::value && CanBack<MaxHeap>::value);
  using HeapSides = TwoMultisets<int, priority_queue>;
  static_assert(!CanFront<typename HeapSides::LeftContainer>::value);
  static_assert(CanBack<typename HeapSides::LeftContainer>::value);
  static_assert(CanFront<typename HeapSides::RightContainer>::value);
  static_assert(!CanBack<typename HeapSides::RightContainer>::value);
  using HeapPartition = TwoMultisets<int, priority_queue>;
  static_assert(!CanErase<HeapPartition>::value);
  test_container<multiset<int>>();
  test_container<MinPriorityQueue<int>>();
  test_container<ErasablePriorityQueue<int>>();
  test_container<DoubleEndedPriorityQueue<int>>();
  test_container<OrderedMultisetTree<int, GroupAddSub<int>>>();
  test_container<MaxPriorityQueue<int>>();
  test_container<priority_queue<int>>();
  test_container<MinErasablePriorityQueue<int>>();
  test_container<MaxErasablePriorityQueue<int>>();
  test_partition<less<int>, multiset>();
  test_partition<greater<int>, multiset>();
  test_partition<less<int>, priority_queue>();
  test_partition<greater<int>, priority_queue>();
  test_partition<less<int>, ErasablePriorityQueue>();
  test_partition<greater<int>, ErasablePriorityQueue>();
  test_partition<less<int>, DoubleEndedPriorityQueue>();
  test_partition<greater<int>, DoubleEndedPriorityQueue>();
  test_partition<greater<int>, OrderedMultisetTree>();
  test_partition<less<int>, MinPriorityQueue>();
  test_partition<less<int>, MaxPriorityQueue>();
  test_partition<less<int>, MinErasablePriorityQueue>();
  test_partition<less<int>, MaxErasablePriorityQueue>();
  test_median<TwoMultisets<int>>();
  test_median<TwoMultisets<int, ErasablePriorityQueue>>();
  test_right_size_condition<multiset>();
  test_right_size_condition<priority_queue>();
  test_right_size_condition<ErasablePriorityQueue>();
  test_right_size_condition<DoubleEndedPriorityQueue>();
  test_right_size_condition<OrderedMultisetTree>();
  test_tree<MonoidMin<int>>();
  test_tree<Product>();
  test_tree<GroupXor<int>>();
  test_conditions_and_monoid();
  test_sum_condition();
  test_deque_and_stateful_heap();
  test_fixed_end_types();
  test_initialization_and_move();
  test_backend_alias();
  test_projected_partition<multiset>();
  test_projected_partition<priority_queue>();
  test_projected_partition<ErasablePriorityQueue>();
  test_projected_partition<DoubleEndedPriorityQueue>();
  test_projected_partition<OrderedMultisetTree>();
  test_projected_partition<OrderedMultisetTree, SumMonoid>();
  test_projection_objects();
#ifdef LOCAL
  cp::options::es_style = cp::types::es_style_t::no_es;
  cp::options::max_line_width = 10000;
  cp::options::max_depth = 10;
  test_dump<multiset>();
  test_dump<priority_queue>();
  test_dump<ErasablePriorityQueue>();
  test_dump<DoubleEndedPriorityQueue>();
  test_dump<OrderedMultisetTree>();
  test_dump<multiset, greater<int>>();
  test_dump<priority_queue, greater<int>>();
  test_dump<OrderedMultisetTree, greater<int>>();
  test_projected_dump();
#endif
  cout << "Hello World" << endl;
}
