# ツール

## `oj-verify` の互換モジュール

`online-judge-verify-helper==5.6.0` は、現在の `setuptools` から削除された
`pkg_resources.resource_string()` をドキュメント生成に使用しています。
`requirements.txt` は安全な最新版の `setuptools` を使いつつ `oj-verify` を保つため、
同じ処理を `importlib.resources` で行う最小互換モジュールを
`ojverify_pkg_resources_compat` からインストールします。

## GitHub Actions の verify 並列化

`.github/workflows/verify.yml` は、verify ファイルを10個の shard に分けて並列に
実行します。通常の8 shard には secret を渡さず、yukicoder 用の2 shard にだけ
`YUKICODER_TOKEN` を渡します。各 shard は担当したファイルがすべて検証済みで
あることを確認してから、timestamp と担当ファイル一覧を artifact に保存します。

`main` への push では、全 shard の artifact が全 verify ファイルを重複なく覆う
ことを `parallel_verify.py` で確認してから timestamp を統合します。その後にだけ
`GITHUB_TOKEN` で timestamp を main へ反映し、`GH_PAT` で従来の `gh-pages`
ブランチへドキュメントを公開します。pull request ではどちらの書き込み用 token も
使用しません。main を更新するジョブとドキュメントを公開するジョブも分け、前者に
`GH_PAT` は渡さず、後者の `GITHUB_TOKEN` は読み取り権限だけにしています。
`DROPBOX_TOKEN` は使用しません。

`parallel_verify.py` は workflow 内部用です。shard の選択はディレクトリ内の
verify ファイルを名前順に並べ、番号を shard 数で割った余りによって行います。
`oj-verify` が35分で全ファイルを処理できなかった場合、その shard は失敗ではなく
警告として終了し、未処理ファイルを Job Summary に表示します。GitHub Pages でも
未処理の verify ファイルは `⚠️`、それだけに依存するライブラリは `❓` と表示し、
実際に検証が失敗したものだけを `❌` として扱います。処理済みの timestamp と、
未処理ファイルに以前の timestamp があればそれを保持します。ジョブ自体が60分以内に
終了できない場合は GitHub Actions の強制タイムアウトとなるため、通常の失敗表示です。

GitHub の `ubuntu-latest` に `g++-15` がない場合は、標準搭載されている
`g++-14` を CI 内だけ `g++-15` という名前でも参照できるようにします。これにより、
手元の `.verify-helper/config.toml` を CI 用に書き換えずに利用できます。

`submit_code.py` は、コンテスト中に使う `main.cpp` にはライブラリをすべて
読み込める状態を保ち、提出用に生成する `bundle.cpp` だけを短くするツールです。

## コンテストディレクトリから使う

`main.cpp` があるディレクトリへ移動して、次のように実行します。

```sh
python3 ~/Documents/02_kyopro/02_solve/library/tools/submit_code.py \
  expand main.cpp -o bundle.cpp
```

カレントディレクトリは任意です。入力ファイルと出力ファイルは、カレント
ディレクトリからの相対パスまたは絶対パスで指定できます。

## 短縮コマンド

`aliases.zsh` を `.zshrc` から読み込むと、コンテストディレクトリでは次の
コマンドだけで実行できます。

```sh
source "$HOME/Documents/02_kyopro/02_solve/library/tools/aliases.zsh"
```

| コマンド | 処理 |
| --- | --- |
| `oj-b` | 従来と同様に `main.cpp` を高速に展開して `bundle.cpp` に出力する |
| `oj-bc` | `oj-b` の後に `cln` を続けて実行する |
| `oj-bs` | 展開後、安全確認付きで未使用ヘッダとコメントなどを除く |
| `oj-ba` | 展開後、未使用ヘッダに加えて未使用宣言も可能な範囲で除く |
| `cln` | 65,536 B 未満の中間結果を出し、その後も短縮を続ける |
| `cln-all` | サイズ制限用の中間結果を作らず、最終短縮だけを行う |
| `oj-b0` | `oj-b` の旧名として残している同じ処理 |

`oj-b` は通常のローカル `#include` を Python 内で直接たどります。ヘッダごとに
GNU C++ コンパイラを起動しないため、従来の `oj-bundle` より高速です。条件分岐内の
ローカル include など、高速展開器が扱わない書き方を検出した場合だけ、
`CXX=g++-15` 相当の設定で `oj-bundle` へフォールバックします。どちらの場合も
ローカルパスを含む `#line` は出力しません。

通常は次の流れで使います。

```sh
cd ~/Documents/02_kyopro/02_solve/01_contests/18
oj-b
cln
```

上の2コマンドを続けて行う場合は、次の1コマンドで実行できます。

```sh
oj-bc
```

コード長に余裕があれば `oj-b` だけで構いません。`cln` は 65,536 B 未満の
検証済み候補ができた時点で `bundle.cpp` へ書き出し、その後も未使用宣言の削除を
続けます。`oj-bs` と `oj-ba` は候補を削除するたびにコンパイルとリンクを行うため、
通常の `oj-b` よりかなり時間がかかります。

ファイル名を変える場合は、入力、出力の順で指定できます。

