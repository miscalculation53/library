#pragma once

/*! SPDX-License-Identifier: CC0-1.0
 * Independent implementation from the dynamic-sequence API contract and the
 * mathematical definition of a Cartesian tree with random priorities.
 */

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <new>
#include <optional>
#include <random>
#include <type_traits>
#include <utility>
#include <vector>

// The legacy type aliases are part of the public API. The implementation below
// uses its own nodes and does not call legacy node or tree operations.
#include "treap_base.hpp"

/**
 * @brief 挿入・削除・反転・区間積ができる列
 * @docs docs/ds/bbst/sequence.md
 */

namespace bbst_detail {

template <class Target, class Source>
void independent_sequence_assign(Target& destination, Source&& value) {
    if constexpr (std::is_assignable_v<Target&, Source&&>)
        destination = std::forward<Source>(value);
    else if constexpr (std::is_assignable_v<Target&, const std::remove_reference_t<Source>&>)
        destination = static_cast<const std::remove_reference_t<Source>&>(value);
    else destination = Target(value);
}

template <class A, bool Copy = std::is_copy_constructible_v<typename A::F>>
struct IndependentSequencePending;

template <class A> struct IndependentSequencePending<A, true> {
    using F = typename A::F;
    std::optional<F> pending;
    bool has_pending() const { return pending.has_value(); }
    F& get_pending() { return *pending; }
    void remember(const F& f) { pending.emplace(f); }
    void forget() { pending.reset(); }
};

template <class A> struct IndependentSequencePending<A, false> {
    using F = typename A::F;
    F pending;
    bool active = false;
    IndependentSequencePending() : pending(A::id()) {}
    bool has_pending() const { return active; }
    F& get_pending() { return pending; }
    void remember(const F& f) {
        if constexpr (std::is_assignable_v<F&, const F&>) pending = f;
        else independent_sequence_assign(pending, A::composition(f, pending));
        active = true;
    }
    void forget() {
        independent_sequence_assign(pending, A::id());
        active = false;
    }
};

template <class A, bool Lazy> struct IndependentSequenceAction {};
template <class A> struct IndependentSequenceAction<A, true> : IndependentSequencePending<A> {};

template <class A, bool Lazy> class SequenceTreeImpl {
public:
    using Node = TreapNode<A, true, Lazy>;
    using S = typename A::S;
    using F = typename Node::F;

private:
    struct Page;
    struct Cell : IndependentSequenceAction<A, Lazy> {
        Cell* left = nullptr;
        Cell* right = nullptr;
        Page* page;
        std::uint64_t priority;
        int count = 1;
        bool reversed = false;
        S value;
        S forward;
        S backward;

        template <class T>
        Cell(Page* p, std::uint64_t priority_, const T& x)
            : page(p), priority(priority_), value(x), forward(value), backward(value) {}
    };

    union Slot {
        Cell cell;
        Slot* next;
        Slot() {}
        ~Slot() {}
    };

    static constexpr std::size_t page_slot_bytes = 32 * 1024;
    static constexpr std::size_t capacity_by_size = page_slot_bytes / sizeof(Slot);
    static constexpr int page_capacity = capacity_by_size == 0 ? 1
        : capacity_by_size < 256 ? static_cast<int>(capacity_by_size) : 256;
    struct Page {
        Page* previous = nullptr;
        Page* next = nullptr;
        Slot* available = nullptr;
        int live = 0;
        Slot slots[page_capacity];

        Page() {
            for (int i = page_capacity; i-- > 0;) {
                slots[i].next = available;
                available = slots + i;
            }
        }
    };

    struct Pool {
        Page* available = nullptr;

        void link(Page* p) {
            p->previous = nullptr;
            p->next = available;
            if (available) available->previous = p;
            available = p;
        }
        void unlink(Page* p) {
            if (p->previous) p->previous->next = p->next;
            else available = p->next;
            if (p->next) p->next->previous = p->previous;
        }
        template <class T> Cell* make(const T& x) {
            if (!available) link(new Page);
            Page* p = available;
            Slot* s = p->available;
            p->available = s->next;
            ++p->live;
            if (!p->available) unlink(p);
            Cell* result;
            try {
                result = ::new (static_cast<void*>(&s->cell)) Cell(p, random_priority(), x);
            } catch (...) {
                release(p, s);
                throw;
            }
            return result;
        }
        void release(Page* p, Slot* s) {
            bool was_full = p->available == nullptr;
            s->next = p->available;
            p->available = s;
            if (was_full) link(p);
            if (--p->live == 0) {
                unlink(p);
                delete p;
            }
        }
        void destroy(Cell* x) {
            Page* p = x->page;
            x->~Cell();
            release(p, reinterpret_cast<Slot*>(x));
        }
    };

    Cell* root_ = nullptr;

    static Pool& pool() {
        static Pool storage;
        return storage;
    }
    static std::uint64_t random_priority() {
        static std::mt19937_64 engine = [] {
            std::random_device source;
            std::seed_seq seed{source(), source(), source(), source(),
                               source(), source(), source(), source()};
            return std::mt19937_64(seed);
        }();
        return engine();
    }
    static int count(Cell* u) { return u ? u->count : 0; }
    template <class T> static void consume(T& destination, T& value) {
        independent_sequence_assign(destination, std::move(value));
    }
    template <class Target, class Source>
    static void assign_result(Target& destination, Source&& value) {
        independent_sequence_assign(destination, std::forward<Source>(value));
    }

    static void pull(Cell* u) {
        u->count = count(u->left) + 1 + count(u->right);
        if (u->left && u->right) {
            assign_result(u->forward, A::op(A::op(u->left->forward, u->value), u->right->forward));
            assign_result(u->backward, A::op(A::op(u->right->backward, u->value), u->left->backward));
        } else if (u->left) {
            assign_result(u->forward, A::op(u->left->forward, u->value));
            assign_result(u->backward, A::op(u->value, u->left->backward));
        } else if (u->right) {
            assign_result(u->forward, A::op(u->value, u->right->forward));
            assign_result(u->backward, A::op(u->right->backward, u->value));
        } else {
            assign_result(u->forward, u->value);
            assign_result(u->backward, u->value);
        }
    }
    static void flip(Cell* u) {
        if (!u) return;
        std::swap(u->left, u->right);
        using std::swap;
        swap(u->forward, u->backward);
        u->reversed = !u->reversed;
    }
    static void paint(Cell* u, const F& f) {
        if constexpr (Lazy) {
            if (!u) return;
            assign_result(u->value, A::mapping(f, u->value));
            if (u->count == 1) {
                assign_result(u->forward, u->value);
                assign_result(u->backward, u->value);
            } else {
                assign_result(u->forward, A::mapping(f, u->forward));
                assign_result(u->backward, A::mapping(f, u->backward));
                if (u->has_pending()) assign_result(u->get_pending(), A::composition(f, u->get_pending()));
                else u->remember(f);
            }
        }
    }
    static void push(Cell* u) {
        if (u->reversed) {
            flip(u->left);
            flip(u->right);
            u->reversed = false;
        }
        if constexpr (Lazy) {
            if (u->has_pending()) {
                paint(u->left, u->get_pending());
                paint(u->right, u->get_pending());
                u->forget();
            }
        }
    }

    static void destroy(Cell* u) {
        // A right rotation removes one left edge; a node with no left child
        // can then be destroyed while its right child remains reachable.
        while (u) {
            if (u->left) {
                Cell* next = u->left;
                u->left = next->right;
                next->right = u;
                u = next;
            } else {
                Cell* next = u->right;
                pool().destroy(u);
                u = next;
            }
        }
    }

    static std::pair<Cell*, Cell*> cut(Cell* u, int k) {
        if (!u) return {nullptr, nullptr};
        if (k == 0) return {nullptr, u};
        if (k == u->count) return {u, nullptr};
        push(u);
        int left = count(u->left);
        if (k <= left) {
            auto result = cut(u->left, k);
            u->left = result.second;
            pull(u);
            result.second = u;
            return result;
        }
        auto result = cut(u->right, k - left - 1);
        u->right = result.first;
        pull(u);
        result.first = u;
        return result;
    }
    static Cell* connect(Cell* a, Cell* b) {
        if (!a) return b;
        if (!b) return a;
        if (a->priority > b->priority) {
            push(a);
            a->right = connect(a->right, b);
            pull(a);
            return a;
        }
        push(b);
        b->left = connect(a, b->left);
        pull(b);
        return b;
    }
    static Cell* insert_cell(Cell* u, int k, Cell* added) {
        if (!u) return added;
        if (added->priority > u->priority) {
            auto parts = cut(u, k);
            added->left = parts.first;
            added->right = parts.second;
            pull(added);
            return added;
        }
        push(u);
        int left = count(u->left);
        if (k <= left) u->left = insert_cell(u->left, k, added);
        else u->right = insert_cell(u->right, k - left - 1, added);
        pull(u);
        return u;
    }
    static Cell* erase_cell(Cell* u, int k) {
        push(u);
        int left = count(u->left);
        if (k < left) u->left = erase_cell(u->left, k);
        else if (k > left) u->right = erase_cell(u->right, k - left - 1);
        else {
            Cell* result = connect(u->left, u->right);
            pool().destroy(u);
            return result;
        }
        pull(u);
        return u;
    }
    static void replace(Cell* u, int k, const S& x) {
        push(u);
        int left = count(u->left);
        if (k < left) replace(u->left, k, x);
        else if (k > left) replace(u->right, k - left - 1, x);
        else assign_result(u->value, x);
        pull(u);
    }
    static S query(Cell* u, int l, int r) {
        if (l == r) return A::e();
        if (l == 0 && r == u->count) return u->forward;
        push(u);
        int left = count(u->left);
        if (r <= left) return query(u->left, l, r);
        if (l > left) return query(u->right, l - left - 1, r - left - 1);
        S result = u->value;
        if (l < left) assign_result(result, A::op(query(u->left, l, left), result));
        if (r > left + 1) assign_result(result, A::op(result, query(u->right, 0, r - left - 1)));
        if constexpr (std::is_move_constructible_v<S>) return result;
        else return static_cast<const S&>(result);
    }
    static void update(Cell* u, int l, int r, const F& f) {
        if (l == r) return;
        if (l == 0 && r == u->count) {
            paint(u, f);
            return;
        }
        push(u);
        int left = count(u->left);
        if (l < left) update(u->left, l, std::min(r, left), f);
        if (l <= left && left < r) assign_result(u->value, A::mapping(f, u->value));
        if (r > left + 1) update(u->right, std::max(0, l - left - 1), r - left - 1, f);
        pull(u);
    }
    static void enumerate(Cell* u, std::vector<S>& result) {
        if (!u) return;
        push(u);
        enumerate(u->left, result);
        result.push_back(u->value);
        enumerate(u->right, result);
    }

    template <class G>
    static bool scan_right(Cell* u, int offset, int start, S& accumulated,
                           int& boundary, const G& g) {
        if (!u || offset + u->count <= start) return true;
        if (start <= offset) {
            S candidate = A::op(accumulated, u->forward);
            if (g(candidate)) {
                consume(accumulated, candidate);
                boundary = offset + u->count;
                return true;
            }
        }
        push(u);
        int middle = offset + count(u->left);
        if (!scan_right(u->left, offset, start, accumulated, boundary, g)) return false;
        if (start <= middle) {
            S candidate = A::op(accumulated, u->value);
            if (!g(candidate)) return false;
            consume(accumulated, candidate);
            boundary = middle + 1;
        }
        return scan_right(u->right, middle + 1, start, accumulated, boundary, g);
    }
    template <class G>
    static bool scan_left(Cell* u, int offset, int end, S& accumulated,
                          int& boundary, const G& g) {
        if (!u || end <= offset) return true;
        if (offset + u->count <= end) {
            S candidate = A::op(u->forward, accumulated);
            if (g(candidate)) {
                consume(accumulated, candidate);
                boundary = offset;
                return true;
            }
        }
        push(u);
        int middle = offset + count(u->left);
        if (!scan_left(u->right, middle + 1, end, accumulated, boundary, g)) return false;
        if (middle < end) {
            S candidate = A::op(u->value, accumulated);
            if (!g(candidate)) return false;
            consume(accumulated, candidate);
            boundary = middle;
        }
        return scan_left(u->left, offset, end, accumulated, boundary, g);
    }

    template <class Iter> void construct(Iter first, Iter last) {
        std::vector<Cell*> spine;
        Cell* unattached = nullptr;
        try {
            for (; first != last; ++first) {
                const auto& value = *first;
                Cell* added = pool().make(value);
                unattached = added;
                Cell* previous = nullptr;
                while (!spine.empty() && spine.back()->priority < added->priority) {
                    previous = spine.back();
                    pull(previous);
                    spine.pop_back();
                }
                added->left = previous;
                if (spine.empty()) root_ = added;
                else spine.back()->right = added;
                unattached = nullptr;
                spine.push_back(added);
            }
            while (!spine.empty()) {
                pull(spine.back());
                spine.pop_back();
            }
        } catch (...) {
            if (unattached) pool().destroy(unattached);
            clear();
            throw;
        }
    }

public:
    SequenceTreeImpl() = default;
    template <class Iter> SequenceTreeImpl(Iter first, Iter last) { construct(first, last); }
    template <class T> explicit SequenceTreeImpl(const std::vector<T>& values)
        : SequenceTreeImpl(values.begin(), values.end()) {}
    SequenceTreeImpl(std::initializer_list<S> values)
        : SequenceTreeImpl(values.begin(), values.end()) {}
    SequenceTreeImpl(const SequenceTreeImpl&) = delete;
    SequenceTreeImpl& operator=(const SequenceTreeImpl&) = delete;
    SequenceTreeImpl(SequenceTreeImpl&& other) noexcept : root_(other.root_) {
        other.root_ = nullptr;
    }
    SequenceTreeImpl& operator=(SequenceTreeImpl&& other) noexcept {
        if (this != &other) {
            clear();
            root_ = other.root_;
            other.root_ = nullptr;
        }
        return *this;
    }
    ~SequenceTreeImpl() { clear(); }

    int size() const { return count(root_); }
    bool empty() const { return root_ == nullptr; }
    void clear() {
        Cell* old = root_;
        root_ = nullptr;
        destroy(old);
    }
    std::vector<S> content() const {
        std::vector<S> result;
        result.reserve(size());
        enumerate(root_, result);
        return result;
    }
    S get(int p) const {
        assert(0 <= p && p < size());
        Cell* u = root_;
        for (;;) {
            push(u);
            int left = count(u->left);
            if (p < left) u = u->left;
            else if (p > left) {
                p -= left + 1;
                u = u->right;
            } else return u->value;
        }
    }
    void set(int p, const S& x) {
        assert(0 <= p && p < size());
        replace(root_, p, x);
    }
    void insert(int p, const S& x) {
        assert(0 <= p && p <= size());
        root_ = insert_cell(root_, p, pool().make(x));
    }
    void erase(int p) {
        assert(0 <= p && p < size());
        root_ = erase_cell(root_, p);
    }
    void erase(int l, int r) {
        assert(0 <= l && l <= r && r <= size());
        if (l == r) return;
        auto end = cut(root_, r);
        auto begin = cut(end.first, l);
        root_ = connect(begin.first, end.second);
        destroy(begin.second);
    }
    S prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= size());
        return query(root_, l, r);
    }
    S all_prod() const { return root_ ? root_->forward : A::e(); }
    SequenceTreeImpl split(int k) {
        assert(0 <= k && k <= size());
        auto parts = cut(root_, k);
        root_ = parts.first;
        SequenceTreeImpl result;
        result.root_ = parts.second;
        return result;
    }
    void concat(SequenceTreeImpl&& other) {
        assert(this != &other);
        root_ = connect(root_, other.root_);
        other.root_ = nullptr;
    }
    SequenceTreeImpl extract(int l, int r) {
        assert(0 <= l && l <= r && r <= size());
        if (l == r) return {};
        auto end = cut(root_, r);
        auto begin = cut(end.first, l);
        root_ = connect(begin.first, end.second);
        SequenceTreeImpl result;
        result.root_ = begin.second;
        return result;
    }
    void reverse(int l, int r) {
        assert(0 <= l && l <= r && r <= size());
        if (r - l <= 1) return;
        auto end = cut(root_, r);
        auto begin = cut(end.first, l);
        flip(begin.second);
        root_ = connect(connect(begin.first, begin.second), end.second);
    }
    void move(int l, int r, int k) {
        assert(0 <= l && l <= r && r <= size());
        assert(0 <= k && k <= size() - (r - l));
        if (l == r || k == l) return;
        SequenceTreeImpl middle = extract(l, r);
        auto parts = cut(root_, k);
        root_ = connect(connect(parts.first, middle.root_), parts.second);
        middle.root_ = nullptr;
    }
    void rotate(int l, int m, int r) {
        assert(0 <= l && l <= m && m <= r && r <= size());
        if (l == m || m == r) return;
        move(l, m, r - (m - l));
    }
    template <class G> int max_right_ok(int l, const G& g) const {
        assert(0 <= l && l <= size());
        S accumulated = A::e();
        assert(g(accumulated));
        int boundary = l;
        scan_right(root_, 0, l, accumulated, boundary, g);
        return boundary;
    }
    template <class G> int min_left_ok(int r, const G& g) const {
        assert(0 <= r && r <= size());
        S accumulated = A::e();
        assert(g(accumulated));
        int boundary = r;
        scan_left(root_, 0, r, accumulated, boundary, g);
        return boundary;
    }
    template <bool Enabled = Lazy, std::enable_if_t<Enabled && Lazy, int> = 0>
    void apply(int p, const F& f) {
        assert(0 <= p && p < size());
        update(root_, p, p + 1, f);
    }
    template <bool Enabled = Lazy, std::enable_if_t<Enabled && Lazy, int> = 0>
    void apply(int l, int r, const F& f) {
        assert(0 <= l && l <= r && r <= size());
        if (l != r) update(root_, l, r, f);
    }
};

} // namespace bbst_detail

template <class M> using SequenceTree = bbst_detail::SequenceTreeImpl<M, false>;
