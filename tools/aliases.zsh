typeset -g KYOPRO_SUBMIT_CODE_TOOL="$HOME/Documents/02_kyopro/02_solve/library/tools/submit_code.py"

unalias oj-b oj-bc oj-bs oj-ba oj-b0 cln cln-all 2>/dev/null

# 通常のローカル include は直接たどり、未対応の場合だけ oj-bundle を使う。
oj-b() {
  command python3 "$KYOPRO_SUBMIT_CODE_TOOL" expand \
    "${1:-main.cpp}" -o "${2:-bundle.cpp}"
}

# main.cpp を高速に展開してから、bundle.cpp の未使用宣言を短縮する。
oj-bc() {
  local source_file="${1:-main.cpp}"
  local output_file="${2:-bundle.cpp}"
  oj-b "$source_file" "$output_file" && cln "$output_file"
}

# main.cpp を展開し、安全確認付きで未使用ヘッダなどを短縮する。
oj-bs() {
  command python3 "$KYOPRO_SUBMIT_CODE_TOOL" bundle "${1:-main.cpp}" -o "${2:-bundle.cpp}"
}

# main.cpp を展開し、宣言単位の短縮も行って bundle.cpp に出力する。
oj-ba() {
  command python3 "$KYOPRO_SUBMIT_CODE_TOOL" bundle "${1:-main.cpp}" -o "${2:-bundle.cpp}" --aggressive
}

# 既存の bundle.cpp に宣言単位の短縮を追加で行う。
cln() {
  command python3 "$KYOPRO_SUBMIT_CODE_TOOL" cleanup "${1:-bundle.cpp}"
}

# サイズ制限用の中間チェックポイントを作らず、最終短縮だけを行う。
cln-all() {
  command python3 "$KYOPRO_SUBMIT_CODE_TOOL" cleanup \
    "${1:-bundle.cpp}" --target-bytes 0
}

# oj-b の旧名。既存の呼び出しとの互換性のため残す。
oj-b0() {
  oj-b "$@"
}
