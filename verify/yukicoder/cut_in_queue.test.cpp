#define PROBLEM "https://yukicoder.me/problems/no/3167"

#include "ds/bbst/sequence.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<ll> a(n);
    for (auto& x : a) cin >> x;
    int q;
    cin >> q;
    using Tree = SequenceTree<MonoidAdd<ll>>;
    Tree tree(a);
    vector<Tree::Handle> handles(n + q + 1);
    for (int i = 0; i < n; ++i) handles[a[i]] = tree.handle_at(i);
    int next_value = n + 1;
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int a;
            cin >> a;
            int position = a == 0 ? tree.size() : *tree.index_of(handles[a]);
            handles[next_value] = tree.insert(position, next_value);
            ++next_value;
        } else if (type == 2) {
            tree.erase(0);
        } else {
            int k;
            cin >> k;
            cout << tree.get(k - 1) << '\n';
        }
    }
}
