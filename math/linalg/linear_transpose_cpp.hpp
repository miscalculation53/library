#pragma once

#include "linear_transpose.hpp"

/**
 * @brief C++ の順方向コードから線形写像の転置を構築
 * @docs docs/math/linalg/linear_transpose_cpp.md
 */

namespace linear_transpose
{

template <class T> class Recorder;
template <class T> class Program;

// 固定値、または coefficient * node。コピー・上書きは参照先を変えるだけ。
template <class T>
class Value
{
  Recorder<T> *owner = nullptr;
  int node = -1;
  T coefficient = 0;
  friend class Recorder<T>;

  Value(Recorder<T> *owner, int node) : owner(owner), node(node), coefficient(1) {}

public:
  Value() = default;
  Value(const T &value) : coefficient(value) {}
  template <class I, std::enable_if_t<std::is_integral_v<I>, int> = 0>
  Value(I value) : coefficient(T(value)) {}

  bool is_fixed() const { return owner == nullptr; }
  T fixed() const
  {
    if (owner) throw std::logic_error("transpose: an active value was used as a fixed value or condition");
    return coefficient;
  }
  explicit operator bool() const { return fixed() != T(0); }
  auto val() const { return fixed().val(); }
  Value inv() const { return fixed().inv(); }

  Value operator+() const { return *this; }
  Value operator-() const
  {
    auto res = *this;
    res.coefficient = -res.coefficient;
    return res;
  }
  friend Value operator+(const Value &a, const Value &b)
  {
    if (!a.owner && !b.owner) return a.coefficient + b.coefficient;
    return (a.owner ? a.owner : b.owner)->add(a, b);
  }
  friend Value operator-(const Value &a, const Value &b) { return a + (-b); }
  friend Value operator*(const Value &a, const Value &b)
  {
    if (a.owner && b.owner) throw std::logic_error("transpose: nonlinear multiplication");
    Value res = a.owner ? a : b;
    res.coefficient *= a.owner ? b.coefficient : a.coefficient;
    if (res.coefficient == T(0)) return Value();
    return res;
  }
  friend Value operator/(const Value &a, const Value &b) { return a * b.inv(); }
  Value &operator+=(const Value &b) { return *this = *this + b; }
  Value &operator-=(const Value &b) { return *this = *this - b; }
  Value &operator*=(const Value &b) { return *this = *this * b; }
  Value &operator/=(const Value &b) { return *this = *this / b; }
  friend bool operator==(const Value &a, const Value &b) { return a.fixed() == b.fixed(); }
  friend bool operator!=(const Value &a, const Value &b) { return !(a == b); }
  friend bool operator<(const Value &a, const Value &b) { return a.fixed() < b.fixed(); }
  friend bool operator>(const Value &a, const Value &b) { return b < a; }
  friend bool operator<=(const Value &a, const Value &b) { return !(b < a); }
  friend bool operator>=(const Value &a, const Value &b) { return !(a < b); }
};

template <class T>
class Program
{
  friend class Recorder<T>;
  enum class Kind { Add, Convolution, Modulo, Ntt, Intt };
  struct Edge { int node; T coefficient; };
  struct Step { Kind kind; int output, first, second; T a, b; };
  struct Block { vc<Edge> input; vc<T> parameter; int output_size; };
  int inputs = 0, nodes = 0;
  vc<Edge> outputs;
  vc<Step> steps;
  vc<Block> blocks;

  static const char *transpose_function(Kind kind)
  {
    if (kind == Kind::Convolution) return "convolution_transpose";
    if (kind == Kind::Modulo) return "polynomial_mod_transpose";
    if (kind == Kind::Ntt) return "ntt_transpose";
    return "intt_transpose";
  }

public:
  int input_size() const { return inputs; }
  int output_size() const { return outputs.size(); }
  int node_count() const { return nodes; }

