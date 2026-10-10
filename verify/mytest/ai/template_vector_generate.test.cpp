#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/meldable_integer_set.hpp"
#include "ds/meldable_heap.hpp"

struct Token
{
  int value;
  Token() = delete;
  explicit Token(int value) : value(value) {}
  Token(const Token &) = delete;
  Token &operator=(const Token &) = delete;
  Token(Token &&) noexcept = default;
  Token &operator=(Token &&) = delete;
};

int main()
{
  vc<int> calls;
  auto values = gen_vec(20, [&](int i)
  {
    calls.eb(i);
    return Token(i * i);
  });
  repi(i, 20) assert(calls[i] == i && values[i].value == i * i);
  assert(SZ(calls) == 20);
  auto empty = gen_vec(0, [&](int)
  {
    assert(false);
    return Token(0);
  });
  assert(empty.empty());
  auto pointers = GEN_VEC(10, i, make_unique<int>(i));
  repi(i, 10) assert(*pointers[i] == i);
  auto bits = GEN_VEC(7, i, i % 2 == 0);
  static_assert(is_same_v<decltype(bits), vc<bool>>);
  repi(i, 7) assert(bits[i] == (i % 2 == 0));
  assert(stov("ABCBA", 'A') == vl({0, 1, 2, 1, 0}));
  assert(stov("JOJO", "JOI") == vl({0, 1, 0, 1}));

  MeldableIntegerSetPool pool(100, 10);
  auto sets = gen_vec(10, [&](int) { return pool.make_set(); });
  assert(pool.node_count() == 0);
  sets[0].insert(3);
  assert(sets[1].empty());
  sets[1].insert(70);
  sets[0].merge(sets[1]);
  assert(sets[0].size() == 2 && sets[1].empty());
  auto singletons = GEN_VEC(7, i, pool.make_set(i));
  repi(i, 7) assert(singletons[i].size() == 1 && singletons[i].contains(i));

  MeldableHeapPool<int> heap_pool;
  auto heaps = gen_vec(3, [&](int) { return heap_pool.make_heap(); });
  heaps[0].push(7);
  heaps[1].push(3);
  heaps[0].merge(heaps[1]);
  assert(heaps[0].top() == 3 && heaps[1].empty() && heaps[2].empty());
  PRINT("Hello World");
}
