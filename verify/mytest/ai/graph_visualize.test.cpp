#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/visualize.hpp"
#include "graph/tree/merge_tree.hpp"

// Parse URL parameters independently, as the site's URL loader does.
map<string, string> parameters(const string &url)
{
  auto decode = [](const string &s)
  {
    string out;
    for (size_t i = 0; i < s.size(); ++i)
    {
      if (s[i] == '%')
      {
        out += char(stoi(s.substr(i + 1, 2), nullptr, 16));
        i += 2;
      }
      else out += s[i] == '+' ? ' ' : s[i];
    }
    return out;
  };
  map<string, string> res;
  auto pos = url.find('?') + 1;
  while (pos < url.size())
  {
    auto end = url.find('&', pos);
    if (end == string::npos) end = url.size();
    auto eq = url.find('=', pos);
    assert(eq < end);
    res[decode(url.substr(pos, eq - pos))] = decode(url.substr(eq + 1, end - eq - 1));
    pos = end + 1;
  }
  return res;
}

void check_url(const string &url, const string &text, bool directed, bool weighted, bool one_indexed)
{
  assert(url.find("https://hello-world-494ec.firebaseapp.com/?") == 0);
  auto params = parameters(url);
  assert(params.size() == 5);
  assert(params["format"] == "normal");
  assert(params["data"] == text);
  assert(params["directed"] == (directed ? "true" : "false"));
  assert(params["weighted"] == (weighted ? "true" : "false"));
  assert(params["indexed"] == (one_indexed ? "true" : "false"));
  assert(url.find_first_of(" \n\r") == string::npos);
}

void test_graphs()
{
  GraphUndirected<> g(5, vc<pair<int, int>>{{2, 0}, {0, 2}, {2, 2}, {1, 3}});
  assert(graph_text(g) == "5 4\n0 2\n0 2\n1 3\n2 2\n");
  assert(graph_text(g, true) == "5 4\n1 3\n1 3\n2 4\n3 3\n");
  check_url(graph_url(g), graph_text(g), false, false, false);
  check_url(graph_url(g, true), graph_text(g, true), false, false, true);

  GraphDirected<ll> weighted(4, vc<tuple<int, int, ll>>{{2, 0, -7}, {0, 2, 9}, {2, 2, LLONG_MAX}});
  assert(graph_text(weighted) == "4 3\n0 2 9\n2 0 -7\n2 2 9223372036854775807\n");
  check_url(graph_url(weighted), graph_text(weighted), true, true, false);

  GraphUndirected<int, true> erasable(4, vc<tuple<int, int, int>>{{0, 2, 3}, {2, 2, 7}, {2, 0, 5}, {1, 3, 11}});
  erasable.erase_edge(0);
  erasable.erase_edge(1);
  assert(graph_text(erasable) == "4 2\n0 2 5\n1 3 11\n");
  check_url(graph_url(erasable), graph_text(erasable), false, true, false);
  erasable.erase_edge(2);
  erasable.erase_edge(3);
  assert(graph_text(erasable) == "4 0\n");

  GraphDirected<void, true> directed(2, vc<pair<int, int>>{{1, 0}, {0, 1}});
  directed.erase_edge(1);
  assert(graph_text(directed) == "2 1\n1 0\n");
  check_url(graph_url(directed), graph_text(directed), true, false, false);

  GraphUndirected<> empty(0, vc<pair<int, int>>{});
  assert(graph_text(empty) == "0 0\n");
  check_url(graph_url(empty), "0 0\n", false, false, false);

  GraphDirected<bool> boolean(2, vc<tuple<int, int, bool>>{{0, 1, true}, {1, 0, false}});
  assert(graph_text(boolean) == "2 2\n0 1 1\n1 0 0\n");
  check_url(graph_url(boolean), graph_text(boolean), true, true, false);

  GraphDirected<unsigned char> byte(2, vc<tuple<int, int, unsigned char>>{{0, 1, 255}});
  GraphDirected<signed char> signed_byte(2, vc<tuple<int, int, signed char>>{{0, 1, -128}});
  assert(graph_text(byte) == "2 1\n0 1 255\n");
  assert(graph_text(signed_byte) == "2 1\n0 1 -128\n");
  check_url(graph_url(byte), graph_text(byte), true, true, false);

  double weight = 1.2345678901234567;
  GraphDirected<double> real(2, vc<tuple<int, int, double>>{{0, 1, weight}, {1, 0, 1e30}});
  istringstream input(graph_text(real));
  int n, m, u, v;
  double w;
  input >> n >> m >> u >> v >> w;
  assert(n == 2 && m == 2 && u == 0 && v == 1 && w == weight);
  input >> u >> v >> w;
  assert(u == 1 && v == 0 && w == 1e30);
  assert(graph_url(real).find("%2B") != string::npos);
  check_url(graph_url(real), graph_text(real), true, true, false);

  GraphDirected<i128> wide(1, vc<tuple<int, int, i128>>{{0, 0, i128(1) << 100}});
  assert(graph_text(wide) == "1 1\n0 0 1267650600228229401496703205376\n");
}

void test_trees()
{
  vc<int> par = {2, 0, -1, 2, 3};
  RootedTree tree(5, par);
  string expected = "5 4\n2 0\n0 1\n2 3\n3 4\n";
  assert(graph_text(tree) == expected && graph_text(par) == expected);
  check_url(graph_url(tree), expected, true, false, false);
  assert(graph_url(par, true) == graph_url(tree, true));
  assert(graph_text(RootedTree(1, vc<int>{0})) == "1 0\n");
  assert(graph_text(vc<int>{}) == "0 0\n");
  assert(graph_text(vc<int>{-1, 0, 2, 2, -3}) == "5 2\n0 1\n2 3\n");
  assert(graph_text(vl{2, LLONG_MIN, 2}) == "3 1\n2 0\n");
  assert(graph_text(vc<unsigned>{2, 1, 2}) == "3 1\n2 0\n");

  // Small element types can describe a forest with more vertices than their maximum.
  vc<unsigned char> narrow(300, 0);
  auto text = graph_text(narrow, true);
  assert(text.find("300 299\n") == 0 && text.find("1 300\n") != string::npos);

  MergeTree merges(4);
  merges.merge(0, 1);
  merges.merge(2, 3);
  assert(graph_text(merges.parents()) == "6 4\n4 0\n4 1\n5 2\n5 3\n");
  check_url(graph_url(merges.parents()), graph_text(merges.parents()), true, false, false);
}

void test_dump()
{
  GraphUndirected<> g(1, vc<pair<int, int>>{});
  int calls = 0;
  auto get_graph = [&]() -> const auto & { ++calls; return g; };
  ostringstream output;
  auto *saved = cerr.rdbuf(output.rdbuf());
  dump_graph(get_graph(), true);
  cerr.rdbuf(saved);
#ifdef LOCAL
  assert(calls == 1);
  assert(output.str() == "[graph] get_graph(), true\n" + graph_url(g, true) + "\n");
#else
  (void)get_graph;
  assert(calls == 0 && output.str().empty());
  dump_graph(expression_that_is_only_available_locally);
#endif
}

int main()
{
  test_graphs();
  test_trees();
  test_dump();
  cout << "Hello World\n";
}
