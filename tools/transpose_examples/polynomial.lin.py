"""固定多項式との畳み込み・剰余・NTT を使う生成例。"""


def fixed_convolution(x: Vec, kernel: "const std::vector<T>&") -> "0 if len(x) == 0 or len(kernel) == 0 else len(x) + len(kernel) - 1":
    y = convolution(x, kernel)
    return y


def fixed_remainder(x: Vec, divisor: "const std::vector<T>&") -> "len(divisor) - 1":
    y = poly_mod(x, divisor)
    return y


def fourier(x: Vec) -> "len(x)":
    y = ntt(x)
    return y


def inverse_fourier(x: Vec) -> "len(x)":
    y = intt(x)
    return y


def newton_basis(x: Vec, products: "const std::vector<std::vector<T>>&", node: "int") -> "len(x)":
    # products は (X + b[i]) の積木。入力の長さは葉数（2 の冪）。
    n = len(x)
    y = zeros(n)
    if n == 1:
        y[0] = x[0]
    else:
        a = slice(x, 0, n // 2)
        b = slice(x, n // 2, n)
        c = newton_basis(a, products, 2 * node)
        d = newton_basis(b, products, 2 * node + 1)
        e = convolution(d, products[2 * node])
        for i in range(len(c)):
            y[i] += c[i]
        for i in range(len(e)):
            y[i] += e[i]
    return y


def evaluate_tree(x: Vec, products: "const std::vector<std::vector<T>>&", node: "int", count: "int") -> "count":
    # products は (X - a[i]) の積木。count はこの部分木の葉数。
    y = zeros(count)
    if count == 1:
        for i in range(len(x)):
            y[0] += x[i] * (-products[node][0]).pow(i)
    else:
        a = poly_mod(x, products[2 * node])
        b = poly_mod(x, products[2 * node + 1])
        c = evaluate_tree(a, products, 2 * node, count // 2)
        d = evaluate_tree(b, products, 2 * node + 1, count // 2)
        for i in range(count // 2):
            y[i] += c[i]
            y[count // 2 + i] += d[i]
    return y
