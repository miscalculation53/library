#!/usr/bin/env python3
"""Generate loop-preserving C++ linear maps and their transposes.

The input uses a restricted Python syntax; it is parsed, never executed.
See tools/transpose_examples/README.md for the language and contest workflow.
"""

from __future__ import annotations

import argparse
import ast
from dataclasses import dataclass, field
import json
from pathlib import Path
import sys


class Error(ValueError):
    pass


def fail(node: ast.AST, message: str):
    raise Error(f"line {getattr(node, 'lineno', 1)}: {message}")


def identifier(name: str, node: ast.AST):
    keywords = set("alignas alignof and and_eq asm atomic_cancel atomic_commit atomic_noexcept auto bitand bitor bool break case catch char char8_t char16_t char32_t class compl concept const consteval constexpr constinit const_cast continue co_await co_return co_yield decltype default delete do double dynamic_cast else enum explicit export extern false float for friend goto if inline int long mutable namespace new noexcept not not_eq nullptr operator or or_eq private protected public reflexpr register reinterpret_cast requires return short signed sizeof static static_assert static_cast struct switch synchronized template this thread_local throw true try typedef typeid typename union unsigned using virtual void volatile wchar_t while xor xor_eq".split())
    if not name.isascii() or name.startswith("_") or "__" in name or name in keywords | {"T", "Vec"}:
        fail(node, f"reserved name: {name}")


@dataclass
class Scope:
    declarations: list = field(default_factory=list)
    steps: list = field(default_factory=list)


@dataclass
class Function:
    name: str
    input: str
    params: list[tuple[str, str]]
    shape: ast.AST
    node: ast.FunctionDef
    scope: Scope | None = None
    output: str = ""


