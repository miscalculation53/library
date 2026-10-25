#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_dump.hpp"

int main()
{
  int x = 0;
  auto increment = [&]() { x += 1, dump(x); };
  increment();
  assert(x == 1);
  int y = (dump(x), x + 1);
  assert(y == 2);

#if defined(LOCAL) && LIBRARY_HAS_FLOAT128
  cp::options::es_style = cp::types::es_style_t::no_es;
  const f128 value = f128(1) / 3;
  const string text = cp::export_var(value);
  assert(text.size() >= 35 && text.find("0.33333333333333333333") == 0);
  assert(cp::export_var(vector<f128>{value}).find(text) != string::npos);
#endif

#ifndef LOCAL
  dump(++x);
  x += 1, dump(++x);
  assert(x == 2);
  dump(local_only_variable | cp::hex());
#endif

  cout << "Hello World" << endl;
}
