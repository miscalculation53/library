#include "template/template_all_but_modint.hpp"

#include "ds/my_deque.hpp"

namespace allocation_tracker
{
struct Header
{
  void *raw;
  size_t size;
};

size_t current = 0, peak = 0;

void *allocate(size_t size, size_t align)
{
  align = max(align, alignof(Header));
  void *raw = malloc(size + align - 1 + sizeof(Header));
  if (!raw) throw bad_alloc();
  uintptr_t p = reinterpret_cast<uintptr_t>(raw) + sizeof(Header);
  p = (p + align - 1) / align * align;
  auto *header = reinterpret_cast<Header *>(p) - 1;
  header->raw = raw, header->size = size;
  current += size, peak = max(peak, current);
  return reinterpret_cast<void *>(p);
}

void deallocate(void *p) noexcept
{
  if (!p) return;
  auto *header = reinterpret_cast<Header *>(p) - 1;
  current -= header->size;
  free(header->raw);
}
} // namespace allocation_tracker

void *operator new(size_t size) { return allocation_tracker::allocate(size, alignof(max_align_t)); }
void *operator new[](size_t size) { return allocation_tracker::allocate(size, alignof(max_align_t)); }
void *operator new(size_t size, align_val_t align)
{
  return allocation_tracker::allocate(size, size_t(align));
}
void *operator new[](size_t size, align_val_t align)
{
  return allocation_tracker::allocate(size, size_t(align));
}
void operator delete(void *p) noexcept { allocation_tracker::deallocate(p); }
void operator delete[](void *p) noexcept { allocation_tracker::deallocate(p); }
void operator delete(void *p, size_t) noexcept { allocation_tracker::deallocate(p); }
void operator delete[](void *p, size_t) noexcept { allocation_tracker::deallocate(p); }
void operator delete(void *p, align_val_t) noexcept { allocation_tracker::deallocate(p); }
void operator delete[](void *p, align_val_t) noexcept { allocation_tracker::deallocate(p); }
void operator delete(void *p, size_t, align_val_t) noexcept { allocation_tracker::deallocate(p); }
void operator delete[](void *p, size_t, align_val_t) noexcept { allocation_tracker::deallocate(p); }

volatile ull benchmark_sink = 0;

template <class D>
void reserve_if_possible(D &deq, int n)
{
  if constexpr (requires { deq.reserve(n); })
    deq.reserve(n);
}

template <class F>
double median_ms(F f)
{
  constexpr int rounds = 5;
  array<double, rounds> times;
  repi(i, rounds)
  {
    auto begin = chrono::steady_clock::now();
    benchmark_sink ^= f();
    auto end = chrono::steady_clock::now();
    times[i] = chrono::duration<double, milli>(end - begin).count();
  }
  sort(ALL(times));
  return times[rounds / 2];
}

template <class D>
ull queue_workload(int n, bool reserve)
{
  D deq;
  if (reserve) reserve_if_possible(deq, n);
  repi(i, n) deq.push_back(i);
  ull sum = 0;
  while (!deq.empty())
    sum += deq.front(), deq.pop_front();
  return sum;
}

template <class D>
ull bounded_queue_workload(int n)
{
  D deq;
  ull sum = 0;
  repi(i, n)
  {
    deq.push_back(i);
    if (deq.size() > 1024)
      sum += deq.front(), deq.pop_front();
  }
  while (!deq.empty())
    sum += deq.front(), deq.pop_front();
  return sum;
}

template <class D>
ull mixed_workload(const vc<unsigned char> &ops)
{
  D deq;
  ull sum = 0;
  repi(i, ops.size())
  {
    int op = deq.empty() ? ops[i] & 1 : ops[i] & 3;
    if (op == 0)
      deq.push_front(i);
    else if (op == 1)
      deq.push_back(i);
    else if (op == 2)
      sum += deq.front(), deq.pop_front();
    else
      sum += deq.back(), deq.pop_back();
  }
  repi(i, deq.size()) sum += deq[i];
  return sum;
}

template <class D>
ull random_access_workload(const vc<int> &indices)
{
  D deq;
  reserve_if_possible(deq, 1'000'000);
  repi(i, 1'000'000) deq.push_back(i);
  ull sum = 0;
  for (int i : indices) sum += deq[i];
  return sum;
}

struct BFS01Graph
{
  vc<int> indptr, to;
  vc<unsigned char> cost;
};

