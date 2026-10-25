#pragma once

#include "lazy_segtree.hpp"

/**
 * @brief Segment Tree Beats
 * @docs docs/ds/segtree/segtree_beats.md
 */

template <class AM>
using SegmentTreeBeats = LazySegmentTree<AM, true>;
