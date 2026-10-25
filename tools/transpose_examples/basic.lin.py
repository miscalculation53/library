"""AtCoder 転置原理入門の例。区間は [l, r)。"""


def range_sum(x: Vec, left: "const std::vector<int>&", right: "const std::vector<int>&") -> "len(left)":
    assert len(left) == len(right)
    s = zeros(len(x) + 1)
    y = zeros(len(left))
    for i in range(len(x)):
        s[i + 1] = s[i] + x[i]
    for i in range(len(left)):
        y[i] = s[right[i]] - s[left[i]]
    return y


def subset_zeta(x: Vec) -> "len(x)":
    assert len(x) > 0 and has_single_bit(len(x))
    for bit in range(bit_width(len(x)) - 1):
        d = 1 << bit
        for block in range(0, len(x), 2 * d):
            for i in range(block, block + d):
                x[i + d] += x[i]
    return x


def divisor_zeta(x: Vec, primes: "const std::vector<int>&") -> "len(x)":
    for p in primes:
        for i in range(1, (len(x) - 1) // p + 1):
            x[i * p] += x[i]
    return x


def fenwick_queries(x: Vec, n: "int", kind: "const std::vector<int>&", left: "const std::vector<int>&", right: "const std::vector<int>&") -> "len(kind)":
    # x[:n] は初期値。kind[t] == 0 なら left[t] に x[n+t] を加算。
    # kind[t] == 1 なら [left[t], right[t]) の和を y[t] に出力。
    assert len(x) == n + len(kind)
    assert len(left) == len(kind) and len(right) == len(kind)
    tree = zeros(n + 1)
    y = zeros(len(kind))
    for i in range(n):
        tree[i + 1] += x[i]
    for i in range(1, n + 1):
        parent = i + (i & -i)
        if parent <= n:
            tree[parent] += tree[i]
    for t in range(len(kind)):
        if kind[t] == 0:
            for b in range(bit_width(n)):
                if (left[t] & (1 << b)) == 0:
                    p = ((left[t] >> b) + 1) << b
                    if p <= n:
                        tree[p] += x[n + t]
        else:
            for b in range(bit_width(n)):
                if right[t] & (1 << b):
                    y[t] += tree[right[t] & ~((1 << b) - 1)]
                if left[t] & (1 << b):
                    y[t] -= tree[left[t] & ~((1 << b) - 1)]
    return y
