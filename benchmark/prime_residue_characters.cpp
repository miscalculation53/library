#include "math/prime/sieve/multiplicative_prefix_sum.hpp"
#include "math/modint/modint.hpp"

// Comparison experiment; no public library API is added.
// All methods use the same N^(2/3) sieve schedule. K=0 counts, K=1 sums primes.
using Mint = static_modint32<2013265921>;

struct CharacterTable
{
  int q, exponent = 1;
  vector<int> units;
  vector<vector<Mint>> values, inverse;
  explicit CharacterTable(int q) : q(q)
  {
    for (int a = 1; a < q; ++a) if (gcd(a, q) == 1)
    {
      units.push_back(a);
      int x = a, order = 1;
      while (x != 1) x = x * a % q, ++order;
      exponent = lcm(exponent, order);
    }
    assert((Mint::mod() - 1) % exponent == 0);
    Mint root;
    for (int g = 1;; ++g)
    {
      root = Mint(g).pow((Mint::mod() - 1) / exponent);
      bool ok = true;
      for (int d = 1; d < exponent; ++d)
        if (exponent % d == 0 && root.pow(d) == Mint(1)) ok = false;
      if (ok) break;
    }
    vector<Mint> roots(exponent);
    for (int e = 0; e < exponent; ++e) roots[e] = root.pow(e);
    auto extend = [&](auto &&self, const vector<int> &chi) -> void
    {
      int g = -1;
      for (int a : units) if (chi[a] == -1) { g = a; break; }
      if (g == -1)
      {
        vector<Mint> v(q), inv(q);
        for (int a : units)
          v[a] = roots[chi[a]], inv[a] = roots[(exponent - chi[a]) % exponent];
        values.push_back(v); inverse.push_back(inv);
        return;
      }
      int h = g, e = 1;
      while (chi[h] == -1) h = h * g % q, ++e;
      for (int b = 0; b < exponent; ++b) if (e * b % exponent == chi[h])
      {
        auto next = chi;
        int power = 1;
        for (int j = 0; j < e; ++j, power = power * g % q)
          for (int a : units) if (chi[a] != -1)
            next[a * power % q] = (chi[a] + j * b) % exponent;
        self(self, next);
      }
    };
    vector<int> start(q, -1); start[1] = 0;
    extend(extend, start);
    assert(values.size() == units.size());
    for (const auto &chi : values)
      for (int a = 0; a < q; ++a) for (int b = 0; b < q; ++b)
        assert(chi[a * b % q] == chi[a] * chi[b]);
    for (int a : units) for (int b : units)
    {
      Mint sum;
      for (int j = 0; j < int(values.size()); ++j) sum += values[j][a] * inverse[j][b];
      assert(sum == Mint(a == b ? units.size() : 0));
    }
  }
};

struct Basis
{
  int q, dim;
  bool character;
  vector<int> residues, index, destination;
  vector<Mint> chi, period, first_moment;
  Basis(const CharacterTable &table, string method, int selected = -1)
    : q(table.q), character(method == "characters")
  {
    if (character)
    {
      dim = selected == -1 ? int(table.units.size()) : 1;
      chi.resize(q * dim);
      for (int r = 0; r < q; ++r) for (int j = 0; j < dim; ++j)
        chi[r * dim + j] = table.values[selected == -1 ? j : selected][r];
      period.resize((q + 1) * dim); first_moment.resize((q + 1) * dim);
      for (int r = 1; r <= q; ++r) for (int j = 0; j < dim; ++j)
      {
        period[r * dim + j] = period[(r - 1) * dim + j] + chi[(r % q) * dim + j];
        first_moment[r * dim + j] = first_moment[(r - 1) * dim + j] + Mint(r) * chi[(r % q) * dim + j];
      }
    }
    else
    {
      if (method == "full") for (int r = 0; r < q; ++r) residues.push_back(r);
      else residues = table.units;
      dim = int(residues.size());
      index.assign(q, -1);
      for (int j = 0; j < dim; ++j) index[residues[j]] = j;
      destination.resize(q * dim);
      for (int r = 0; r < q; ++r) for (int j = 0; j < dim; ++j)
        destination[r * dim + j] = index[r * residues[j] % q];
    }
  }
  bool active(int r) const { return dim == q || gcd(r, q) == 1; }
  template <int K> void prefix(ll x, Mint *out) const
  {
    if (!character)
    {
      for (int j = 0; j < dim; ++j)
      {
        const int first = residues[j] == 0 ? q : residues[j];
        const ll count = x < first ? 0 : (x - first) / q + 1;
        if constexpr (K == 0) out[j] = Mint(count);
        else out[j] = Mint(count) * (Mint(2 * first) + Mint(count - 1) * Mint(q)) * Mint::raw((Mint::mod() + 1) / 2);
      }
    }
    else
    {
      const ll blocks = x / q; const int rem = int(x % q);
      const Mint b(blocks), shift = b * Mint(q), triangle = b * Mint(blocks - 1) * Mint::raw((Mint::mod() + 1) / 2) * Mint(q);
      for (int j = 0; j < dim; ++j)
      {
        if constexpr (K == 0) out[j] = b * period[q * dim + j] + period[rem * dim + j];
        else out[j] = triangle * period[q * dim + j] + b * first_moment[q * dim + j]
          + shift * period[rem * dim + j] + first_moment[rem * dim + j];
      }
    }
  }
  template <int K> void subtract_product(Mint *to, const Mint *from, int p, const Mint *factors) const
  {
    const int r = p % q;
    if (!character)
    {
      for (int j = 0; j < dim; ++j)
      {
        if constexpr (K == 0) to[destination[r * dim + j]] -= from[j];
        else to[destination[r * dim + j]] -= Mint(p) * from[j];
      }
    }
    else
    {
      for (int j = 0; j < dim; ++j)
      {
        const Mint c = factors[j];
        // Real signs are implemented without general multiplication.
        if constexpr (K == 0)
        {
          if (c == Mint(1)) to[j] -= from[j];
          else if (c == Mint(-1)) to[j] += from[j];
          else to[j] -= c * from[j];
        }
        else to[j] -= c * from[j];
      }
    }
  }
  template <int K> void add_point(Mint *to, ll v) const
  {
    const int r = int(v % q);
    const Mint weight = K == 0 ? Mint(1) : Mint(v);
    if (!character) to[index[r]] += weight;
    else for (int j = 0; j < dim; ++j) to[j] += weight * chi[r * dim + j];
  }
};

