#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "../../template/template_bit.hpp"
#include "../../template/template_dump.hpp"
#include "../../template/template_algo.hpp"

void test1()
{
  vc<ll> width_model = {0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 40};
  vc<ll> floor_model = {0, 1, 2, 2, 4, 4, 4, 4, 8, 8, 1LL << 39};
  vc<ll> ceil_model = {1, 1, 2, 4, 4, 8, 8, 8, 8, 16, 1LL << 40};
  vc<ll> pp_model = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 20};
  vc<ll> single_model = {0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0};
  vc<ll> width, floor, ceil, pp, single;
  fec(k : vl{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0b1010101010101010101010101010101010101010LL})
  {
    width.push_back(bit_width(k));
    floor.push_back(bit_floor(k));
    ceil.push_back(bit_ceil(k));
    pp.push_back(popcount(k));
    single.push_back(has_single_bit(k));
  }
  assert(width == width_model);
  assert(floor == floor_model);
  assert(ceil == ceil_model);
  assert(pp == pp_model);
  assert(single == single_model);
}

void test2()
{
  vc<ll> crz_model = {0, 1, 0, 2, 0, 1, 0, 3, 0, 1};
  vc<ll> lp_model = {0, 1, 0, 2, 0, 1, 0, 3, 0, 1};
  vc<ll> lm_model = {1, 2, 1, 4, 1, 2, 1, 8, 1, 2};
  vc<ll> mp_model = {0, 1, 1, 2, 2, 2, 2, 3, 3, 39};
  vc<ll> mm_model = {1, 2, 2, 4, 4, 4, 4, 8, 8, 1LL << 39};
  vc<ll> crz, lp, lm, mp, mm;
  fec(k : vl{1, 2, 3, 4, 5, 6, 7, 8, 9, 0b1010101010101010101010101010101010101010LL})
  {
    crz.push_back(countr_zero(k));
    lp.push_back(lsb_pos(k));
    mp.push_back(msb_pos(k));
    lm.push_back(lsb_mask(k));
    mm.push_back(msb_mask(k));
  }
  assert(crz == crz_model);
  assert(lp == lp_model);
  assert(lm == lm_model);
  assert(mp == mp_model);
  assert(mm == mm_model);
}

void test3()
{
  ll x = 0b1001;
  assert(btest(x, 0));
  assert(!btest(x, 1));
  bset(x, 1);
  assert(btest(x, 1));
  bset(x, 0, 0);
  assert(!btest(x, 0));
  ll y(x);
  bflip(y, 2);
  bflip(y, 3);
  assert(btest(x, 2) != btest(y, 2));
  assert(btest(x, 3) != btest(y, 3));
}

int main()
{
  test1();
  test2();
  test3();

  cout << "Hello World" << endl;
}