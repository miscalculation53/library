#pragma once

template <class T> struct Zarts
{
private:
  vector<T> dat;
  int base;

public:
  vector<T> vals;
  vector<int> cnt;
  Zarts() {}
  Zarts(const vector<T> &vec, const int &min_id = 0)
  {
    base = min_id;
    dat = vector<T>(vec);
    vector<T> vec2 = vector<T>(vec);
    sort(vec2.begin(), vec2.end());
    for (auto x : vec2)
    {
      if (vals.empty() || vals.back() != x)
      {
        vals.emplace_back(x);
        cnt.emplace_back(1);
      }
      else
        cnt.back()++;
    }
  }
  int get_id(const T &val) const
  {
    auto it = lower_bound(vals.begin(), vals.end(), val);
    if (*it != val)
      return -1;
    return it - vals.begin() + base;
  }
  vector<T> get_vec() const
  {
    vector<T> res(dat.size());
    for (int i = 0; i < (int)dat.size(); i++)
      res[i] = get_id(dat[i]);
    return res;
  }
};