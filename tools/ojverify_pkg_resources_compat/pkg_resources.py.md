---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: py
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "Traceback (most recent call last):\n  File \"/opt/hostedtoolcache/Python/3.11.15/x64/lib/python3.11/site-packages/onlinejudge_verify/documentation/build.py\"\
    , line 71, in _render_source_code_stat\n    bundled_code = language.bundle(stat.path,\
    \ basedir=basedir, options={'include_paths': [basedir]}).decode()\n          \
    \         ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\
    \  File \"/opt/hostedtoolcache/Python/3.11.15/x64/lib/python3.11/site-packages/onlinejudge_verify/languages/python.py\"\
    , line 96, in bundle\n    raise NotImplementedError\nNotImplementedError\n"
  code: "\"\"\"Compatibility subset required by online-judge-verify-helper 5.6.0.\"\
    \"\"\n\nfrom importlib.resources import files\n\n\ndef resource_string(package:\
    \ str, resource: str) -> bytes:\n    return files(package).joinpath(resource).read_bytes()\n"
  dependsOn: []
  isVerificationFile: false
  path: tools/ojverify_pkg_resources_compat/pkg_resources.py
  requiredBy: []
  timestamp: '1970-01-01 00:00:00+00:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: tools/ojverify_pkg_resources_compat/pkg_resources.py
layout: document
redirect_from:
- /library/tools/ojverify_pkg_resources_compat/pkg_resources.py
- /library/tools/ojverify_pkg_resources_compat/pkg_resources.py.html
title: tools/ojverify_pkg_resources_compat/pkg_resources.py
---