```sh
oj-b a.cpp a-bundle.cpp
oj-bc a.cpp a-bundle.cpp
cln a-bundle.cpp
cln-all a-bundle.cpp
```

`oj-c`、`oj-s` などが内部で `oj-b` を呼ぶ既存の設定も、そのまま新しい
`oj-b` を使用します。

## 高速な展開

```sh
python3 tools/submit_code.py expand main.cpp -o bundle.cpp
```

`#pragma once` または一般的な include guard の付いたローカルヘッダを読み込み、
重複を除きながら展開します。対応していない include の形では自動的に
`oj-bundle` を使います。フォールバックを禁止して検査したい場合は
`--no-fallback` を指定します。

## 安全確認付きの展開

```sh
python3 tools/submit_code.py bundle main.cpp -o bundle.cpp
```

次の処理を順に行います。

1. リポジトリのインクルードパスを指定して `oj-bundle` を実行する。
2. ローカルパスを含むすべての `#line` を `bundle/...` という仮想パスに置き換える。
3. 展開されたヘッダを単位として、削除後もコンパイルとリンクが通る未使用部分を除く。
4. コメントと無効なプリプロセッサ分岐を除き、連続する空行を1行にまとめる。
5. 最終出力から `#line` を除く。

短縮が受理されるたびに、コンパイルとリンクが通る状態をアトミックに
`bundle.cpp` へ書き出します。途中で停止しても、最後に確認できた状態が残ります。

主なオプションは次の通りです。

```sh
# 個人情報を含まない仮想パスに直した #line を残す。
python3 tools/submit_code.py bundle main.cpp --keep-line

# 宣言単位の未使用コード削除も行う。
python3 tools/submit_code.py bundle main.cpp --aggressive

# 先の削除で依存関係が変わった場合を考慮し、ヘッダ削除を再試行する。
python3 tools/submit_code.py bundle main.cpp --header-passes 2

# インクルードパスやコンパイラオプションを追加する。
python3 tools/submit_code.py bundle main.cpp -I path/to/library \
  --cxx-arg=-DMOD=998244353
```

`g++-15`、`g++-14`、`oj-bundle`、リポジトリ内の
`.venv/bin/oj-bundle` は自動的に探索します。明示的に指定する場合は
`--cxx` または `--oj-bundle` を使います。

検証時には `<bits/stdc++.h>` のプリコンパイル済みヘッダを再利用します。
`<bits/stdc++.h>` より前に定義した設定用マクロもキャッシュへ含めるため、通常の
テンプレートをそのまま展開した `bundle.cpp` でも利用できます。初めて現れた
設定ではキャッシュ生成に時間がかかりますが、同じ設定の次回以降は再利用します。
キャッシュ先は `$SUBMIT_CODE_CACHE_DIR`、未指定の場合はシステムの一時
ディレクトリです。コンパイラ、C++ 規格、インクルードパス、コンパイラ
オプションが異なる場合は別のキャッシュを使用します。

## 既存ファイルの短縮

`cleanup` は、すでにある `bundle.cpp` をその場で短くします。既定では
65,536 B 未満になった最初の候補をすぐ書き出し、その後も短縮を続けます。

```sh
python3 tools/submit_code.py cleanup bundle.cpp
```

C++23 を既定値とし、次の順に処理します。

1. 既に 65,536 B 未満なら、そのファイルを提出可能な中間結果として維持する。
2. インデントと行内の空白を保ったままコメントを除き、連続する空行を1行にまとめて構文を確認する。
3. 65,536 B 未満なら、その候補をアトミックに書き出す。
4. 無効な分岐の除去、無名名前空間化、未使用宣言の削除を続ける。
5. コンパイラに加えて `cppcheck` も使い、既定では最大3周解析する。
6. 最後にGCCのIPA解析を行い、外部から到達しない関数群を削除する。

中間結果を書き出した後も処理は続きますが、ディスク上の `bundle.cpp` が再び
65,536 B 以上の候補で上書きされることはありません。サイズ制限用の中間結果を
作らず、最終的な短縮だけを行うには、`cln-all` または次を使います。

```sh
python3 tools/submit_code.py cleanup bundle.cpp --target-bytes 0
```

別の制限を指定する場合、たとえば 100,000 B 未満なら次のようにします。

```sh
python3 tools/submit_code.py cleanup bundle.cpp --target-bytes 100000
```

コメントなどだけを高速に除き、宣言単位の解析を行わない場合は次を使います。

```sh
python3 tools/submit_code.py cleanup bundle.cpp --safe-only
```

どちらの処理も、解析、検証、書き出しの各段階と所要時間、変更前後のバイト数を
表示します。端末では段階ごとに自動で色が付き、バイト数は太字で強調されます。
色を明示的に無効化するには
`--color never`、端末以外への出力でも有効化するには `--color always` を指定します。
削除したヘッダや宣言を表示するには `-v` を指定します。

直接 `#include` したローカルヘッダは常に残します。推移的に読み込まれた
ヘッダと未使用宣言は、削除後にコンパイルとリンクが通った場合だけ除きます。
グローバルな副作用があるため残したいヘッダは、たとえば
`--keep-header 'bundle/include-0/path/to/header.hpp'` のように指定できます。
