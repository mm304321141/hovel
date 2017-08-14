
#include <cassert>
#include <ctime>
#include <vector>
#include <algorithm>
#include <utility>
#include <iostream>

template<class T>
struct finder_t
{
  struct pos_info
  {
    std::vector<T> const *arr;
    size_t p[3];
  };
  std::vector<pos_info> pos;

  T find_kth(std::vector<std::vector<T>> const &arrs, size_t k)
  {
    pos.clear();
    pos.reserve(arrs.size());
    for (auto &arr : arrs)
    {
      pos.emplace_back(pos_info{&arr, 0, std::min(k + 1, arr.size()), 0});
    }
    size_t b = 0, e = 1, m = 2;
    size_t find = k;
    while (true)
    {
      size_t max_i = size_t(-1), max_size = 0;
      size_t new_end = pos.size();
      for (size_t i = 0; i < new_end; )
      {
        auto &p = pos[i];
        size_t size = p.p[e] - p.p[b];
        if (size == 0)
        {
          std::swap(p, pos[--new_end]);
        }
        else
        {
          if (size > max_size)
          {
            max_i = i;
            max_size = size;
          }
          ++i;
        }
      }
      pos.resize(new_end);
      if (max_size == 1)
      {
        break;
      }
      if (max_i > 0)
      {
        std::swap(pos[max_i], pos[0]);
      }
      auto p = &pos[0];
      p->p[m] = (p->p[b] + p->p[e]) / 2;
      T const &t_mid = (*p->arr)[p->p[m]];
      size_t mid_size = p->p[m] - p->p[b];
      for (size_t i = 1; i < pos.size(); ++i)
      {
        p = &pos[i];
        p->p[m] = std::lower_bound(p->arr->begin() + p->p[b], p->arr->begin() + p->p[e], t_mid) - p->arr->begin();
        mid_size += p->p[m] - p->p[b];
      }
      if (mid_size <= find)
      {
        std::swap(m, b);
        find -= mid_size;
      }
      else
      {
        std::swap(m, e);
      }
    }
    std::nth_element(pos.begin(), pos.begin() + find, pos.end(), [b](pos_info const &x, pos_info const &y)
    {
      return (*x.arr)[x.p[b]] < (*y.arr)[y.p[b]];
    });
    auto &p = pos[find];
    return (*p.arr)[p.p[b]];
  }
};


void init(size_t k, size_t num, std::vector<std::vector<int>> &data, std::vector<int> &verify) {
  srand((uint32_t)time(NULL));
  data.resize(num);
  verify.clear();
  for (int i = 0; i < num; ++i)
  {
    data[i].clear();
    int m = rand() % k;
    for (int j = 0; j < m; ++j)
    {
      int v = rand() % (k * num);
      data[i].push_back(v);
      verify.push_back(v);
    }
    std::sort(data[i].begin(), data[i].end());
  }
  std::sort(verify.begin(), verify.end());
}

int main()
{
  std::vector<std::vector<int>> data;
  std::vector<int> verify;
  finder_t<int> finder;
  while (true)
  {
    init(1000, 10, data, verify);
    for (size_t i = 0; i < verify.size(); ++i)
    {
      if (finder.find_kth(data, i) != verify[i])
      {
        std::cout << "we got an error ..." << std::endl;
      }
    }
  }
  system("pause");
}
