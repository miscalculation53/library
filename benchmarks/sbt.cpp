// g++-15 -std=c++17 -O2 -I . benchmarks/sbt.cpp -o /tmp/sbt_bench
// /tmp/sbt_bench > benchmarks/sbt.csv
#include "math/sbt.hpp"
#include "detail/sbt_reference.hpp"

using Clock = chrono::steady_clock;
volatile ull sbt_checksum = 0;

template <class Node>
ull fingerprint(const Node &node)
{
  return (ull(node.p) * 998244353) ^ (ull(node.q) * 1000000007) ^
         (ull(node.r) * 1000000009) ^ (ull(node.s) * 1000000033);
}

template <class F, class G>
void measure(const string &operation, const string &type, const string &shape,
             int count, const F &before, const G &after, double before_calls = 0, double after_calls = 0)
{
  const ull expected = before();
  if (after() != expected) abort();
  array<vc<double>, 2> times;
  for (int round = 0; round < 7; ++round)
    for (int j = 0; j < 2; ++j)
    {
      int method = (round + j) % 2;
      auto start = Clock::now();
      ull actual = method == 0 ? before() : after();
      double ns = chrono::duration<double, nano>(Clock::now() - start).count() / count;
      if (actual != expected) abort();
      sbt_checksum = actual;
      times[method].push_back(ns);
    }
  for (auto &t : times) sort(t.begin(), t.end());
  cout << operation << ',' << type << ',' << shape << ',' << count << ','
       << times[0][3] << ',' << times[1][3] << ',' << times[0][3] / times[1][3] << ','
       << before_calls << ',' << after_calls << '\n';
  cout.flush();
}

template <bool Current, class T, class Wide, bool Count>
ull run_search(const vc<array<T, 3>> &inputs, ull &calls)
{
  ull sum = 0;
  for (const auto &input : inputs)
  {
    const T x = input[0], y = input[1], cap = input[2];
    auto judge = [&](T a, T b)
    {
      if constexpr (Count) ++calls;
      return Wide(a) * y <= Wide(b) * x;
    };
    if constexpr (Current) sum += fingerprint(sbt_search<T>(judge, cap));
    else sum += fingerprint(sbt_reference::sbt_search<T>(judge, cap));
  }
  return sum;
}

template <class Node, class T>
ull run_constructor(const vc<array<T, 3>> &inputs)
{
  ull sum = 0;
  for (const auto &input : inputs) sum += fingerprint(Node(input[0], input[1]));
  return sum;
}

template <class Node>
ull run_depth(const vc<Node> &nodes)
{
  ull sum = 0;
  for (const auto &node : nodes) sum += ull(node.depth());
  return sum;
}

template <class T, class Wide>
void benchmark(const string &type)
{
  mt19937_64 rng(1783);
  const T cap = sizeof(T) == 4 ? 1000000000LL : 1000000000000000000LL;
  const int count = 50000;
  for (string shape : {"random", "near_zero", "near_one", "fibonacci"})
  {
    T fa = 1, fb = 1;
    while (fa + fb <= cap)
    {
      T fc = fa + fb;
      fa = fb, fb = fc;
    }
    vc<array<T, 3>> inputs;
    for (int i = 0; i < count; ++i)
    {
      T n = cap - rng() % (cap / 10), x, y;
      if (shape == "random") x = 1 + rng() % cap, y = 1 + rng() % cap;
      else if (shape == "near_zero") x = 1 + rng() % 20, y = cap - rng() % (cap / 10);
      else if (shape == "near_one") x = cap - rng() % (cap / 10), y = x + 1 + rng() % 20;
      else x = fa, y = fb;
      inputs.push_back({x, y, n});
    }
    ull before_calls = 0, after_calls = 0, unused = 0;
    if (run_search<false, T, Wide, true>(inputs, before_calls) !=
        run_search<true, T, Wide, true>(inputs, after_calls)) abort();
    measure("search", type, shape, count,
            [&] { return run_search<false, T, Wide, false>(inputs, unused); },
            [&] { return run_search<true, T, Wide, false>(inputs, unused); },
            double(before_calls) / count, double(after_calls) / count);
    measure("constructor", type, shape, count,
            [&] { return run_constructor<sbt_reference::SBTNode<T>>(inputs); },
            [&] { return run_constructor<SBTNode<T>>(inputs); });
    vc<sbt_reference::SBTNode<T>> before_nodes;
    vc<SBTNode<T>> after_nodes;
    for (const auto &input : inputs)
    {
      before_nodes.emplace_back(input[0], input[1]);
      after_nodes.emplace_back(input[0], input[1]);
    }
    measure("depth", type, shape, count,
            [&] { return run_depth(before_nodes); }, [&] { return run_depth(after_nodes); });
  }
}

int main()
{
  cout << fixed << setprecision(3);
  cout << "operation,type,shape,count,before_ns,after_ns,speedup,before_judge_calls,after_judge_calls\n";
  benchmark<int, ll>("int32");
  benchmark<ll, i128>("int64");
}
