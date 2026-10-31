#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/segtree/segtree.hpp"
#include "math/linalg/matrix_array.hpp"

struct CountedConcat
{
  using S = string;
  static inline int operations = 0;
  static S e() { return {}; }
  static S op(const S &a, const S &b)
  {
    ++operations;
    return a + b;
  }
};

struct AppendAction
{
  int calls = 0;
  AppendAction() = default;
  AppendAction(const AppendAction &) = delete;
  string operator()(const string &x, const string &a)
  {
    ++calls;
    return x + a;
  }
};

struct OwnedText
{
  unique_ptr<string> value;
  explicit OwnedText(string text) : value(make_unique<string>(move(text))) {}
};

void test_order_and_updates()
{
  mt19937 rng(20261026);
  for (int n = 0; n <= 65; ++n)
  {
    vc<string> a(n);
    repi(i, n) a[i] = string(1 + rng() % 3, 'a' + rng() % 26);
    SegmentTree<CountedConcat> seg(a);
    repi(round, 3)
    {
      if (n > 0)
      {
        int p = rng() % n;
        a[p] = string(rng() % 4, 'a' + rng() % 26);
        seg.set(p, a[p]);
      }
      const auto &read = seg;
      repi(l, n + 1) repi(r, l, n + 1)
      {
        string expected = "initial:";
        repi(i, l, r) expected += a[i];
        AppendAction apply;
        CountedConcat::operations = 0;
        assert(read.prod_apply(l, r, string("initial:"), apply) == expected);
        assert(CountedConcat::operations == 0);
        if (l == r) assert(apply.calls == 0);
        else assert(0 < apply.calls && apply.calls <= 2 * int(bit_width(unsigned(n))));
      }
    }

    // An rvalue callback may hold a resource with unique ownership.
    auto result = seg.prod_apply(0, n, string{},
        [counter = make_unique<int>(0)](const string &x, const string &s) mutable
        {
          ++*counter;
          return x + s;
        });
    assert(result == accumulate(a.begin(), a.end(), string{}));
  }
}

void test_shared_callback_and_nested_reads()
{
  const SegmentTree<CountedConcat> seg(vc<string>{"a", "b", "c", "d", "e"});
  int calls = 0;
  auto apply = [&](const string &x, const string &s)
  {
    ++calls;
    assert(seg.prod(0, 5) == "abcde");
    return x + s;
  };
  assert(seg.prod_apply(1, 5, string("x"), apply) == "xbcde");
  int previous = calls;
  assert(previous > 0);
  assert(seg.prod_apply(0, 1, string("y"), apply) == "ya");
  assert(calls == previous + 1);

  // The accumulator needs neither default construction nor a monoid operation.
  static_assert(!is_default_constructible_v<OwnedText> && !is_copy_constructible_v<OwnedText>);
  auto append = [](OwnedText &x, const string &s)
  {
    *x.value += s;
    return move(x);
  };
  auto value = seg.prod_apply(1, 4, OwnedText("z"), append);
  assert(*value.value == "zbcd");
  value = seg.prod_apply(3, 3, move(value), append);
  assert(*value.value == "zbcd");
}

using SR = SemiRingMaxPlus<ll>;
using Mat = MatrixArray<SR, 9, 9>;
using Row = array<ll, 9>;

Mat transition(ll a)
{
  Mat result = Mat::e1();
  for (int s = -1; s <= 1; ++s)
    for (int t = -1; t <= 1; ++t)
      for (int u = -1; u <= 1; ++u)
      {
        int i = (s + 1) * 3 + t + 1, j = (t + 1) * 3 + u + 1;
        chmax(result[i][j], (3 * s + 2 * t + u) * a);
      }
  return result;
}

ll brute(const vc<ll> &a, int l, int r)
{
  ll answer = 0;
  repi(mask, 1 << (r - l))
  {
    vc<ll> b;
    repi(i, r - l) if (mask >> i & 1) b.push_back(a[l + i]);
    ll value = 0;
    for (int i = 0; i + 2 < SZ(b); ++i)
      value += abs(b[i] + 2 * b[i + 1] + 3 * b[i + 2]);
    chmax(answer, value);
  }
  return answer;
}

void test_max_plus_matrices()
{
  mt19937 rng(998244353);
  for (int n = 0; n <= 9; ++n)
  {
    vc<ll> a(n);
    vc<Mat> matrices(n);
    repi(i, n) matrices[i] = transition(a[i] = ll(rng() % 21) - 10);
    SegmentTree<MonoidMul<Mat>> seg(matrices);
    repi(iter, 100)
    {
      if (n > 0 && iter % 3 == 0)
      {
        int p = rng() % n;
        a[p] = ll(rng() % 21) - 10;
        seg.set(p, transition(a[p]));
      }
      int l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      Row initial;
      initial.fill(-INF);
      initial[4] = 0;
      auto result = seg.prod_apply(l, r, initial,
          [](const Row &v, const Mat &mat) { return v * mat; });
      auto product = initial * seg.prod(l, r);
      assert(result[4] == product[4]);
      assert(result[4] == brute(a, l, r));
    }
  }
}

int main()
{
  test_order_and_updates();
  test_shared_callback_and_nested_reads();
  test_max_plus_matrices();
  cout << "Hello World" << endl;
}
