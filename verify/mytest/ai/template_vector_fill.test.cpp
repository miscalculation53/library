#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_vector.hpp"
#include "ds/flat_dvec.hpp"

int main()
{
  auto nested = dvec({3, 4, 5}, 0LL);
  FlatDvec flat({3, 4, 5}, 0LL);
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 4; j++)
      for (int k = 0; k < 5; k++) nested[i][j][k] = flat(i, j, k) = i + j + k;
  fill(nested, -1); // int の値を ll の要素に代入する。
  fill(flat, -1);
  fill(nested[1], 9);
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 4; j++)
      for (int k = 0; k < 5; k++)
      {
        assert(nested[i][j][k] == (i == 1 ? 9 : -1));
        assert(flat(i, j, k) == -1);
      }
  assert(SZ(nested) == 3 && SZ(nested[1]) == 4 && SZ(nested[1][2]) == 5);

  auto bits = dvec({2, 3, 4}, false);
  fill(bits, true);
  for (const auto &plane : bits)
    for (const auto &row : plane)
      for (bool b : row) assert(b);
  fill(bits, false);
  for (const auto &plane : bits)
    for (const auto &row : plane)
      for (bool b : row) assert(!b);
  FlatDvec flat_bits({2, 3}, false);
  fill(flat_bits, true);
  for (bool b : flat_bits) assert(b);

  vvc<int> ragged{{}, {1, 2}, {}, {3}};
  fill(ragged, 7);
  assert((ragged == vvc<int>{{}, {7, 7}, {}, {7}}));
  auto empty_middle = dvec({2, 0, 3}, 1);
  fill(empty_middle, 0);
  assert(empty_middle.size() == 2 && empty_middle[0].empty() && empty_middle[1].empty());
  vc<int> empty;
  fill(empty, 4);
  FlatDvec<int, 3> empty_flat;
  fill(empty_flat, 4);
  assert(empty.empty() && empty_flat.empty());

  array<array<int, 3>, 2> a{};
  int b[2][3]{};
  fill(a, 5);
  fill(b, 5);
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 3; j++) assert(a[i][j] == 5 && b[i][j] == 5);
  array<vc<array<ll, 2>>, 2> mixed{vc<array<ll, 2>>(3), vc<array<ll, 2>>(1)};
  fill(mixed, 6);
  for (const auto &rows : mixed)
    for (const auto &row : rows)
      for (ll x : row) assert(x == 6);

  // 文字列や pair を値として扱い、内部への再帰を止める。
  auto strings = dvec({2, 3}, string("old"));
  fill(strings, "new");
  for (const auto &row : strings)
    for (const auto &s : row) assert(s == "new");
  auto pairs = dvec({2, 3}, pair<int, int>{0, 0});
  fill(pairs, pair<int, int>{1, 2});
  for (const auto &row : pairs)
    for (const auto &p : row) assert((p == pair<int, int>{1, 2}));
  vvc<int> vector_values{{1}, {2, 3}};
  fill(vector_values, vc<int>{4, 5});
  assert((vector_values == vvc<int>{{4, 5}, {4, 5}}));

  // 3 引数の std::fill と同じ名前で使える。
  vc<int> one{1, 2, 3};
  fill(one.begin(), one.end(), 8);
  assert((one == vc<int>{8, 8, 8}));
  cout << "Hello World" << endl;
}
