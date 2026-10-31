#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/bbst/lazy_sequence.hpp"

namespace sequence_handles_test {
struct WordMonoid {
    using S = string;
    static S e() { return {}; }
    static S op(const S& a, const S& b) { return a + b; }
};
struct WordAction : WordMonoid {
    using F = pair<int, int>;
    static F id() { return {1, 0}; }
    static F composition(F f, F g) {
        return {f.first * g.first % 26, (f.first * g.second + f.second) % 26};
    }
    static S mapping(F f, S value) {
        for (char& c : value) c = char('a' + (f.first * (c - 'a') + f.second) % 26);
        return value;
    }
};

template<class Tree> struct Entry {
    typename Tree::Handle handle;
    string value;
};

template<class Tree> void check(const Tree& tree, const vector<Entry<Tree>>& model) {
    using Handle = typename Tree::Handle;
    assert(tree.size() == int(model.size()));
    assert(tree.index_of(Handle{}) == nullopt);
    // Query every retained handle before positional access pushes lazy tags.
    for (int i = tree.size(); i-- > 0;) assert(tree.index_of(model[i].handle) == i);
    string product;
    for (int i = 0; i < tree.size(); ++i) {
        assert(tree.handle_at(i) == model[i].handle);
        assert(tree.get(i) == model[i].value);
        product += model[i].value;
    }
    assert(tree.all_prod() == product);
    auto values = tree.content();
    for (int i = 0; i < tree.size(); ++i) assert(values[i] == model[i].value);
}

template<class Tree> void construction_and_lifetime() {
    using Handle = typename Tree::Handle;
    static_assert(is_trivially_copyable_v<Handle>);
    static_assert(is_default_constructible_v<Handle>);
    static_assert(is_same_v<decltype(declval<Tree&>().insert(0, "a")), Handle>);
    static_assert(is_same_v<decltype(declval<const Tree&>().handle_at(0)), Handle>);
    static_assert(is_same_v<decltype(declval<const Tree&>().index_of(Handle{})), optional<int>>);
    static_assert(is_same_v<decltype(Handle{} == Handle{}), bool>);
    static_assert(is_same_v<decltype(Handle{} != Handle{}), bool>);
    assert(Handle{} == Handle{});

    Tree tree({"a", "a", "b", "a"}), other;
    vector<Entry<Tree>> model;
    for (int i = 0; i < tree.size(); ++i) model.push_back({tree.handle_at(i), tree.get(i)});
    assert(model[0].handle != model[1].handle);
    Handle copied = model[1].handle;
    assert(copied == tree.handle_at(1));
    check(tree, model);

    tree.reverse(0, tree.size());
    reverse(model.begin(), model.end());
    for (const auto& entry : model) assert(other.index_of(entry.handle) == nullopt);
    check(tree, model);
    Tree moved(std::move(tree));
    check(moved, model);
    for (const auto& entry : model) assert(tree.index_of(entry.handle) == nullopt);
    tree = std::move(moved);
    check(tree, model);
    auto* original = &tree;
    tree = std::move(*original);
    check(tree, model);

    auto tail = tree.split(2);
    vector<Entry<Tree>> right(model.begin() + 2, model.end());
    model.resize(2);
    for (const auto& entry : model) assert(tail.index_of(entry.handle) == nullopt);
    for (const auto& entry : right) assert(tree.index_of(entry.handle) == nullopt);
    check(tree, model);
    check(tail, right);
    tree.concat(std::move(tail));
    model.insert(model.end(), right.begin(), right.end());
    for (const auto& entry : model) assert(tail.index_of(entry.handle) == nullopt);
    check(tree, model);

    other.insert(0, "z");
    other = std::move(tree);
    check(other, model);
    auto* self = &other;
    other = std::move(*self);
    check(other, model);
    // Existing void-style calls remain usable, and later positional access
    // obtains a handle for the same node.
    other.insert(2, "c");
    model.insert(model.begin() + 2, {other.handle_at(2), "c"});
    other.set(2, "a");
    model[2].value = "a";
    check(other, model);
}

template<bool Lazy> void randomized() {
    using Tree = conditional_t<Lazy, LazySequenceTree<WordAction>, SequenceTree<WordMonoid>>;
    array<Tree, 4> trees;
    array<vector<Entry<Tree>>, 4> models;
    mt19937 rng(20261028 + Lazy);
    auto pick = [&](int n) { return int(rng() % n); };
    for (int group = 0; group < 4; ++group)
        for (int i = 0; i < 24; ++i) {
            string value(1, char('a' + pick(4)));
            models[group].push_back({trees[group].insert(trees[group].size(), value), value});
        }

    for (int step = 0; step < 16000; ++step) {
        int group = pick(4), target = (group + 1 + pick(3)) % 4;
        auto& tree = trees[group];
        auto& model = models[group];
        int n = tree.size(), l = pick(n + 1), r = pick(n + 1);
        if (l > r) swap(l, r);
        int type = pick(24);
        if (type < 8 && n < 250) {
            string value(1, char('a' + pick(4)));
            auto handle = tree.insert(l, value);
            model.insert(model.begin() + l, {handle, value});
        } else if (type == 8 && n) {
            int p = pick(n);
            tree.erase(p);
            model.erase(model.begin() + p);
        } else if (type == 9) {
            tree.erase(l, r);
            model.erase(model.begin() + l, model.begin() + r);
        } else if (type == 10 && n) {
            int p = pick(n);
            model[p].value = string(1, char('a' + pick(4)));
            tree.set(p, model[p].value);
        } else if (type == 11) {
            tree.reverse(l, r);
            reverse(model.begin() + l, model.begin() + r);
        } else if (type == 12) {
            int k = pick(n - (r - l) + 1);
            tree.move(l, r, k);
            vector<Entry<Tree>> part(model.begin() + l, model.begin() + r);
            model.erase(model.begin() + l, model.begin() + r);
            model.insert(model.begin() + k, part.begin(), part.end());
        } else if (type == 13) {
            int middle = l + pick(r - l + 1);
            tree.rotate(l, middle, r);
            rotate(model.begin() + l, model.begin() + middle, model.begin() + r);
        } else if (type == 14) {
            auto tail = tree.split(l);
            trees[target].concat(std::move(tail));
            models[target].insert(models[target].end(), model.begin() + l, model.end());
            model.resize(l);
        } else if (type == 15) {
            auto part = tree.extract(l, r);
            trees[target].concat(std::move(part));
            models[target].insert(models[target].end(), model.begin() + l, model.begin() + r);
            model.erase(model.begin() + l, model.begin() + r);
        } else if (type == 16) {
            tree.concat(std::move(trees[target]));
            model.insert(model.end(), models[target].begin(), models[target].end());
            models[target].clear();
        } else if (type == 17) {
            swap(tree, trees[target]);
            swap(model, models[target]);
        } else if (type == 18) {
            Tree temporary(std::move(tree));
            tree = std::move(temporary);
        } else if (type == 19 && pick(8) == 0) {
            tree = std::move(trees[target]);
            model = std::move(models[target]);
            models[target].clear();
        } else if (type == 20 && pick(20) == 0) {
            tree.clear();
            model.clear();
        } else if constexpr (Lazy) {
            WordAction::F f{pick(26), pick(26)};
            tree.apply(l, r, f);
            for (int i = l; i < r; ++i) model[i].value = WordAction::mapping(f, model[i].value);
        }
        // Deliberately leave several consecutive edits and lazy operations
        // pending before inspecting all retained handles.
        if (step % 13 == 0) {
            for (int owner = 0; owner < 4; ++owner)
                for (int i = int(models[owner].size()); i-- > 0;) {
                    auto handle = models[owner][i].handle;
                    for (int j = 0; j < 4; ++j)
                        assert(trees[j].index_of(handle) == (j == owner ? optional<int>(i) : nullopt));
                }
            for (int owner = 0; owner < 4; ++owner) check(trees[owner], models[owner]);
        }
    }
    for (int owner = 0; owner < 4; ++owner) check(trees[owner], models[owner]);
}

void page_reuse_and_deferred_reversals() {
    using Tree = LazySequenceTree<WordAction>;
    vector<string> values(4097, "a");
    Tree tree(values), separate(values);
    vector<Entry<Tree>> model;
    for (int i = 0; i < tree.size(); ++i) model.push_back({tree.handle_at(i), "a"});
    for (int step = 0; step < 37; ++step) {
        int l = step, r = tree.size() - step;
        tree.reverse(l, r);
        reverse(model.begin() + l, model.begin() + r);
        tree.apply(l, r, {1, 1});
        for (int i = l; i < r; ++i) model[i].value = WordAction::mapping({1, 1}, model[i].value);
        for (int i = 0; i < tree.size(); ++i) assert(tree.index_of(model[i].handle) == i);
    }
    check(tree, model);
    tree.erase(10, tree.size() - 10);
    model.erase(model.begin() + 10, model.end() - 10);
    separate.clear();
    for (int i = 0; i < 4097; ++i) {
        auto handle = separate.insert(separate.size(), "b");
        assert(tree.index_of(handle) == nullopt);
    }
    check(tree, model);
    separate.clear();
    for (int i = 0; i < 2049; ++i) model.push_back({tree.insert(tree.size(), "c"), "c"});
    tree.reverse(0, tree.size());
    reverse(model.begin(), model.end());
    check(tree, model);
}
} // namespace sequence_handles_test

int main() {
    using namespace sequence_handles_test;
    construction_and_lifetime<SequenceTree<WordMonoid>>();
    construction_and_lifetime<LazySequenceTree<WordAction>>();
    randomized<false>();
    randomized<true>();
    page_reuse_and_deferred_reversals();
    cout << "Hello World\n";
}
