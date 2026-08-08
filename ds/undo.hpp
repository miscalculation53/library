#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief undo 可能な vector など
 * @docs docs/ds/undo.md
 */

// 保持できる snapshot は 1 個まで
template <class T>
struct ValRollback
{
private:
  T val;
  T snap_val;

public:
  ValRollback() : val{}, snap_val{} {}
  ValRollback(T x) : val(x), snap_val(x) {}
  inline T get() const { return val; }
  inline void set(T x) { val = x; }
  inline void snapshot() { snap_val = val; }
  inline void rollback() { val = snap_val; }
};

template <class T>
struct ValUndo
{
private:
  vc<T> his;
  int snap_time;

public:
  ValUndo() : his(1), snap_time(1) {}
  ValUndo(T x) : his(1, x), snap_time(1) {}
  inline T get() const { return his.back(); }
  inline void set(T x) { his.eb(x); }
  inline void undo(int times = 1)
  {
    assert(0 <= times && times < SZ(his));
    his.resize(SZ(his) - times);
  }
  inline void reset() { his.resize(1), snap_time = 1; }
  inline void snapshot() { snap_time = his.size(); }
  inline void rollback() { undo(SZ(his) - snap_time); }
};

template <class T>
struct VectorUndo
{
private:
  vc<T> vec;
  vc<pair<int, T>> his;
  int snap_time;

public:
  VectorUndo() : snap_time(0) {}
  VectorUndo(int n) : vec(n), snap_time(0) {}
  template <class Iter>
  VectorUndo(const Iter &bg, const Iter &ed) : vec(bg, ed), snap_time(0) {}
  VectorUndo(const vc<T> &vec) : vec(vec), snap_time(0) {}

  T get(int i) const
  {
    assert(0 <= i && i < SZ(vec));
    return vec[i];
  }
  void set(int i, T val)
  {
    assert(0 <= i && i < SZ(vec));
    his.eb(i, vec[i]);
    vec[i] = val;
  }

  void undo(int times = 1)
  {
    repi(_, times)
    {
      assert(!his.empty());
      cauto & [ i, val ] = his.back();
      vec[i] = val;
      his.pop_back();
    }
  }
  void reset()
  {
    undo(SZ(his));
    snap_time = 0;
  }
  void snapshot() { snap_time = his.size(); }
  void rollback() { undo(SZ(his) - snap_time); }

  vc<T> content() { return vec; }
};

#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(get());
#endif
