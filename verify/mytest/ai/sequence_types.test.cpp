#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/ITP1_1_A"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
using namespace std;
using ll = long long;
using ull = unsigned long long;
#include "ds/bbst/lazy_sequence.hpp"

namespace block_type_fixture {
long long checks = 0;
void require(bool ok, int line, const char* expression) {
    ++checks;
    if (!ok) throw runtime_error("line " + to_string(line) + ": " + expression);
}
#define REQUIRE(x) ::block_type_fixture::require(bool(x), __LINE__, #x)
template<class S> constexpr int expected_capacity() {
    int b = 16;
    while (b > 1 && b * sizeof(S) > 512) b /= 2;
    return b;
}

template<size_t Padding> struct SizedValue {
    array<unsigned char, Padding> payload{};
    int value, length;
    SizedValue() = delete;
    explicit SizedValue(int x) : value(x), length(1) {}
    SizedValue(int x, int n) : value(x), length(n) {}
};
template<size_t Padding> struct SizedAction {
    using S = SizedValue<Padding>;
    using F = pair<int, int>;
    static S e() { return S(0, 0); }
    static S op(const S& x, const S& y) { return S(x.value + y.value, x.length + y.length); }
    static F id() { return {1, 0}; }
    static F composition(const F& f, const F& g) { return {f.first * g.first, f.first * g.second + f.second}; }
    static S mapping(const F& f, const S& x) { return S(f.first * x.value + f.second * x.length, x.length); }
};
static_assert(expected_capacity<SizedValue<40>>() == 8);
static_assert(expected_capacity<SizedValue<80>>() == 4);
static_assert(expected_capacity<SizedValue<176>>() == 2);
template<size_t Padding> void intermediate_capacity_boundaries() {
    using Action = SizedAction<Padding>;
    using Tree = LazySequenceTree<Action>;
    using S = typename Action::S;
    const int b = expected_capacity<S>(), n = 5 * b + 3;
    for (int p = 0; p <= n; ++p) {
        vector<int> a;
        for (int i = 0; i < n; ++i) a.push_back(1 + i % 7);
        Tree t(a);
        t.apply(0, n, {2, 3}); for (int& x : a) x = 2 * x + 3;
        t.reverse(0, n); reverse(a.begin(), a.end());
        t.apply(1, n - 1, {3, 1}); for (int i = 1; i < n - 1; ++i) a[i] = 3 * a[i] + 1;
        auto tail = t.split(p);
        t.insert(p, S(91)); tail.insert(0, S(92));
        t.concat(std::move(tail));
        a.insert(a.begin() + p, 91); a.insert(a.begin() + p + 1, 92);
        int l = max(0, p - 1), r = min(int(a.size()), p + b + 1);
        t.erase(l, r); a.erase(a.begin() + l, a.begin() + r);
        REQUIRE(t.size() == int(a.size()) && tail.empty());
        int sum = 0;
        auto content = t.content();
        REQUIRE(content.size() == a.size());
        for (int i = 0; i < t.size(); ++i) {
            REQUIRE(t.get(i).value == a[i] && content[i].value == a[i]);
            REQUIRE(content[i].length == 1);
            sum += a[i];
        }
        REQUIRE(t.all_prod().value == sum && t.all_prod().length == t.size());
        for (int i = 0; i <= t.size(); ++i) REQUIRE(t.prod(i, i).length == 0);
    }
}

// B = 1. Positive affine functions preserve max and send the empty identity
// to b. Applying a tag twice changes an ordinary value and is observable.
struct alignas(64) BigValue {
    static inline int live = 0;
    array<unsigned char, 640> payload{};
    long long value;
    BigValue() = delete;
    explicit BigValue(long long x) : value(x) { ++live; }
    BigValue(const BigValue& x) : value(x.value) { ++live; }
    BigValue(BigValue&& x) noexcept : value(x.value) { ++live; }
    BigValue& operator=(const BigValue& x) { value = x.value; return *this; }
    BigValue& operator=(BigValue&& x) noexcept { value = x.value; return *this; }
    ~BigValue() { --live; }
};
struct BigAffine {
    long long a, b;
    BigAffine() = delete;
    BigAffine(long long a, long long b) : a(a), b(b) {}
};
struct BigMaxAction {
    using S = BigValue;
    using F = BigAffine;
    static S e() { return S(0); }
    static S op(const S& x, const S& y) { return S(max(x.value, y.value)); }
    static F id() { return F(1, 0); }
    static F composition(const F& f, const F& g) { return F(f.a * g.a, f.a * g.b + f.b); }
    static S mapping(const F& f, const S& x) { return S(f.a * x.value + f.b); }
};
using BigTree = LazySequenceTree<BigMaxAction>;
static_assert(expected_capacity<BigValue>() == 1);
static_assert(alignof(BigValue) == 64);
static_assert(is_same_v<BigTree::Node, bbst_detail::TreapNode<BigMaxAction, true, true>>);
static_assert(is_same_v<BigTree::F, BigTree::Node::F>);
static_assert(is_nothrow_move_constructible_v<BigTree> && is_nothrow_move_assignable_v<BigTree>);
long long maximum(const vector<long long>& a, int l, int r) {
    long long out = 0;
    for (int i = l; i < r; ++i) out = max(out, a[i]);
    return out;
}
void big_apply(BigTree& t, vector<long long>& a, int l, int r, const BigAffine& f) {
    t.apply(l, r, f);
    for (int i = l; i < r; ++i) a[i] = f.a * a[i] + f.b;
}
void check_big(const BigTree& t, const vector<long long>& a) {
    REQUIRE(t.size() == int(a.size()));
    REQUIRE(t.all_prod().value == maximum(a, 0, int(a.size())));
    auto content = t.content();
    REQUIRE(content.size() == a.size());
    for (int i = 0; i < t.size(); ++i) {
        REQUIRE(t.get(i).value == a[i] && content[i].value == a[i]);
        REQUIRE(reinterpret_cast<uintptr_t>(&content[i]) % alignof(BigValue) == 0);
    }
    for (int l = 0; l <= t.size(); ++l) {
        REQUIRE(t.prod(l, l).value == 0);
        for (int r = l; r <= t.size(); ++r)
            REQUIRE(t.prod(l, r).value == maximum(a, l, r));
    }
}
void one_element_blocks_nonunital_nonidempotent() {
    REQUIRE(BigValue::live == 0);
    {
        BigTree empty;
        empty.apply(0, 0, BigAffine(2, 3));
        REQUIRE(empty.prod(0, 0).value == 0 && empty.all_prod().value == 0);
        REQUIRE(BigMaxAction::mapping(BigAffine(2, 3), BigMaxAction::e()).value == 3);
        // The single leaf exposes accidental value/block/subtree double mapping.
        BigTree single(vector<int>{2});
        single.apply(0, 1, BigAffine(2, 3));
        REQUIRE(single.get(0).value == 7 && single.all_prod().value == 7);
        single.apply(0, BigAffine(3, 1));
        REQUIRE(single.get(0).value == 22 && single.prod(0, 1).value == 22);
        single.reverse(0, 1);
        auto only = single.split(0);
        single.clear();
        REQUIRE(only.get(0).value == 22);
        only.insert(0, BigValue(4));
        REQUIRE(only.get(0).value == 4 && only.get(1).value == 22);
        only.clear();

        vector<long long> a;
        BigTree t;
        for (int i = 0; i < 35; ++i) { t.insert(i, BigValue(i + 1)); a.push_back(i + 1); }
        big_apply(t, a, 0, 35, BigAffine(2, 3));
        t.reverse(0, 35); reverse(a.begin(), a.end());
        big_apply(t, a, 3, 32, BigAffine(3, 5));
        big_apply(t, a, 7, 24, BigAffine(2, 1));
        t.insert(16, BigValue(4)); a.insert(a.begin() + 16, 4);
        t.set(3, BigValue(9)); a[3] = 9;
        auto tail = t.split(17);
        vector<long long> b(a.begin() + 17, a.end()); a.resize(17);
        big_apply(tail, b, 0, int(b.size()), BigAffine(2, 7));
        t.reverse(2, 16); reverse(a.begin() + 2, a.begin() + 16);
        check_big(t, a); check_big(tail, b);
        t.concat(std::move(tail)); a.insert(a.end(), b.begin(), b.end());
        REQUIRE(tail.empty() && tail.all_prod().value == 0);
        auto middle = t.extract(4, 29);
        vector<long long> cut(a.begin() + 4, a.begin() + 29);
        a.erase(a.begin() + 4, a.begin() + 29);
        t.clear(); a.clear();
        check_big(middle, cut);
        middle.erase(1, middle.size() - 1);
        cut.erase(cut.begin() + 1, cut.end() - 1);
        big_apply(middle, cut, 0, middle.size(), BigAffine(2, 3));
        check_big(middle, cut);
        middle.clear();
    }
    REQUIRE(BigValue::live == 0);
}

// Verified existing subset: copy construction/assignment with explicitly
// deleted move operations. Its original API subset excludes vector<S>/content.
struct CopyAdlValue {
    static inline int live = 0;
    int value;
    explicit CopyAdlValue(int x) : value(x) { ++live; }
    CopyAdlValue(const CopyAdlValue& x) : value(x.value) { ++live; }
    CopyAdlValue& operator=(const CopyAdlValue&) = default;
    CopyAdlValue(CopyAdlValue&&) = delete;
    CopyAdlValue& operator=(CopyAdlValue&&) = delete;
    ~CopyAdlValue() { --live; }
    friend void swap(CopyAdlValue& x, CopyAdlValue& y) noexcept {
        int before = x.value; x.value = y.value; y.value = before;
    }
};
struct CopyAdlMonoid {
    using S = CopyAdlValue;
    static S e() { return S(0); }
    static S op(const S& x, const S& y) { return S(x.value + y.value); }
};
static_assert(is_copy_constructible_v<CopyAdlValue> && is_copy_assignable_v<CopyAdlValue>);
static_assert(!is_move_constructible_v<CopyAdlValue> && !is_move_assignable_v<CopyAdlValue>);
static_assert(is_swappable_v<CopyAdlValue>);
void check_copy_adl(const SequenceTree<CopyAdlMonoid>& t, const vector<int>& a) {
    REQUIRE(t.size() == int(a.size()));
    int sum = 0;
    for (int i = 0; i < t.size(); ++i) { REQUIRE(t.get(i).value == a[i]); sum += a[i]; }
    REQUIRE(t.all_prod().value == sum && t.prod(0, t.size()).value == sum);
}
void copy_adl_boundary_edits() {
    REQUIRE(CopyAdlValue::live == 0);
    {
        vector<int> a;
        for (int i = 0; i < 65; ++i) a.push_back(i + 1);
        SequenceTree<CopyAdlMonoid> t(a);
        for (int p : {0, 3, 4, 15, 16, 17, 31, 32, 33, 64, 65}) {
            t.insert(p, CopyAdlValue(100 + p)); a.insert(a.begin() + p, 100 + p);
            int q = min(p + 1, int(a.size()) - 1);
            t.set(q, CopyAdlValue(7)); a[q] = 7;
            t.erase(q); a.erase(a.begin() + q);
            check_copy_adl(t, a);
        }
        t.reverse(3, 62); reverse(a.begin() + 3, a.begin() + 62);
        auto tail = t.split(17);
        vector<int> b(a.begin() + 17, a.end()); a.resize(17);
        check_copy_adl(t, a); check_copy_adl(tail, b);
        t.concat(std::move(tail)); a.insert(a.end(), b.begin(), b.end());
        auto part = t.extract(3, 61);
        b.assign(a.begin() + 3, a.begin() + 61); a.erase(a.begin() + 3, a.begin() + 61);
        check_copy_adl(t, a); check_copy_adl(part, b);
        t.clear(); part.clear();
    }
    REQUIRE(CopyAdlValue::live == 0);
}

// Verified existing subset: S has copy/move construction and move assignment;
// copy assignment is deleted. The original subset excludes set.
struct MoveAssignedValue {
    static inline int live = 0;
    int value;
    explicit MoveAssignedValue(int x) : value(x) { ++live; }
    MoveAssignedValue(const MoveAssignedValue& x) : value(x.value) { ++live; }
    MoveAssignedValue(MoveAssignedValue&& x) noexcept : value(x.value) { ++live; }
    MoveAssignedValue& operator=(const MoveAssignedValue&) = delete;
    MoveAssignedValue& operator=(MoveAssignedValue&&) = default;
    ~MoveAssignedValue() { --live; }
};
struct MoveAssignedMonoid {
    using S = MoveAssignedValue;
    static S e() { return S(0); }
    static S op(const S& x, const S& y) { return S(x.value + y.value); }
};
struct MoveAssignedAction : MoveAssignedMonoid {
    using F = int;
    static F id() { return 1; }
    static F composition(F f, F g) { return f * g; }
    static S mapping(F f, const S& x) { return S(f * x.value); }
};
static_assert(is_copy_constructible_v<MoveAssignedValue> && is_move_constructible_v<MoveAssignedValue>);
static_assert(!is_copy_assignable_v<MoveAssignedValue> && is_move_assignable_v<MoveAssignedValue>);
template<class Tree> void check_move_assigned(const Tree& t, const vector<int>& a) {
    REQUIRE(t.size() == int(a.size()));
    auto content = t.content();
    int sum = 0;
    for (int i = 0; i < t.size(); ++i) {
        REQUIRE(t.get(i).value == a[i] && content[i].value == a[i]); sum += a[i];
    }
    REQUIRE(t.all_prod().value == sum && t.prod(0, t.size()).value == sum);
}
template<class Tree, bool Lazy> void move_assigned_operations() {
    vector<int> a;
    for (int i = 0; i < 81; ++i) a.push_back(1 + i % 7);
    Tree t(a);
    if constexpr (Lazy) {
        t.apply(0, 81, 2); for (int& x : a) x *= 2;
        t.apply(3, 79, 3); for (int i = 3; i < 79; ++i) a[i] *= 3;
    }
    t.reverse(1, 80); reverse(a.begin() + 1, a.begin() + 80);
    for (int p : {0, 3, 4, 15, 16, 17, 31, 32, 33, 80}) {
        t.insert(p, MoveAssignedValue(5)); a.insert(a.begin() + p, 5);
        t.erase(p + 1); a.erase(a.begin() + p + 1);
    }
    check_move_assigned(t, a);
    auto tail = t.split(19);
    vector<int> b(a.begin() + 19, a.end()); a.resize(19);
    check_move_assigned(t, a); check_move_assigned(tail, b);
    t.concat(std::move(tail)); a.insert(a.end(), b.begin(), b.end());
    auto part = t.extract(3, 76);
    b.assign(a.begin() + 3, a.begin() + 76); a.erase(a.begin() + 3, a.begin() + 76);
    t.clear();
    check_move_assigned(part, b);
    part.reverse(0, part.size()); reverse(b.begin(), b.end());
    if constexpr (Lazy) { part.apply(0, part.size(), 2); for (int& x : b) x *= 2; }
    check_move_assigned(part, b);
    part.clear();
}
void move_assignment_value_subsets() {
    REQUIRE(MoveAssignedValue::live == 0);
    move_assigned_operations<SequenceTree<MoveAssignedMonoid>, false>();
    move_assigned_operations<LazySequenceTree<MoveAssignedAction>, true>();
    REQUIRE(MoveAssignedValue::live == 0);
}

struct Resource {
    static inline int live = 0;
    int id;
    explicit Resource(int id) : id(id) { ++live; }
    ~Resource() { --live; }
};
struct SharedValue {
    vector<shared_ptr<Resource>> objects;
    SharedValue() = delete;
    explicit SharedValue(vector<shared_ptr<Resource>> x) : objects(std::move(x)) {}
};
struct SharedMonoid {
    using S = SharedValue;
    static S e() { return S(vector<shared_ptr<Resource>>{}); }
    static S op(const S& x, const S& y) {
        auto result = x.objects;
        result.insert(result.end(), y.objects.begin(), y.objects.end());
        return S(std::move(result));
    }
};
using SharedTree = SequenceTree<SharedMonoid>;
weak_ptr<Resource> append(SharedTree& t, int id) {
    auto object = make_shared<Resource>(id);
    weak_ptr<Resource> weak = object;
    t.insert(t.size(), SharedValue({object}));
    return weak;
}
void check_ids(const SharedTree& t, const vector<int>& ids) {
    REQUIRE(t.size() == int(ids.size()));
    auto content = t.content();
    for (int i = 0; i < t.size(); ++i) {
        REQUIRE(content[i].objects.size() == 1 && content[i].objects[0]->id == ids[i]);
    }
    auto aggregate = t.all_prod();
    REQUIRE(aggregate.objects.size() == ids.size());
    for (int i = 0; i < t.size(); ++i) REQUIRE(aggregate.objects[i]->id == ids[i]);
}
void independent_tree_value_lifetimes() {
    REQUIRE(Resource::live == 0);
    vector<weak_ptr<Resource>> weak(72);
    SharedTree receiver;
    vector<int> ids;
    for (int id = 0; id < 33; ++id) { weak[id] = append(receiver, id); ids.push_back(id); }
    {
        SharedTree independent;
        for (int id = 33; id < 70; ++id) weak[id] = append(independent, id);
        independent.reverse(0, 37);
        receiver.concat(std::move(independent));
        for (int id = 69; id >= 33; --id) ids.push_back(id);
        REQUIRE(independent.empty());
        weak[70] = append(independent, 70);
        independent.clear();
        REQUIRE(weak[70].expired());
    }
    check_ids(receiver, ids);
    auto tail = receiver.split(3);
    vector<int> rest(ids.begin() + 3, ids.end()); ids.resize(3);
    receiver.clear();
    for (int id : ids) REQUIRE(weak[id].expired());
    for (int id : rest) REQUIRE(!weak[id].expired());
    tail.reverse(0, tail.size()); reverse(rest.begin(), rest.end());
    int last = tail.size() - 4;
    auto part = tail.extract(3, last);
    vector<int> middle(rest.begin() + 3, rest.begin() + last);
    rest.erase(rest.begin() + 3, rest.begin() + last);
    tail.clear();
    for (int id : rest) REQUIRE(weak[id].expired());
    check_ids(part, middle);
    SharedTree destination;
    weak[71] = append(destination, 71);
    destination = std::move(part);
    REQUIRE(part.empty() && weak[71].expired());
    check_ids(destination, middle);
    auto survivor = destination.split(16);
    vector<int> suffix(middle.begin() + 16, middle.end()); middle.resize(16);
    destination.clear();
    for (int id : middle) REQUIRE(weak[id].expired());
    check_ids(survivor, suffix);
    survivor.clear();
    for (const auto& x : weak) REQUIRE(x.expired());
    REQUIRE(Resource::live == 0);
}

// Verified existing F subset: no copy/move construction, copy assignment,
// id() prvalue, and composition() returning const F&.
struct alignas(128) AssignedAction {
    static inline int live = 0;
    int level;
    shared_ptr<Resource> resource;
    explicit AssignedAction(int x) : level(x) { ++live; }
    AssignedAction(const AssignedAction&) = delete;
    AssignedAction(AssignedAction&&) = delete;
    AssignedAction& operator=(const AssignedAction&) = default;
    ~AssignedAction() { --live; }
};
struct AssignedMax {
    using S = int;
    using F = AssignedAction;
    static S e() { return 0; }
    static S op(S x, S y) { return max(x, y); }
    static F id() { return F(0); }
    static const F& composition(const F& f, const F& g) { return f.level >= g.level ? f : g; }
    static S mapping(const F& f, S x) { return max(f.level, x); }
};
static_assert(!is_copy_constructible_v<AssignedAction> && !is_move_constructible_v<AssignedAction>);
static_assert(is_copy_assignable_v<AssignedAction> && alignof(AssignedAction) == 128);
void assigned_action_split_concat_lifetimes() {
    REQUIRE(AssignedAction::live == 0 && Resource::live == 0);
    vector<weak_ptr<Resource>> weak(3);
    {
        LazySequenceTree<AssignedMax> t(vector<int>(81, 1));
        LazySequenceTree<AssignedMax> independent(vector<int>(37, 2));
        {
            AssignedAction f(7); f.resource = make_shared<Resource>(100); weak[0] = f.resource;
            t.apply(0, t.size(), f);
        }
        {
            AssignedAction f(11); f.resource = make_shared<Resource>(101); weak[1] = f.resource;
            t.apply(3, 79, f);
        }
        {
            AssignedAction f(13); f.resource = make_shared<Resource>(102); weak[2] = f.resource;
            independent.apply(0, independent.size(), f);
        }
        t.reverse(0, t.size());
        t.concat(std::move(independent));
        REQUIRE(independent.empty());
        independent.insert(0, 3);
        REQUIRE(independent.get(0) == 3);
        auto tail = t.split(3);
        auto middle = tail.extract(12, 83);
        t.clear(); tail.clear(); independent.clear();
        REQUIRE(middle.size() == 71);
        for (int i = 0; i < middle.size(); ++i) {
            // Original reversed indices 15..80 followed by the first 5 donors.
            int original_index = i + 15;
            int expected = original_index < 78 ? 11 : original_index < 81 ? 7 : 13;
            REQUIRE(middle.get(i) == expected);
        }
        middle.apply(0, middle.size(), AssignedAction(17));
        REQUIRE(middle.all_prod() == 17);
        middle.clear();
    }
    for (const auto& x : weak) REQUIRE(x.expired());
    REQUIRE(AssignedAction::live == 0 && Resource::live == 0);
}
} // namespace block_type_fixture

int main() {
    try {
        using namespace block_type_fixture;
        intermediate_capacity_boundaries<40>();
        intermediate_capacity_boundaries<80>();
        intermediate_capacity_boundaries<176>();
        one_element_blocks_nonunital_nonidempotent();
        copy_adl_boundary_edits();
        move_assignment_value_subsets();
        independent_tree_value_lifetimes();
        assigned_action_split_concat_lifetimes();
        cout << "Hello World" << '\n';
    } catch (const exception& error) {
        cerr << "block type/resource fixture FAIL " << error.what() << '\n';
        return 1;
    }
}
