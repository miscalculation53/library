#include "math/prime/sieve/multiplicative_prefix_sum.hpp"
#include "math/modint/modint.hpp"

// Library Checker と同じ入力。引数: auto|sieve|sieve23。
// 比較用に internal の実装を直接呼ぶ。答えを stdout、段階ごとの合計秒数を stderr に出す。
int main(int argc, char **argv)
{
  using mint = static_modint32<469762049>;
  using R = FieldAddSubMulDiv<mint>;
  const string method = argc >= 2 ? argv[1] : "auto";
  assert(argc <= 2 && (method == "auto" || method == "sieve" || method == "sieve23"));
  array<double, 4> elapsed{};
  auto timed = [&](int stage, const auto &f)
  {
    const auto begin = chrono::steady_clock::now();
    auto result = f();
    elapsed[stage] += chrono::duration<double>(chrono::steady_clock::now() - begin).count();
    return result;
  };
  INT(T);
  const int cases = T;
  ll max_n = 0, max_k = 0;
  while (T--)
  {
    LL(N, a, b);
    max_n = max(max_n, N);
    const int root = int(iroot(N, 2));
    max_k = max<ll>(max_k, root);
    auto input = timed(0, [&]
    {
      return make_pair(DirichletPrefixSum<R>(N, zeta_prefix_sum),
                       DirichletPrefixSum<R>(N, id_prefix_sum));
    });
    const auto prime_sum_of = [&](const auto &values)
    {
      if (method == "sieve") return internal::prime_prefix_sum_sieve(values);
      if (method == "sieve23") return internal::prime_prefix_sum_sieve_2_3(values);
      return prime_prefix_sum(values);
    };
    auto count = timed(1, [&] { return prime_sum_of(input.first); });
    auto sum = timed(2, [&] { return prime_sum_of(input.second); });
    auto result = timed(3, [&]
    {
      const auto prime_sum = count * mint(a) + sum * mint(b);
      const auto primepower = [&](const auto &q) { return mint(a) * q.e + mint(b) * q.p; };
      if (method == "sieve") return internal::multiplicative_prefix_sum_sieve(prime_sum, primepower);
      if (method == "sieve23") return internal::multiplicative_prefix_sum_sieve_2_3(prime_sum, primepower);
      return multiplicative_prefix_sum(prime_sum, primepower);
    });
    PRINT(result.F(N));
  }
  cerr << fixed << setprecision(6)
       << "method=" << method << " cases=" << cases << " max_n=" << max_n << " max_k=" << max_k
       << " init_s=" << elapsed[0] << " prime_count_s=" << elapsed[1]
       << " prime_sum_s=" << elapsed[2] << " multiplicative_sum_s=" << elapsed[3] << '\n';
}
