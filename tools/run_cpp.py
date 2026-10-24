#!/usr/bin/env python3
"""Compile a local C++ program with its source hash, then open its executable."""
from __future__ import annotations

import argparse
import hashlib
from pathlib import Path
import subprocess
import sys

import template_pch


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source")
    parser.add_argument("executable")
    parser.add_argument("--precompile-only", action="store_true")
    parser.add_argument("compiler", nargs=argparse.REMAINDER)
    args = parser.parse_args(argv)
    command = args.compiler
    if command[:1] == ["--"]:
        command = command[1:]
    if not command:
        parser.error("pass the compiler command after --")
    source = Path(args.source).expanduser().resolve()
    executable = Path(args.executable).expanduser().resolve()
    try:
        digest = hashlib.sha256(source.read_bytes()).hexdigest()
        banner = Path(__file__).with_name("run_source_hash.cpp").resolve()
        # Link the banner first: Main<> reads input during global initialization.
        # A separate translation unit also preserves the user's precompiled headers.
        with template_pch.prepare(source, command) as prepared:
            if args.precompile_only:
                if hashlib.sha256(source.read_bytes()).hexdigest() != digest or not prepared.unchanged():
                    print("プリコンパイル中に入力が変更されました。もう一度実行してください。", file=sys.stderr)
                    return 1
                return 0
            compile_command = prepared.command
            result = subprocess.run([
                compile_command[0], str(banner),
                *(arg for arg in compile_command[1:] if arg != "-lquadmath"),
                f'-DKYOPRO_RUN_SOURCE_HASH="{digest}"', "-lquadmath",
            ], check=False)
            if result.returncode:
                return result.returncode
            if hashlib.sha256(source.read_bytes()).hexdigest() != digest:
                print("コンパイル中に元コードが変更されました。もう一度実行してください。", file=sys.stderr)
                return 1
            if not prepared.unchanged():
                print("コンパイル中にヘッダが変更されました。もう一度実行してください。", file=sys.stderr)
                return 1
            return subprocess.run(["open", str(executable)], check=False).returncode
    except KeyboardInterrupt:
        return 130
    except (OSError, template_pch.PrecompileError) as exc:
        print(f"[run] {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