class Generator:
    primitives = {"copy", "slice", "resize", "reverse", "ntt", "intt", "convolution", "poly_mod"}

    def __init__(self, source: str):
        self.functions: dict[str, Function] = {}
        self.counter = 0
        module = ast.parse(source)
        for node in module.body:
            if isinstance(node, ast.Expr) and isinstance(node.value, ast.Constant) and isinstance(node.value.value, str):
                continue
            if not isinstance(node, ast.FunctionDef):
                fail(node, "the file must contain function definitions only")
            if node.decorator_list or node.args.defaults or node.args.kw_defaults or node.args.vararg or node.args.kwarg or node.args.kwonlyargs or node.args.posonlyargs:
                fail(node, "decorators, default arguments and variadic arguments are unsupported")
            identifier(node.name, node)
            if node.name in self.functions or node.name in self.primitives or node.name in {"zeros", "scalar"}:
                fail(node, f"duplicate or reserved function: {node.name}")
            args = node.args.args
            if not args or not isinstance(args[0].annotation, ast.Name) or args[0].annotation.id != "Vec":
                fail(node, "the first argument must have annotation Vec")
            params = []
            seen = set()
            for i, arg in enumerate(args):
                identifier(arg.arg, arg)
                if arg.arg in seen:
                    fail(arg, "duplicate argument")
                seen.add(arg.arg)
                if i:
                    if not isinstance(arg.annotation, ast.Constant) or not isinstance(arg.annotation.value, str):
                        fail(arg, "fixed arguments need a quoted C++ type")
                    params.append((arg.arg, arg.annotation.value))
            if not isinstance(node.returns, ast.Constant) or not isinstance(node.returns.value, str):
                fail(node, 'declare the output length, for example -> "len(x)"')
            try:
                shape = ast.parse(node.returns.value, mode="eval").body
            except SyntaxError:
                fail(node, "invalid output length expression")
            self.functions[node.name] = Function(node.name, args[0].arg, params, shape, node)
        if not self.functions:
            raise Error("no functions found")
        generated_names = set()
        for f in self.functions.values():
            names = {f.name + suffix for suffix in ["", "_transpose", "_output_size", "_check"]}
            if names & generated_names:
                fail(f.node, f"generated function name collision: {f.name}")
            generated_names |= names
        for f in self.functions.values():
            env = {f.input: "vec", **{name: "fixed" for name, _ in f.params}}
            self.fixed(f.shape, env)
            body = list(f.node.body)
            if body and isinstance(body[0], ast.Expr) and isinstance(body[0].value, ast.Constant) and isinstance(body[0].value.value, str):
                body.pop(0)
            if not body or not isinstance(body[-1], ast.Return) or not isinstance(body[-1].value, ast.Name):
                fail(f.node, "end the function with return <vector name>")
            f.output = body[-1].value.id
            f.scope = self.scope(body[:-1], env)
            if env.get(f.output) != "vec":
                fail(body[-1], "return a vector declared in the function's outer scope")

    def temp(self):
        self.counter += 1
        return f"_lt_{self.counter}"

    def active(self, node, env):
        if isinstance(node, ast.Call) and isinstance(node.func, ast.Name) and node.func.id == "len":
            return False
        if isinstance(node, ast.Name):
            return env.get(node.id) in {"vec", "scalar"}
        return any(self.active(child, env) for child in ast.iter_child_nodes(node))

    def fixed(self, node, env, sizes=None):
        sizes = sizes or {}
        if self.active(node, env):
            fail(node, "a coefficient, index, condition or loop bound depends on an active value")
        if isinstance(node, ast.Constant):
            if isinstance(node.value, bool):
                return "true" if node.value else "false"
            if isinstance(node.value, (int, float, str)):
                return json.dumps(node.value)
            fail(node, "unsupported literal")
        if isinstance(node, ast.Name):
            if node.id not in env and node.id != "T":
                fail(node, f"unknown fixed value: {node.id}")
            return node.id
        if isinstance(node, ast.Subscript):
            if isinstance(node.slice, ast.Slice):
                fail(node, "use slice(vector, l, r) for active vectors")
            return f"{self.fixed(node.value, env, sizes)}[{self.fixed(node.slice, env, sizes)}]"
        if isinstance(node, ast.Attribute):
            return f"{self.fixed(node.value, env, sizes)}.{node.attr}"
        if isinstance(node, ast.BinOp):
            operators = {ast.Add: "+", ast.Sub: "-", ast.Mult: "*", ast.Div: "/", ast.FloorDiv: "/", ast.Mod: "%", ast.LShift: "<<", ast.RShift: ">>", ast.BitAnd: "&", ast.BitOr: "|", ast.BitXor: "^"}
            if type(node.op) not in operators:
                fail(node, "unsupported fixed arithmetic")
            return f"({self.fixed(node.left, env, sizes)} {operators[type(node.op)]} {self.fixed(node.right, env, sizes)})"
        if isinstance(node, ast.UnaryOp):
            operators = {ast.UAdd: "+", ast.USub: "-", ast.Invert: "~", ast.Not: "!"}
            if type(node.op) not in operators:
                fail(node, "unsupported unary operation")
            return f"({operators[type(node.op)]}{self.fixed(node.operand, env, sizes)})"
        if isinstance(node, ast.BoolOp):
            op = " && " if isinstance(node.op, ast.And) else " || "
            return "(" + op.join(self.fixed(x, env, sizes) for x in node.values) + ")"
        if isinstance(node, ast.Compare):
            operators = {ast.Eq: "==", ast.NotEq: "!=", ast.Lt: "<", ast.LtE: "<=", ast.Gt: ">", ast.GtE: ">="}
            args = [node.left] + node.comparators
            parts = []
            for i, op in enumerate(node.ops):
                if type(op) not in operators:
                    fail(node, "unsupported comparison")
                parts.append(f"({self.fixed(args[i], env, sizes)} {operators[type(op)]} {self.fixed(args[i+1], env, sizes)})")
            return "(" + " && ".join(parts) + ")"
        if isinstance(node, ast.IfExp):
            return f"({self.fixed(node.test, env, sizes)} ? {self.fixed(node.body, env, sizes)} : {self.fixed(node.orelse, env, sizes)})"
        if isinstance(node, ast.Call):
            if node.keywords:
                fail(node, "keyword arguments are unsupported")
            if isinstance(node.func, ast.Name) and node.func.id == "len":
                if len(node.args) != 1:
                    fail(node, "len takes one array")
                a = node.args[0]
                if isinstance(a, ast.Name) and env.get(a.id) == "vec":
                    return sizes.get(a.id, f"int({a.id}.size())")
                return f"int({self.fixed(a, env, sizes)}.size())"
            if isinstance(node.func, ast.Name):
                name = node.func.id
                if name in self.functions or name in self.primitives or name in {"zeros", "scalar"}:
                    fail(node, "an active operation cannot be used as a fixed expression")
                name = {"min": "std::min", "max": "std::max", "abs": "std::abs"}.get(name, name)
            else:
                name = self.fixed(node.func, env, sizes)
            return name + "(" + ", ".join(self.fixed(a, env, sizes) for a in node.args) + ")"
        fail(node, "unsupported fixed expression")

    def reference(self, node, env):
        if isinstance(node, ast.Name) and env.get(node.id) == "scalar":
            return node.id
        if isinstance(node, ast.Subscript) and isinstance(node.value, ast.Name) and env.get(node.value.id) == "vec":
            return f"{node.value.id}[{self.fixed(node.slice, env)}]"
        fail(node, "expected an active scalar or vector element")

    def terms(self, node, env):
        """Return (coefficient, active reference) pairs; reject affine/nonlinear code."""
        if not self.active(node, env):
            if isinstance(node, ast.Constant) and node.value == 0:
                return []
            fail(node, "a linear expression may contain only a zero constant term")
        if isinstance(node, (ast.Name, ast.Subscript)):
            return [("1", self.reference(node, env))]
        if isinstance(node, ast.UnaryOp) and isinstance(node.op, (ast.UAdd, ast.USub)):
            terms = self.terms(node.operand, env)
            return terms if isinstance(node.op, ast.UAdd) else [(f"(-({c}))", ref) for c, ref in terms]
        if isinstance(node, ast.BinOp):
            if isinstance(node.op, (ast.Add, ast.Sub)):
                left, right = self.terms(node.left, env), self.terms(node.right, env)
                if isinstance(node.op, ast.Sub):
                    right = [(f"(-({c}))", ref) for c, ref in right]
                return left + right
            if isinstance(node.op, ast.Mult):
                la, ra = self.active(node.left, env), self.active(node.right, env)
                if la and ra:
                    fail(node, "nonlinear multiplication: both operands depend on the active input")
                variable, coeff = (node.left, node.right) if la else (node.right, node.left)
                c = self.fixed(coeff, env)
                return [(f"({k}) * T({c})", ref) for k, ref in self.terms(variable, env)]
            if isinstance(node.op, ast.Div) and not self.active(node.right, env):
                c = self.fixed(node.right, env)
                return [(f"({k}) / T({c})", ref) for k, ref in self.terms(node.left, env)]
        fail(node, "unsupported or nonlinear expression")

    def vector_call(self, node, env):
        if isinstance(node, ast.Name) and env.get(node.id) == "vec":
            return ("copy", node.id, [], f"int({node.id}.size())")
        if not isinstance(node, ast.Call) or not isinstance(node.func, ast.Name) or node.keywords:
            return None
        name = node.func.id
        if name not in self.primitives and name not in self.functions:
            return None
        if not node.args or not isinstance(node.args[0], ast.Name) or env.get(node.args[0].id) != "vec":
            fail(node, "the first argument of a vector operation must be an active vector name")
        src = node.args[0].id
        args = [self.fixed(a, env) for a in node.args[1:]]
        n = f"int({src}.size())"
        arity = {"copy": 0, "slice": 2, "resize": 1, "reverse": 0, "ntt": 0, "intt": 0, "convolution": 1, "poly_mod": 1}
        expected = arity.get(name, len(self.functions[name].params) if name in self.functions else 0)
        if len(args) != expected:
            fail(node, f"{name} expects {expected + 1} arguments")
        if name == "slice":
            shape = f"({args[1]} - {args[0]})"
        elif name == "resize":
            shape = args[0]
        elif name == "convolution":
            shape = f"({src}.empty() || ({args[0]}).empty() ? 0 : {n} + int(({args[0]}).size()) - 1)"
        elif name == "poly_mod":
            shape = f"(int(({args[0]}).size()) - 1)"
        elif name in self.functions:
            shape = f"{name}_output_size<T>({', '.join([n] + args)})"
        else:
            shape = n
        return name, src, args, shape

    def scope(self, body, env):
        result = Scope()
        for node in body:
            if isinstance(node, ast.Pass):
                continue
            if isinstance(node, ast.Assign):
                if len(node.targets) != 1:
                    fail(node, "use one assignment target")
                target = node.targets[0]
                if isinstance(target, ast.Name) and target.id not in env:
                    name = target.id
                    identifier(name, node)
                    value = node.value
                    if isinstance(value, ast.Call) and isinstance(value.func, ast.Name) and value.func.id in {"zeros", "scalar"}:
                        kind = value.func.id
                        if value.keywords or len(value.args) != (1 if kind == "zeros" else 0):
                            fail(node, f"invalid {kind} declaration")
                        shape = self.fixed(value.args[0], env) if kind == "zeros" else ""
                        result.declarations.append(("vec" if kind == "zeros" else "scalar", name, shape))
                        env[name] = "vec" if kind == "zeros" else "scalar"
                    elif (call := self.vector_call(value, env)) is not None:
                        result.declarations.append(("vec", name, call[3]))
                        env[name] = "vec"
                        result.steps.append(("call", name, call))
                    elif self.active(value, env):
                        terms = self.terms(value, env)
                        result.declarations.append(("scalar", name, ""))
                        env[name] = "scalar"
                        result.steps.append(("set", name, terms))
                    else:
                        result.declarations.append(("fixed", name, self.fixed(value, env)))
                        env[name] = "fixed"
                else:
                    dst = self.reference(target, env)
                    result.steps.append(("set", dst, self.terms(node.value, env)))
            elif isinstance(node, ast.AugAssign):
                dst = self.reference(node.target, env)
                if isinstance(node.op, (ast.Add, ast.Sub)):
                    terms = self.terms(node.value, env)
                    if isinstance(node.op, ast.Sub):
                        terms = [(f"(-({c}))", ref) for c, ref in terms]
                    result.steps.append(("add", dst, terms))
                elif isinstance(node.op, (ast.Mult, ast.Div)):
                    c = f"T({self.fixed(node.value, env)})"
                    if isinstance(node.op, ast.Div): c = f"T(1) / ({c})"
                    result.steps.append(("scale", dst, c))
                else:
                    fail(node, "supported updates: =, +=, -=, *= fixed, /= fixed")
            elif isinstance(node, ast.For):
                if not isinstance(node.target, ast.Name) or node.orelse:
                    fail(node, "for requires a single loop variable and no else clause")
                var = node.target.id
                identifier(var, node)
                if var in env:
                    fail(node, "use a fresh name for each nested loop variable")
                inner = dict(env, **{var: "fixed"})
                if isinstance(node.iter, ast.Call) and isinstance(node.iter.func, ast.Name) and node.iter.func.id == "range":
                    args = [self.fixed(a, env) for a in node.iter.args]
                    if node.iter.keywords or not 1 <= len(args) <= 3:
                        fail(node, "range accepts one to three fixed arguments")
                    if len(args) == 1: args = ["0", args[0], "1"]
                    if len(args) == 2: args += ["1"]
                    if args[2] == "0": fail(node, "range step must be nonzero")
                    result.steps.append(("range", var, args, self.scope(node.body, inner)))
                else:
                    result.steps.append(("each", var, self.fixed(node.iter, env), self.scope(node.body, inner)))
            elif isinstance(node, ast.If):
                result.steps.append(("if", self.fixed(node.test, env), self.scope(node.body, dict(env)), self.scope(node.orelse, dict(env))))
            elif isinstance(node, ast.Assert):
                result.steps.append(("assert", self.fixed(node.test, env)))
            else:
                fail(node, "supported statements: assignment, for, if and a final return")
        return result

    def signature(self, f, mode):
        fixed = [f"[[maybe_unused]] {typ} {name}" for name, typ in f.params]
        if mode == "forward":
            return f"std::vector<T> {f.name}(" + ", ".join([f"std::vector<T> {f.input}"] + fixed) + ")"
        if mode == "transpose":
            return f"std::vector<T> {f.name}_transpose(" + ", ".join(["const std::vector<T>& _lt_seed", "int _lt_input_size"] + fixed) + ")"
        if mode == "shape":
            return f"int {f.name}_output_size(" + ", ".join(["[[maybe_unused]] int _lt_input_size"] + fixed) + ")"
        return f"bool {f.name}_check(" + ", ".join(["int _lt_input_size"] + fixed + ["int _lt_trials = 8"]) + ")"

    def declarations(self, scope, lines, indent):
        for kind, name, value in scope.declarations:
            if kind == "fixed":
                lines.append(indent + f"[[maybe_unused]] const auto {name} = {value};")
            elif kind == "scalar":
                lines.append(indent + f"T {name} = T(0);")
            else:
                length = self.temp()
                lines.append(indent + f"const int {length} = {value};")
                lines.append(indent + f"assert({length} >= 0);")
                lines.append(indent + f"std::vector<T> {name}({length});")

    def emit_scope(self, scope, transpose, lines, indent):
        self.declarations(scope, lines, indent)
        self.emit_steps(scope, transpose, lines, indent)

    def emit_steps(self, scope, transpose, lines, indent):
        # Fixed assertions are preconditions for this scope in either direction.
        for step in scope.steps:
            if step[0] == "assert":
                lines.append(indent + f"assert({step[1]});")
        for step in reversed(scope.steps) if transpose else scope.steps:
            kind = step[0]
            def out(s): lines.append(indent + s)
            if kind in {"set", "add"}:
                _, dst, terms = step
                if transpose:
                    tmp = self.temp()
                    out(f"const T {tmp} = {dst};")
                    if kind == "set": out(f"{dst} = T(0);")
                    for c, ref in terms:
                        out(f"{ref} += {tmp} * ({c});")
                else:
                    rhs = " + ".join(f"({ref}) * ({c})" for c, ref in terms) or "T(0)"
                    out(f"{dst} {'=' if kind == 'set' else '+='} {rhs};")
            elif kind == "scale":
                out(f"{step[1]} *= {step[2]};")
            elif kind == "assert":
                continue
            elif kind == "call":
                _, dst, (name, src, args, _) = step
                if not transpose:
                    names = {"copy": "", "slice": "linear_transpose::slice", "resize": "linear_transpose::resized", "reverse": "linear_transpose::reversed", "ntt": "linear_transpose::ntt_forward", "intt": "linear_transpose::intt_forward", "convolution": "convolution", "poly_mod": "linear_transpose::polynomial_mod"}
                    fn = names.get(name, name + "<T>")
                    expression = f"{fn}({', '.join([src] + args)})" if fn else src
                    out(f"{dst} = {expression};")
                else:
                    if name in {"copy", "slice", "resize"}:
                        i = self.temp()
                        if name == "slice":
                            out(f"assert(0 <= {args[0]} && {args[0]} <= {args[1]} && {args[1]} <= int({src}.size()));")
                            out(f"for (int {i} = 0; {i} < int({dst}.size()); ++{i}) {src}[({args[0]}) + {i}] += {dst}[{i}];")
                        else:
                            out(f"for (int {i} = 0; {i} < int(std::min({src}.size(), {dst}.size())); ++{i}) {src}[{i}] += {dst}[{i}];")
                    else:
                        if name == "reverse": expression = f"linear_transpose::reversed({dst})"
                        elif name in {"ntt", "intt"}: expression = f"linear_transpose::{name}_transpose({dst})"
                        elif name in {"convolution", "poly_mod"}:
                            primitive = "convolution" if name == "convolution" else "polynomial_mod"
                            expression = f"linear_transpose::{primitive}_transpose({dst}, {args[0]}, int({src}.size()))"
                        else:
                            expression = f"{name}_transpose<T>({', '.join([dst, f'int({src}.size())'] + args)})"
                        out(f"linear_transpose::add_to({src}, {expression});")
                    out(f"std::fill({dst}.begin(), {dst}.end(), T(0));")
            elif kind == "if":
                out(f"if ({step[1]}) {{")
                self.emit_scope(step[2], transpose, lines, indent + "  ")
                if step[3].steps or step[3].declarations:
                    out("} else {")
                    self.emit_scope(step[3], transpose, lines, indent + "  ")
                out("}")
            elif kind == "range":
                _, var, args, inner = step
                lo, hi, delta, count, pos = [self.temp() for _ in range(5)]
                out("{")
                out(f"  const long long {lo} = {args[0]}, {hi} = {args[1]}, {delta} = {args[2]};")
                out(f"  assert({delta} != 0);")
                out(f"  const long long {count} = {delta} > 0 ? ({lo} < {hi} ? ({hi} - {lo} - 1) / {delta} + 1 : 0) : ({lo} > {hi} ? ({lo} - {hi} - 1) / (-{delta}) + 1 : 0);")
                if transpose:
                    out(f"  for (long long {pos} = {count}; {pos}-- > 0;) {{")
                else:
                    out(f"  for (long long {pos} = 0; {pos} < {count}; ++{pos}) {{")
                out(f"    [[maybe_unused]] const long long {var} = {lo} + {pos} * {delta};")
                self.emit_scope(inner, transpose, lines, indent + "    ")
                out("  }")
                out("}")
            elif kind == "each":
                _, var, array, inner = step
                it, seq = self.temp(), self.temp()
                out("{")
                out(f"  const auto& {seq} = {array};")
                begin, end = ("rbegin", "rend") if transpose else ("begin", "end")
                out(f"  for (auto {it} = {seq}.{begin}(); {it} != {seq}.{end}(); ++{it}) {{")
                out(f"    [[maybe_unused]] const auto& {var} = *{it};")
                self.emit_scope(inner, transpose, lines, indent + "    ")
                out("  }")
                out("}")

    def generate(self):
        lines = ['#pragma once', '', '// Generated by tools/transpose.py. Edit the .lin.py source to regenerate.', '#include "math/linalg/linear_transpose.hpp"', '']
        for f in self.functions.values():
            for mode in ["forward", "transpose", "shape"]:
                lines.extend(["template <class T>", self.signature(f, mode) + ";"])
        for f in self.functions.values():
            env = {f.input: "vec", **{name: "fixed" for name, _ in f.params}}
            shape = self.fixed(f.shape, env, {f.input: "_lt_input_size"})
            lines.extend(["", "template <class T>", self.signature(f, "shape"), "{", f"  return {shape};", "}"])
            fixed_args = [name for name, _ in f.params]
            shape_call = f"{f.name}_output_size<T>({', '.join(['_lt_input_size'] + fixed_args)})"
            for transpose in [False, True]:
                lines.extend(["", "template <class T>", self.signature(f, "transpose" if transpose else "forward"), "{"])
                if transpose:
                    lines.append("  assert(_lt_input_size >= 0);")
                    lines.append(f"  std::vector<T> {f.input}(_lt_input_size);")
                    lines.append(f"  assert(int(_lt_seed.size()) == {shape_call});")
                else:
                    lines.append(f"  [[maybe_unused]] const int _lt_input_size = {f.input}.size();")
                self.declarations(f.scope, lines, "  ")
                if transpose:
                    lines.append(f"  linear_transpose::add_to({f.output}, _lt_seed);")
                self.emit_steps(f.scope, transpose, lines, "  ")
                if not transpose:
                    lines.append(f"  assert(int({f.output}.size()) == {shape_call});")
                lines.extend([f"  return {f.input if transpose else f.output};", "}"])
            lines.extend(["", "template <class T>", self.signature(f, "check"), "{"])
            forward_args = ", ".join(["_lt_x"] + fixed_args)
            transpose_args = ", ".join(["_lt_y", "_lt_input_size"] + fixed_args)
            lines.extend([
                f"  return linear_transpose::check<T>(_lt_input_size, {shape_call},",
                f"    [&](const std::vector<T>& _lt_x) {{ return {f.name}<T>({forward_args}); }},",
                f"    [&](const std::vector<T>& _lt_y) {{ return {f.name}_transpose<T>({transpose_args}); }}, _lt_trials);",
                "}",
            ])
        return "\n".join(lines) + "\n"


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path, help="restricted Python source (.lin.py)")
    parser.add_argument("-o", "--output", type=Path, help="generated C++ header; default: stdout")
    parser.add_argument("--check", action="store_true", help="check that -o is already up to date")
    args = parser.parse_args(argv)
    if args.check and not args.output:
        parser.error("--check requires -o")
    try:
        generated = Generator(args.source.read_text()).generate()
        if args.check:
            if not args.output.exists() or args.output.read_text() != generated:
                print(f"{args.output}: regenerate with tools/transpose.py", file=sys.stderr)
                return 1
        elif args.output:
            args.output.write_text(generated)
        else:
            print(generated, end="")
    except (Error, SyntaxError, OSError) as e:
        print(f"{args.source}: {e}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())
