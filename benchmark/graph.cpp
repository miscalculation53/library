#include "template/template_all_but_modint.hpp"
#include "graph/graph.hpp"

struct OldGraph
{
  vc<int> start;
  vc<Edge<ll>> es;

  OldGraph(int n, const vc<tuple<int, int, ll>> &input) : start(n + 1), es(input.size())
  {
    fec([ u, v, w ] : input) start[u + 1]++;
    repi(v, n) start[v + 1] += start[v];
    auto cnt = start;
    repi(i, input.size())
    {
      auto [u, v, w] = input[i];
      es[cnt[u]++] = Edge<ll>(u, v, w, i);
    }
  }
};

template <class F>
double elapsed_ms(F f)
{
  auto beg = chrono::steady_clock::now();
  f();
  return chrono::duration<double, milli>(chrono::steady_clock::now() - beg).count();
}

int main(int argc, char **argv)
{
  int n = argc >= 2 ? atoi(argv[1]) : 500'000;
  int m = argc >= 3 ? atoi(argv[2]) : 4'000'000;
  vc<tuple<int, int, ll>> es(m);
  repi(i, m)
  {
    int u = int((uint64_t(i) * 11995408973635179863ULL) % n);
    int v = int((uint64_t(i) * 10150724397891781847ULL + 1) % n);
    es[i] = {u, v, ll(i % 1000)};
  }

  unique_ptr<OldGraph> old;
  double old_build = elapsed_ms([&] { old = make_unique<OldGraph>(n, es); });
  unique_ptr<GraphDirected<ll>> graph;
  double new_build = elapsed_ms([&] { graph = make_unique<GraphDirected<ll>>(n, es); });

  volatile ll checksum = 0;
  auto scan_old = [&]
  {
    repi(_, 5) repi(v, n) for (int i = old->start[v]; i < old->start[v + 1]; i++)
    {
      cauto &e = old->es[i];
      checksum += e.from + e.to + e.cost + e.index;
    }
  };
  auto scan_edges = [&]
  {
    repi(_, 5) repi(v, n) fec(e : graph->out_edges(v)) checksum += e.from + e.to + e.cost + e.index;
  };
  auto scan_arcs = [&]
  {
    repi(_, 5) repi(v, n) fec(e : graph->out_arcs(v)) checksum += v + e.to + e.cost + e.index;
  };
  double old_scan = elapsed_ms(scan_old);
  double edge_scan = elapsed_ms(scan_edges);
  double arc_scan = elapsed_ms(scan_arcs);

  cout << "n=" << n << " m=" << m << '\n';
  cout << "old payload: " << sizeof(Edge<ll>) * size_t(m) / (1 << 20) << " MiB\n";
  cout << "new payload: " << (sizeof(int) * 2 + sizeof(ll)) * size_t(m) / (1 << 20) << " MiB\n";
  cout << fixed << setprecision(3);
  cout << "old build: " << old_build << " ms, scan x5: " << old_scan << " ms\n";
  cout << "new build: " << new_build << " ms\n";
  cout << "out_edges scan x5: " << edge_scan << " ms\n";
  cout << "out_arcs scan x5: " << arc_scan << " ms\n";
  cerr << checksum << '\n';
}