BFS01Graph make_bfs01_graph(int n, int m, mt19937 &rng)
{
  vc<tuple<int, int, unsigned char>> es;
  es.reserve(m);
  repi(v, 1, n) es.eb(rng() % v, v, rng() & 1);
  while (SZ<int>(es) < m) es.eb(rng() % n, rng() % n, rng() & 1);

  BFS01Graph g;
  g.indptr.assign(n + 1, 0), g.to.resize(m), g.cost.resize(m);
  for (auto [u, v, w] : es) g.indptr[u + 1]++;
  repi(v, n) g.indptr[v + 1] += g.indptr[v];
  vc<int> pos = g.indptr;
  for (auto [u, v, w] : es)
  {
    int i = pos[u]++;
    g.to[i] = v, g.cost[i] = w;
  }
  return g;
}

template <class D>
ull bfs01_workload(const BFS01Graph &g, bool reserve)
{
  const int n = SZ<int>(g.indptr) - 1;
  vc<int> dist(n, INT_MAX);
  D deq;
  if (reserve) reserve_if_possible(deq, n);
  dist[0] = 0, deq.push_back(0);
  while (!deq.empty())
  {
    int v = deq.front();
    deq.pop_front();
    repi(i, g.indptr[v], g.indptr[v + 1])
    {
      int to = g.to[i], nd = dist[v] + g.cost[i];
      if (nd < dist[to])
      {
        dist[to] = nd;
        if (g.cost[i] == 0)
          deq.push_front(to);
        else
          deq.push_back(to);
      }
    }
  }
  ull sum = 0;
  for (int d : dist) sum += d;
  return sum;
}

ull bfs01_two_vectors_workload(const BFS01Graph &g)
{
  const int n = SZ<int>(g.indptr) - 1;
  vc<int> dist(n, INT_MAX), cur{0}, nxt;
  vc<unsigned char> used(n, false);
  dist[0] = 0;
  while (!cur.empty() || !nxt.empty())
  {
    if (cur.empty()) cur.swap(nxt);
    int v = cur.back();
    cur.pop_back();
    if (used[v]) continue;
    used[v] = true;
    repi(i, g.indptr[v], g.indptr[v + 1])
    {
      int to = g.to[i], nd = dist[v] + g.cost[i];
      if (nd < dist[to])
      {
        dist[to] = nd;
        (g.cost[i] == 0 ? cur : nxt).eb(to);
      }
    }
  }
  ull sum = 0;
  for (int d : dist) sum += d;
  return sum;
}

ull dial01_workload(const BFS01Graph &g)
{
  const int n = SZ<int>(g.indptr) - 1;
  vc<int> dist(n, INT_MAX), head(2, -1), next(n, -2), prev(n, -2);
  int size = 0;
  auto erase = [&](int v, int b)
  {
    int p = prev[v], q = next[v];
    if (p == -1)
      head[b] = q;
    else
      next[p] = q;
    if (q != -1)
      prev[q] = p;
    next[v] = prev[v] = -2, size--;
  };
  auto push = [&](int v, int b)
  {
    next[v] = head[b], prev[v] = -1;
    if (head[b] != -1)
      prev[head[b]] = v;
    head[b] = v, size++;
  };

  int cur = 0;
  dist[0] = 0, push(0, 0);
  while (size)
  {
    while (head[cur & 1] == -1) cur++;
    int b = cur & 1, v = head[b];
    erase(v, b);
    repi(i, g.indptr[v], g.indptr[v + 1])
    {
      int to = g.to[i], nd = dist[v] + g.cost[i];
      if (nd < dist[to])
      {
        if (prev[to] != -2)
          erase(to, dist[to] & 1);
        dist[to] = nd, push(to, nd & 1);
      }
    }
  }
  ull sum = 0;
  for (int d : dist) sum += d;
  return sum;
}

struct MemoryResult
{
  size_t live, peak;
};

template <class D, class T>
MemoryResult one_deque_memory(int n, bool reserve)
{
  const size_t base = allocation_tracker::current;
  allocation_tracker::peak = base;
  MemoryResult res;
  {
    D deq;
    if (reserve) reserve_if_possible(deq, n);
    repi(i, n) deq.push_back(T{});
    res = {allocation_tracker::current - base, allocation_tracker::peak - base};
  }
  assert(allocation_tracker::current == base);
  return res;
}

template <class D>
MemoryResult many_deques_memory(int count, bool push_one)
{
  const size_t base = allocation_tracker::current;
  allocation_tracker::peak = base;
  MemoryResult res;
  {
    vc<D> deqs(count);
    if (push_one)
      for (D &deq : deqs) deq.push_back(1);
    res = {allocation_tracker::current - base, allocation_tracker::peak - base};
  }
  assert(allocation_tracker::current == base);
  return res;
}

void print_time(string_view name, double std_ms, double own_ms)
{
  cout << left << setw(28) << name << right << fixed << setprecision(2)
       << " std " << setw(9) << std_ms << " ms"
       << "  MyDeque " << setw(9) << own_ms << " ms"
       << "  ratio " << setw(6) << own_ms / std_ms << '\n';
}

