#pragma once

/*! SPDX-License-Identifier: CC0-1.0 */

#include "sequence.hpp"

/**
 * @brief 挿入・削除・反転・区間作用・区間積ができる列
 * @docs docs/ds/bbst/lazy_sequence.md
 */

template <class AM> using LazySequenceTree = bbst_detail::SequenceTreeImpl<AM, true>;