// Same cutoffs, composite enumeration, and Fenwick schedule as
// internal::prime_prefix_sum_sieve_2_3. Known monomials permit a sparse Fenwick update.
template <int K>
vector<Mint> sieve(ll n, const Basis &basis)
{
  const int dim = basis.dim, root = int(iroot(n, 2));
  const int large = int(n / (root + 1)), cut = int(iroot(n, 6)), cube = int(iroot(n, 3));
  const int top = min(large, cube), size = root + large, bitsize = size - top;
  const ll bound = n / (top + 1);
  internal::dirichlet_prefix_sum_index coordinates(n);
  vector<Mint> dp(size_t(size) * dim), bit(size_t(bitsize + 1) * dim);
  vector<Mint> one(dim), before(dim), work(dim), factors(dim), point(dim);
  basis.prefix<K>(1, one.data());
  auto at = [&](ll x) { return dp.data() + size_t(coordinates.index(x)) * dim; };
  for (int i = 0; i < size; ++i)
  {
    Mint *row = dp.data() + size_t(i) * dim;
    basis.prefix<K>(coordinates.value(i), row);
    for (int j = 0; j < dim; ++j) row[j] -= one[j];
  }
  auto delta_subtract = [&](ll x, Mint *row)
  {
    for (int i = coordinates.index(x) + 1; i > 0; i -= i & -i)
      for (int j = 0; j < dim; ++j) row[j] -= bit[size_t(i) * dim + j];
  };
  auto prepare = [&](int p)
  {
    copy_n(at(p - 1), dim, before.begin());
    if (basis.character) for (int j = 0; j < dim; ++j)
      factors[j] = basis.chi[(p % basis.q) * dim + j] * (K == 0 ? Mint(1) : Mint(p));
  };
  auto lucy = [&](int p)
  {
    if (!basis.active(p % basis.q)) return;
    prepare(p);
    auto update = [&](ll x)
    {
      const Mint *source = at(x / p);
      for (int j = 0; j < dim; ++j) work[j] = source[j] - before[j];
      basis.subtract_product<K>(at(x), work.data(), p, factors.data());
    };
    for (int i = 1; i <= large && n / i >= ll(p) * p; ++i) update(n / i);
    for (ll x = root; x >= ll(p) * p; --x) update(x);
  };
  const auto primes = LinearSieve::primes(root);
  size_t pi = 0;
  while (pi < primes.size<size_t>() && primes[pi] <= cut) lucy(primes[pi++]);
  for (; pi < primes.size<size_t>() && primes[pi] <= cube; ++pi)
  {
    const int p = primes[pi];
    if (!basis.active(p % basis.q)) continue;
    prepare(p);
    for (int i = 1; i <= top && n / i >= ll(p) * p; ++i)
    {
      const ll x = n / i, y = x / p;
      copy_n(at(y), dim, work.begin());
      if (y <= bound) delta_subtract(y, work.data());
      for (int j = 0; j < dim; ++j) work[j] -= before[j];
      basis.subtract_product<K>(at(x), work.data(), p, factors.data());
    }
    auto dfs = [&](auto &&self, ll v, size_t first) -> void
    {
      if (v != p)
      {
        if (basis.character)
        {
          fill(point.begin(), point.end(), Mint());
          basis.add_point<K>(point.data(), v);
          for (int i = coordinates.index(v) + 1; i <= bitsize; i += i & -i)
            for (int j = 0; j < dim; ++j) bit[size_t(i) * dim + j] += point[j];
        }
        else
        {
          const int coordinate = basis.index[v % basis.q];
          const Mint weight = K == 0 ? Mint(1) : Mint(v);
          for (int i = coordinates.index(v) + 1; i <= bitsize; i += i & -i)
            bit[size_t(i) * dim + coordinate] += weight;
        }
      }
      for (size_t j = first; j < primes.size<size_t>() && primes[j] <= bound / v; ++j)
        if (basis.active(primes[j] % basis.q)) self(self, v * primes[j], j);
    };
    dfs(dfs, p, pi);
  }
  for (int i = 0; i < bitsize; ++i)
    delta_subtract(coordinates.value(i), dp.data() + size_t(i) * dim);
  while (pi < primes.size<size_t>() && primes[pi] <= root) lucy(primes[pi++]);
  return dp;
}