  vc<T> transpose(const vc<T> &seed) const
  {
    assert((int)seed.size() == output_size());
    vc<T> gradient(nodes);
    repi(i, outputs.size()) if (outputs[i].node >= 0)
      gradient[outputs[i].node] += seed[i] * outputs[i].coefficient;
    for (auto it = steps.rbegin(); it != steps.rend(); ++it)
    {
      const auto &s = *it;
      if (s.kind == Kind::Add)
      {
        gradient[s.first] += gradient[s.output] * s.a;
        gradient[s.second] += gradient[s.output] * s.b;
        continue;
      }
      const auto &b = blocks[s.first];
      vc<T> w(gradient.begin() + s.output, gradient.begin() + s.output + b.output_size), t;
      if constexpr (!std::is_integral_v<T>)
      {
        if (s.kind == Kind::Convolution) t = convolution_transpose(w, b.parameter, b.input.size());
        if (s.kind == Kind::Modulo) t = polynomial_mod_transpose(w, b.parameter, b.input.size());
        if (s.kind == Kind::Ntt) t = ntt_transpose(w);
        if (s.kind == Kind::Intt) t = intt_transpose(w);
      }
      else throw std::logic_error("transpose: polynomial blocks require a modint coefficient type");
      repi(i, b.input.size()) if (b.input[i].node >= 0)
        gradient[b.input[i].node] += t[i] * b.input[i].coefficient;
    }
    gradient.resize(inputs);
    return gradient;
  }

  // 記録した形状・固定係数に特化した C++ を出力する。ループは展開される。
  void write_cpp(std::ostream &out, const std::string &name, const std::string &coefficient_type) const
  {
    out << "// Generated from a C++ forward execution. Fixed sizes and coefficients.\n"
        << "#include \"math/linalg/linear_transpose.hpp\"\n"
        << "inline std::vector<" << coefficient_type << "> " << name
        << "(const std::vector<" << coefficient_type << ">& seed) {\n"
        << "  using T = " << coefficient_type << ";\n"
        << "  assert(seed.size() == " << outputs.size() << ");\n"
        << "  std::vector<T> g(" << nodes << ");\n";
    repi(i, outputs.size()) if (outputs[i].node >= 0)
      out << "  g[" << outputs[i].node << "] += seed[" << i << "] * T(" << outputs[i].coefficient << ");\n";
    for (auto it = steps.rbegin(); it != steps.rend(); ++it)
    {
      const auto &s = *it;
      if (s.kind == Kind::Add)
      {
        out << "  g[" << s.first << "] += g[" << s.output << "] * T(" << s.a << ");\n"
            << "  g[" << s.second << "] += g[" << s.output << "] * T(" << s.b << ");\n";
        continue;
      }
      const auto &b = blocks[s.first];
      out << "  {\n    std::vector<T> w(g.begin() + " << s.output << ", g.begin() + " << s.output + b.output_size << ");\n"
          << "    auto t = linear_transpose::" << transpose_function(s.kind) << "(w";
      if (s.kind == Kind::Convolution || s.kind == Kind::Modulo)
      {
        out << ", std::vector<T>{";
        repi(i, b.parameter.size()) out << (i ? ", " : "") << "T(" << b.parameter[i] << ")";
        out << "}, " << b.input.size();
      }
      out << ");\n";
      repi(i, b.input.size()) if (b.input[i].node >= 0)
        out << "    g[" << b.input[i].node << "] += t[" << i << "] * T(" << b.input[i].coefficient << ");\n";
      out << "  }\n";
    }
    out << "  g.resize(" << inputs << ");\n  return g;\n}\n";
  }
};

template <class T>
class Recorder
{
  using V = Value<T>;
  using P = Program<T>;
  using Kind = typename P::Kind;
  P program;

  typename P::Edge edge(const V &x) const
  {
    if (x.owner && x.owner != this) throw std::logic_error("transpose: mixed recordings");
    if (!x.owner && x.coefficient != T(0)) throw std::logic_error("transpose: nonzero constant term");
    return {x.node, x.coefficient};
  }

  vc<V> block(Kind kind, const vc<V> &x, const vc<T> &parameter, int output_size)
  {
    typename P::Block b{{}, parameter, output_size};
    b.input.reserve(x.size());
    for (const auto &v : x) b.input.push_back(edge(v));
    const int first = program.nodes;
    program.steps.push_back({kind, first, (int)program.blocks.size(), 0, T(0), T(0)});
    program.blocks.push_back(std::move(b));
    vc<V> result;
    result.reserve(output_size);
    repi(i, output_size) result.push_back(V(this, program.nodes++));
    return result;
  }

public:
  Recorder() = default;
  Recorder(const Recorder &) = delete;
  Recorder &operator=(const Recorder &) = delete;

