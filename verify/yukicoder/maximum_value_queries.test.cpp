#define PROBLEM "https://yukicoder.me/problems/no/97"

#include "math/convolution/convolution.hpp"
#include "math/prime/large/order_primitive_root.hpp"

int main(int argc, char **argv)
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  constexpr int P = 100003, M = P - 1;
  int n, q;
  cin >> n >> q;
  const int block_size = argc > 1 ? stoi(argv[1]) : 2048;
  assert(block_size > 0);
  vc<unsigned char> present(P), requested(P);
  vc<int> values;
  unsigned sx = 123456789, sy = 362436069, sz = 521288629, sw = 88675123;
  repi(i, n)
  {
    const unsigned t = sx ^ (sx << 11);
    sx = sy; sy = sz; sz = sw;
    sw = sw ^ (sw >> 19) ^ (t ^ (t >> 8));
    const int v = sw % P;
    if (!present[v]) values.eb(v), present[v] = true;
  }
  vc<int> queries(q), pending, answer(P);
  for (int &x : queries)
  {
    cin >> x;
    if (x != 0 && !requested[x]) pending.eb(x), requested[x] = true;
  }

  if (ll(values.size()) * pending.size() <= 32000000)
  {
    for (int x : pending)
      for (int v : values) answer[x] = max(answer[x], int(ll(x) * v % P));
  }
  else
  {
    const int root = primitive_root(ll(P), factorize(ll(M)));
    vc<int> log(P), power(M), inverse(P);
    power[0] = 1;
    repi(i, 1, M) power[i] = ll(power[i - 1]) * root % P;
    repi(i, M) log[power[i]] = i;
    inverse[1] = 1;
    repi(x, 2, P) inverse[x] = P - ll(P / x) * inverse[P % x] % P;

    using mint = modint998244353;
    const int length = bit_ceil(2 * M - 1);
    vc<mint> f(M);
    for (int v : values) if (v != 0) f[(M - log[v]) % M] = 1;
    f.resize(length);
    ntt(f);
    const mint scale = mint(length).inv();
    for (auto &v : f) v *= scale;
    for (int block = (P - 1) / block_size; block >= 0 && !pending.empty(); --block)
    {
      const int l = max(1, block * block_size), r = min(P, (block + 1) * block_size);
      vc<mint> h(length);
      repi(v, l, r) h[log[v]] = 1;
      ntt(h);
      repi(i, length) h[i] *= f[i];
      intt(h);
      h.resize(2 * M - 1);
      int count = 0;
      for (int x : pending)
      {
        const int y = log[x];
        const mint hits = h[y] + (y + M < int(h.size()) ? h[y + M] : mint(0));
        if (hits == 0)
        {
          pending[count++] = x;
          continue;
        }
        int value = r - 1, a = ll(value) * inverse[x] % P;
        while (!present[a])
        {
          --value;
          a -= inverse[x];
          if (a < 0) a += P;
        }
        answer[x] = value;
      }
      pending.resize(count);
    }
  }
  for (int x : queries) cout << answer[x] << '\n';
}