template <int K>
vector<Mint> run(ll n, const CharacterTable &table, const string &method, bool all_points)
{
  const int q = table.q, phi = int(table.units.size());
  internal::dirichlet_prefix_sum_index coordinates(n);
  const int points = all_points ? coordinates.size() : 1;
  vector<Mint> answer(size_t(points) * q);
  const Mint invphi = Mint(phi).inv();
  auto extract = [&](const vector<Mint> &dp, const Basis &basis, int selected)
  {
    for (int t = 0; t < points; ++t)
    {
      const int at = all_points ? t : coordinates.size() - 1;
      const Mint *row = dp.data() + size_t(at) * basis.dim;
      if (!basis.character)
        for (int j = 0; j < basis.dim; ++j) answer[size_t(t) * q + basis.residues[j]] = row[j];
      else
        for (int a : table.units) for (int j = 0; j < basis.dim; ++j)
          answer[size_t(t) * q + a] += row[j] * table.inverse[selected == -1 ? j : selected][a] * invphi;
    }
  };
  if (method == "characters_sequential")
  {
    for (int j = 0; j < phi; ++j)
    {
      Basis basis(table, "characters", j);
      extract(sieve<K>(n, basis), basis, j);
    }
  }
  else
  {
    Basis basis(table, method);
    extract(sieve<K>(n, basis), basis, -1);
  }
  if (method != "full")
    for (int p = 2; p <= q; ++p) if (q % p == 0)
    {
      bool prime = true;
      for (int d = 2; d * d <= p; ++d) if (p % d == 0) prime = false;
      if (prime) for (int t = 0; t < points; ++t)
        if ((all_points ? coordinates.value(t) : n) >= p)
          answer[size_t(t) * q + p % q] += K == 0 ? Mint(1) : Mint(p);
    }
  return answer;
}

template <int K> void check()
{
  for (int q : {3, 5, 6, 7, 8, 9, 12, 16, 24, 30})
  {
    CharacterTable table(q);
    for (ll n : {1, 2, 3, 63, 64, 65, 997, 10000, 1000000})
    {
      LinearSieve::reserve(int(iroot(n, 2)));
      internal::dirichlet_prefix_sum_index coords(n);
      vector<bool> composite(n + 1);
      vector<Mint> expected(size_t(coords.size()) * q), current(q);
      int at = 0;
      for (int x = 1; x <= n; ++x)
      {
        if (x >= 2 && !composite[x])
        {
          current[x % q] += K == 0 ? Mint(1) : Mint(x);
          for (ll y = ll(x) * x; y <= n; y += x) composite[y] = true;
        }
        if (at < coords.size() && x == coords.value(at))
          copy(current.begin(), current.end(), expected.begin() + size_t(at++) * q);
      }
      for (string method : {"full", "units", "characters", "characters_sequential"})
        if (run<K>(n, table, method, true) != expected)
          throw runtime_error("mismatch q=" + to_string(q) + " n=" + to_string(n) + " method=" + method);
    }
  }
}

int main(int argc, char **argv)
{
  if (argc == 2 && string(argv[1]) == "check")
  {
    check<0>(); check<1>();
    cout << "All quotient positions: 720 cases passed.\n";
    return 0;
  }
  if (argc != 5) { cerr << "N q K full|units|characters|characters_sequential\n"; return 1; }
  const ll n = stoll(argv[1]); const int q = stoi(argv[2]), k = stoi(argv[3]);
  const string method = argv[4];
  if (n < 1 || q < 2 || (k != 0 && k != 1)
      || (method != "full" && method != "units" && method != "characters" && method != "characters_sequential")) return 1;
  const auto begin = chrono::steady_clock::now();
  CharacterTable table(q);
  LinearSieve::reserve(int(iroot(n, 2)));
  const auto values = k == 0 ? run<0>(n, table, method, false) : run<1>(n, table, method, false);
  const double seconds = chrono::duration<double>(chrono::steady_clock::now() - begin).count();
  cerr << fixed << setprecision(6) << seconds << '\n';
  for (int r = 0; r < q; ++r) cout << (r ? " " : "") << values[r].val();
  cout << '\n';
}