  vc<V> input(int n)
  {
    assert(n >= 0 && program.nodes == 0);
    program.inputs = n;
    vc<V> result;
    result.reserve(n);
    repi(i, n) result.push_back(V(this, program.nodes++));
    return result;
  }

  V add(const V &a, const V &b)
  {
    const auto ea = edge(a), eb = edge(b);
    if (ea.node < 0) return b;
    if (eb.node < 0) return a;
    if (ea.node == eb.node)
    {
      auto c = a;
      c.coefficient += b.coefficient;
      return c.coefficient == T(0) ? V() : c;
    }
    const int id = program.nodes++;
    program.steps.push_back({Kind::Add, id, ea.node, eb.node, ea.coefficient, eb.coefficient});
    return V(this, id);
  }

  static Recorder *owner_of(const vc<V> &x)
  {
    Recorder *result = nullptr;
    for (const auto &a : x) if (a.owner)
    {
      if (result && result != a.owner) throw std::logic_error("transpose: mixed recordings");
      result = a.owner;
    }
    return result;
  }

  vc<V> convolution_block(const vc<V> &x, const vc<T> &b)
  {
    if (x.empty() || b.empty()) return {};
    return block(Kind::Convolution, x, b, x.size() + b.size() - 1);
  }
  vc<V> modulo_block(const vc<V> &x, const vc<T> &g)
  {
    assert(g.size() >= 2 && g.back() == T(1));
    return block(Kind::Modulo, x, g, g.size() - 1);
  }
  vc<V> ntt_block(const vc<V> &x, bool inverse)
  {
    assert(x.empty() || ntt_ok<T>(x.size()));
    return block(inverse ? Kind::Intt : Kind::Ntt, x, {}, x.size());
  }

  P finish(const vc<V> &y)
  {
    for (const auto &v : y) program.outputs.push_back(edge(v));
    return std::move(program);
  }
};

template <class T>
vc<T> fixed_values(const vc<Value<T>> &a)
{
  vc<T> res;
  res.reserve(a.size());
  for (const auto &v : a) res.push_back(v.fixed());
  return res;
}

// ADL で既存テンプレートからも呼び出す。
template <class T>
vc<Value<T>> convolution(const vc<Value<T>> &a, const vc<T> &b)
{
  if (auto *recorder = Recorder<T>::owner_of(a)) return recorder->convolution_block(a, b);
  auto c = ::convolution(fixed_values(a), b);
  return vc<Value<T>>(c.begin(), c.end());
}
template <class T>
vc<Value<T>> convolution(const vc<T> &a, const vc<Value<T>> &b) { return convolution(b, a); }
template <class T>
vc<Value<T>> convolution(const vc<Value<T>> &a, const vc<Value<T>> &b)
{
  if (Recorder<T>::owner_of(a) && Recorder<T>::owner_of(b)) throw std::logic_error("transpose: nonlinear convolution");
  if (Recorder<T>::owner_of(b)) return convolution(b, fixed_values(a));
  return convolution(a, fixed_values(b));
}
template <class T>
vc<Value<T>> polynomial_mod(const vc<Value<T>> &a, const vc<T> &g)
{
  if (auto *recorder = Recorder<T>::owner_of(a)) return recorder->modulo_block(a, g);
  auto c = polynomial_mod(fixed_values(a), g);
  return vc<Value<T>>(c.begin(), c.end());
}
template <class T>
void ntt(vc<Value<T>> &a)
{
  if (auto *recorder = Recorder<T>::owner_of(a)) a = recorder->ntt_block(a, false);
  else
  {
    auto c = ntt_forward(fixed_values(a));
    a.assign(c.begin(), c.end());
  }
}
template <class T>
void intt(vc<Value<T>> &a)
{
  if (auto *recorder = Recorder<T>::owner_of(a)) a = recorder->ntt_block(a, true);
  else
  {
    auto c = intt_forward(fixed_values(a));
    a.assign(c.begin(), c.end());
  }
}

template <class T, class Forward>
Program<T> record(int n, const Forward &forward)
{
  Recorder<T> recorder;
  auto x = recorder.input(n);
  return recorder.finish(forward(std::move(x)));
}

template <class T, class Forward>
vc<T> transpose(int n, const vc<T> &seed, const Forward &forward)
{
  return record<T>(n, forward).transpose(seed);
}

} // namespace linear_transpose