void print_memory(string_view name, MemoryResult std_mem, MemoryResult own_mem)
{
  auto mib = [](size_t x) { return double(x) / (1 << 20); };
  cout << left << setw(28) << name << right << fixed << setprecision(2)
       << " std " << setw(7) << mib(std_mem.live) << '/' << setw(7) << mib(std_mem.peak) << " MiB"
       << "  MyDeque " << setw(7) << mib(own_mem.live) << '/' << setw(7) << mib(own_mem.peak) << " MiB\n";
}

int main()
{
  mt19937 rng(123456789);
  vc<unsigned char> ops(10'000'000);
  for (auto &op : ops) op = rng();
  vc<int> indices(10'000'000);
  for (int &i : indices) i = rng() % 1'000'000;
  BFS01Graph bfs_graph = make_bfs01_graph(300'000, 2'000'000, rng);
  ull bfs_expected = bfs01_workload<deque<int>>(bfs_graph, false);
  if (bfs01_workload<MyDeque<int>>(bfs_graph, false) != bfs_expected ||
      bfs01_two_vectors_workload(bfs_graph) != bfs_expected ||
      dial01_workload(bfs_graph) != bfs_expected)
    abort();

  cout << "time (median of 5 rounds)\n";
  print_time("queue 5M, no reserve",
             median_ms([] { return queue_workload<deque<int>>(5'000'000, false); }),
             median_ms([] { return queue_workload<MyDeque<int>>(5'000'000, false); }));
  print_time("queue 5M, reserve",
             median_ms([] { return queue_workload<deque<int>>(5'000'000, true); }),
             median_ms([] { return queue_workload<MyDeque<int>>(5'000'000, true); }));
  print_time("bounded queue 20M",
             median_ms([] { return bounded_queue_workload<deque<int>>(20'000'000); }),
             median_ms([] { return bounded_queue_workload<MyDeque<int>>(20'000'000); }));
  print_time("mixed ends 10M",
             median_ms([&] { return mixed_workload<deque<int>>(ops); }),
             median_ms([&] { return mixed_workload<MyDeque<int>>(ops); }));
  print_time("random access 10M",
             median_ms([&] { return random_access_workload<deque<int>>(indices); }),
             median_ms([&] { return random_access_workload<MyDeque<int>>(indices); }));
  print_time("01 BFS n=300K m=2M",
             median_ms([&] { return bfs01_workload<deque<int>>(bfs_graph, false); }),
             median_ms([&] { return bfs01_workload<MyDeque<int>>(bfs_graph, false); }));
  print_time("01 BFS, reserve n",
             median_ms([&] { return bfs01_workload<deque<int>>(bfs_graph, true); }),
             median_ms([&] { return bfs01_workload<MyDeque<int>>(bfs_graph, true); }));
  cout << left << setw(28) << "01 BFS, two vectors" << right << fixed << setprecision(2)
       << " time " << setw(9) << median_ms([&] { return bfs01_two_vectors_workload(bfs_graph); })
       << " ms\n";
  cout << left << setw(28) << "Dial, max_cost=1" << right << fixed << setprecision(2)
       << " time " << setw(9) << median_ms([&] { return dial01_workload(bfs_graph); })
       << " ms\n";

  cout << "\nrequested heap bytes (live/peak)\n";
  print_memory("one int deque 1M",
               one_deque_memory<deque<int>, int>(1'000'000, false),
               one_deque_memory<MyDeque<int>, int>(1'000'000, false));
  print_memory("one int deque 1M reserve",
               one_deque_memory<deque<int>, int>(1'000'000, true),
               one_deque_memory<MyDeque<int>, int>(1'000'000, true));
  using Large = array<ll, 8>;
  print_memory("one 64B deque 200K",
               one_deque_memory<deque<Large>, Large>(200'000, false),
               one_deque_memory<MyDeque<Large>, Large>(200'000, false));
  print_memory("one 64B deque reserve",
               one_deque_memory<deque<Large>, Large>(200'000, true),
               one_deque_memory<MyDeque<Large>, Large>(200'000, true));
  print_memory("100K empty int deques",
               many_deques_memory<deque<int>>(100'000, false),
               many_deques_memory<MyDeque<int>>(100'000, false));
  print_memory("100K one-int deques",
               many_deques_memory<deque<int>>(100'000, true),
               many_deques_memory<MyDeque<int>>(100'000, true));

  cout << "\nsizeof: std::deque<int>=" << sizeof(deque<int>)
       << ", MyDeque<int>=" << sizeof(MyDeque<int>) << '\n';
}
