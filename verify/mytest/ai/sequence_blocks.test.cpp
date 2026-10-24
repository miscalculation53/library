#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/ITP1_1_A"

/*! SPDX-License-Identifier: CC0-1.0 */

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
using namespace std;
using ll = long long;
using ull = unsigned long long;
#include "ds/bbst/lazy_sequence.hpp"

namespace block_vector_fixture {
long long checks = 0;
void require(bool ok, int line, const char* expression) {
    ++checks;
    if (!ok) throw runtime_error("line " + to_string(line) + ": " + expression);
}
#define REQUIRE(x) ::block_vector_fixture::require(bool(x), __LINE__, #x)

string token(int x) { return string(1 + x % 3, char('a' + x % 26)); }
struct Word {
    string text;
    Word() = delete;
    explicit Word(string x) : text(std::move(x)) {}
    explicit Word(int x) : text(token(x)) {}
};
struct Affine {
    int a, b;
    Affine() = delete;
    Affine(int a, int b) : a(a), b(b) {}
};
struct WordMonoid {
    using S = Word;
    static S e() { return S(string()); }
    static S op(const S& x, const S& y) { return S(x.text + y.text); }
};
struct WordAction : WordMonoid {
    using F = Affine;
    static F id() { return F(1, 0); }
    static F composition(const F& f, const F& g) {
        return F(f.a * g.a % 26, (f.a * g.b + f.b) % 26);
    }
    static S mapping(const F& f, const S& x) {
        string out = x.text;
        for (char& c : out) c = char('a' + (f.a * (c - 'a') + f.b) % 26);
        return S(std::move(out));
    }
};
using Tree = LazySequenceTree<WordAction>;
using Plain = SequenceTree<WordMonoid>;
template<class S> constexpr int expected_capacity() {
    int b = 16;
    while (b > 1 && b * sizeof(S) > 512) b /= 2;
    return b;
}
static_assert(expected_capacity<Word>() == 16);
static_assert(is_same_v<Tree, bbst_detail::SequenceTreeImpl<WordAction, true>>);
static_assert(is_same_v<Tree::Node, bbst_detail::TreapNode<WordAction, true, true>>);
static_assert(is_same_v<Plain::Node, bbst_detail::TreapNode<WordMonoid, true, false>>);
static_assert(is_same_v<Tree::S, Tree::Node::S> && is_same_v<Tree::F, Tree::Node::F>);
static_assert(is_same_v<Plain::S, Plain::Node::S> && is_same_v<Plain::F, Plain::Node::F>);
static_assert(is_same_v<Tree::F, Affine>);
static_assert(!is_copy_constructible_v<Tree> && !is_copy_assignable_v<Tree>);
static_assert(is_nothrow_move_constructible_v<Tree> && is_nothrow_move_assignable_v<Tree>);
static_assert(is_nothrow_move_constructible_v<Plain> && is_nothrow_move_assignable_v<Plain>);
static_assert(is_same_v<decltype(declval<const Tree&>().content()), vector<Word>>);
static_assert(is_same_v<decltype(declval<Tree&>().split(0)), Tree>);
static_assert(is_same_v<decltype(declval<Tree&>().extract(0, 0)), Tree>);
static_assert(is_constructible_v<Tree, const vector<int>&>);
static_assert(!is_convertible_v<const vector<int>&, Tree>);
template<class T, bool Enabled, class = void> struct has_apply : false_type {};
template<class T, bool Enabled> struct has_apply<T, Enabled, void_t<decltype(
    declval<T&>().template apply<Enabled>(0, 0, declval<const typename T::F&>()))>>
    : true_type {};
static_assert(has_apply<Tree, true>::value && !has_apply<Tree, false>::value);
static_assert(!has_apply<Plain, true>::value && !has_apply<Plain, false>::value);

vector<string> initial(int n, int offset = 0) {
    vector<string> out;
    for (int i = 0; i < n; ++i) out.push_back(token(i + offset));
    return out;
}
string fold(const vector<string>& a, int l, int r) {
    string out;
    for (int i = l; i < r; ++i) out += a[i];
    return out;
}
void model_apply(vector<string>& a, int l, int r, const Affine& f) {
    for (int i = l; i < r; ++i)
        for (char& c : a[i]) c = char('a' + (f.a * (c - 'a') + f.b) % 26);
}
void apply(Tree& t, vector<string>& a, int l, int r, const Affine& f) {
    t.apply(l, r, f); model_apply(a, l, r, f);
}
void reverse(Tree& t, vector<string>& a, int l, int r) {
    t.reverse(l, r); std::reverse(a.begin() + l, a.begin() + r);
}
void move(Tree& t, vector<string>& a, int l, int r, int k) {
    t.move(l, r, k);
    vector<string> cut(a.begin() + l, a.begin() + r);
    a.erase(a.begin() + l, a.begin() + r);
    a.insert(a.begin() + k, cut.begin(), cut.end());
}
void check(const Tree& t, const vector<string>& a, bool all_ranges = false) {
    const int n = int(a.size());
    REQUIRE(t.size() == n && t.empty() == a.empty());
    REQUIRE(t.all_prod().text == fold(a, 0, n));
    auto content = t.content();
    REQUIRE(content.size() == a.size());
    for (int i = 0; i < n; ++i) {
        REQUIRE(content[i].text == a[i]);
        REQUIRE(t.get(i).text == a[i]);
    }
    for (int p = 0; p <= n; ++p) REQUIRE(t.prod(p, p).text.empty());
    if (all_ranges) {
        for (int l = 0; l <= n; ++l)
            for (int r = l; r <= n; ++r) REQUIRE(t.prod(l, r).text == fold(a, l, r));
    } else {
        const int positions[] = {0, 1, 3, 4, 5, 7, 8, 15, 16, 17, 31, 32, 33, n};
        for (int l : positions) for (int r : positions)
            if (0 <= l && l <= r && r <= n) REQUIRE(t.prod(l, r).text == fold(a, l, r));
    }
}

void constructors_and_moves() {
    istringstream stream("0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16");
    Tree input(istream_iterator<int>(stream), istream_iterator<int>{});
    check(input, initial(17), true);
    Tree list = {Word("ab"), Word("c"), Word("de")};
    check(list, {"ab", "c", "de"}, true);
    Tree moved(std::move(input));
    REQUIRE(input.empty());
    input.insert(0, Word("fresh"));
    input = std::move(moved);
    REQUIRE(moved.empty());
    check(input, initial(17), true);
    auto* self = &input;
    input = std::move(*self);
    check(input, initial(17));
    Plain plain(vector<int>{0, 1, 2, 3, 4});
    plain.reverse(0, 5);
    REQUIRE(plain.all_prod().text == fold(vector<string>{token(4), token(3), token(2), token(1), token(0)}, 0, 5));
}

// Each split offset forces a different boundary block. Pending noncommuting
// actions and reversal precede the cut; independently acted pieces are rejoined.
void exhaustive_split_boundaries() {
    const int sizes[] = {0, 1, 3, 4, 5, 7, 8, 9, 15, 16, 17, 19, 20, 31, 32, 33, 47, 48, 49, 63, 64, 65};
    for (int n : sizes) for (int p = 0; p <= n; ++p) {
        vector<string> a = initial(n);
        Tree t(a);
        apply(t, a, 0, n, Affine(5, 3));
        reverse(t, a, 0, n);
        const int l = min(n, 3), r = max(l, n - 3);
        apply(t, a, l, r, Affine(3, 7));
        Tree tail = t.split(p);
        vector<string> left(a.begin(), a.begin() + p), right(a.begin() + p, a.end());
        apply(t, left, 0, int(left.size()), Affine(7, 11));
        reverse(tail, right, 0, int(right.size()));
        apply(tail, right, 0, int(right.size()), Affine(9, 5));
        check(t, left); check(tail, right);
        t.concat(std::move(tail));
        left.insert(left.end(), right.begin(), right.end());
        REQUIRE(tail.empty());
        tail.insert(0, Word("new"));
        check(tail, {"new"});
        check(t, left);
    }
}

// With capacity 16 and interior minimum 4, retaining 0..5 boundary elements
// exercises refill/merge decisions around the exact density threshold.
void density_boundary_edits() {
    for (int keep = 0; keep <= 5; ++keep) for (int shift = 0; shift < 16; ++shift) {
        vector<string> a = initial(113, shift);
        Tree t(a);
        apply(t, a, 0, int(a.size()), Affine(5, 1));
        reverse(t, a, 7, 105);
        apply(t, a, 3, 110, Affine(3, 4));
        const int l = 16 + shift, r = min(int(a.size()), l + 32 - keep);
        t.erase(l, r); a.erase(a.begin() + l, a.begin() + r);
        for (int j = 0; j < 20; ++j) {
            const int p = min(int(a.size()), l + j % 5);
            string value = token(1000 + j);
            t.insert(p, Word(value)); a.insert(a.begin() + p, value);
            const int q = max(0, min(int(a.size()) - 1, l - 1 + j % 7));
            t.erase(q); a.erase(a.begin() + q);
        }
        auto cut = t.extract(3, min(20 + keep, t.size()));
        vector<string> middle(a.begin() + 3, a.begin() + min(20 + keep, int(a.size())));
        a.erase(a.begin() + 3, a.begin() + min(20 + keep, int(a.size())));
        reverse(cut, middle, 0, int(middle.size()));
        t.concat(std::move(cut)); a.insert(a.end(), middle.begin(), middle.end());
        check(t, a);
        for (int remaining = t.size(); remaining > 0; --remaining) {
            int p = remaining % 2 ? 0 : remaining - 1;
            t.erase(p); a.erase(a.begin() + p);
            if (remaining % 4 == 0 || remaining <= 5) check(t, a);
        }
        check(t, {});
    }
    Tree joined;
    vector<string> a;
    // Small former endpoints become interior blocks during concatenation.
    for (int piece = 0; piece < 96; ++piece) {
        auto b = initial(1 + piece % 5, piece * 7);
        Tree independently_allocated(b);
        apply(independently_allocated, b, 0, int(b.size()), Affine(5, piece % 26));
        joined.concat(std::move(independently_allocated));
        a.insert(a.end(), b.begin(), b.end());
        REQUIRE(independently_allocated.empty());
        if (piece % 7 == 0) check(joined, a);
    }
    check(joined, a);
}

struct Random {
    uint64_t state;
    explicit Random(uint64_t seed) : state(seed) {}
    int below(int n) { state ^= state << 7; state ^= state >> 9; return int(state % uint64_t(n)); }
};
void randomized_forest() {
    for (int seed = 0; seed < 4; ++seed) {
        Random random(902311 + seed);
        vector<Tree> trees(5);
        vector<vector<string>> models(5);
        for (int i = 0; i < 5; ++i) { models[i] = initial(17 + 15 * i, seed + i); trees[i] = Tree(models[i]); }
        for (int step = 0; step < 2400; ++step) {
            int i = random.below(5), j = (i + 1 + random.below(4)) % 5;
            auto& t = trees[i]; auto& a = models[i];
            int n = int(a.size()), l = random.below(n + 1), r = random.below(n + 1);
            if (l > r) std::swap(l, r);
            int kind = random.below(13);
            if (kind == 0 && n < 320) {
                string value = token(step + seed);
                t.insert(l, Word(value)); a.insert(a.begin() + l, value);
            } else if (kind == 1) { t.erase(l, r); a.erase(a.begin() + l, a.begin() + r); }
            else if (kind == 2) reverse(t, a, l, r);
            else if (kind == 3) apply(t, a, l, r, Affine(1 + 2 * random.below(13), random.below(26)));
            else if (kind == 4) {
                trees[j] = t.split(l); models[j].assign(a.begin() + l, a.end()); a.resize(l);
            } else if (kind == 5 && n + int(models[j].size()) <= 480) {
                t.concat(std::move(trees[j])); a.insert(a.end(), models[j].begin(), models[j].end()); models[j].clear();
                REQUIRE(trees[j].empty());
            } else if (kind == 6) {
                trees[j] = t.extract(l, r); models[j].assign(a.begin() + l, a.begin() + r); a.erase(a.begin() + l, a.begin() + r);
            } else if (kind == 7) move(t, a, l, r, random.below(n - (r - l) + 1));
            else if (kind == 8) {
                int m = l + random.below(r - l + 1);
                t.rotate(l, m, r); rotate(a.begin() + l, a.begin() + m, a.begin() + r);
            } else if (kind == 9 && n) {
                int p = random.below(n); string value = token(step + 31);
                t.set(p, Word(value)); a[p] = value;
            } else if (kind == 10 && n) {
                int p = random.below(n); Affine f(3, 1);
                t.apply(p, f); model_apply(a, p, p + 1, f);
            } else if (kind == 11) REQUIRE(t.prod(l, r).text == fold(a, l, r));
            else if (kind == 12 && step % 31 == 0) { t.clear(); a.clear(); }
            if (step % 37 == 0) for (int k = 0; k < 5; ++k) check(trees[k], models[k]);
        }
        for (int i = 0; i < 5; ++i) check(trees[i], models[i]);
    }
}

struct ReentrantPredicate {
    const Tree& tree;
    Tree& other;
    const vector<string>& model;
    string expected;
    size_t limit;
    bool from_left;
    unique_ptr<int> resource = make_unique<int>(7);
    ReentrantPredicate(const Tree& t, Tree& o, const vector<string>& a, string e, size_t k, bool direction)
        : tree(t), other(o), model(a), expected(std::move(e)), limit(k), from_left(direction) {}
    ReentrantPredicate(const ReentrantPredicate&) = delete;
    bool operator()(Word& value) const {
        string before = value.text;
        REQUIRE(before == (from_left ? expected.substr(0, before.size())
                                    : expected.substr(expected.size() - before.size())));
        REQUIRE(tree.all_prod().text == fold(model, 0, int(model.size())));
        REQUIRE(tree.get(0).text == model[0]);
        REQUIRE(tree.prod(3, tree.size() - 2).text == fold(model, 3, tree.size() - 2));
        auto values = tree.content();
        REQUIRE(values.size() == model.size());
        auto inner = [](Word& x) { return x.text.size() <= 11; };
        int right = 0, left = tree.size();
        while (right < tree.size() && fold(model, 0, right + 1).size() <= 11) ++right;
        while (left > 0 && fold(model, left - 1, tree.size()).size() <= 11) --left;
        REQUIRE(tree.max_right_ok(0, inner) == right);
        REQUIRE(tree.min_left_ok(tree.size(), inner) == left);
        other.insert(other.size(), Word("x"));
        if (other.size() > 4) other.erase(0);
        other.reverse(0, other.size());
        REQUIRE(value.text == before && *resource == 7);
        return value.text.size() <= limit;
    }
};
static_assert(!is_copy_constructible_v<ReentrantPredicate>);
void reentrant_search_with_pending_tags() {
    auto a = initial(81);
    Tree t(a), other;
    apply(t, a, 0, t.size(), Affine(5, 3));
    reverse(t, a, 0, t.size());
    apply(t, a, 4, t.size() - 3, Affine(3, 7));
    reverse(t, a, 13, 67);
    const int points[] = {0, 1, 3, 4, 15, 16, 17, 31, 32, 33, 63, 64, 80, 81};
    for (int l : points) for (int r : points) if (l <= r) {
        size_t limit = fold(a, l, r).size();
        ReentrantPredicate right(t, other, a, fold(a, l, t.size()), limit, true);
        ReentrantPredicate left(t, other, a, fold(a, 0, r), limit, false);
        REQUIRE(t.max_right_ok(l, right) == r);
        REQUIRE(t.min_left_ok(r, left) == l);
    }
    check(t, a);
}
} // namespace block_vector_fixture

int main() {
    try {
        using namespace block_vector_fixture;
        constructors_and_moves();
        exhaustive_split_boundaries();
        density_boundary_edits();
        randomized_forest();
        reentrant_search_with_pending_tags();
        cout << "Hello World" << '\n';
    } catch (const exception& error) {
        cerr << "block vector fixture FAIL " << error.what() << '\n';
        return 1;
    }
}
