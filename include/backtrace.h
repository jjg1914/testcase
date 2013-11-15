#ifndef H_BACKTRACE
#define H_BACKTRACE

#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>

std::string sbacktrace(int bottom_offset = 0, int top_offset = 0);

std::string demangle(const std::type_info &info);

template<typename A, typename B>
std::string sdiff(const std::vector<int> &d, A a_start, B b_start);

template<typename A, typename B>
std::vector<int> diff(A a_start, A a_end, B b_start, B b_end);

template<typename A, typename B>
std::vector<std::vector<int>> lcs(A a_start, A a_end, B b_start, B b_end);

template<typename A, typename B>
std::string sdiff(const std::vector<int>& d, A a_start, B b_start)
{
  std::stringstream ss;
  for (int i = 0; i < d.size(); ++i) {
    if (d[i] < 0) {
      ss << "-" << *a_start << std::endl;
      ++a_start;
    } else if (d[i] > 0) {
      ss << "+" << *b_start << std::endl;
      ++b_start;
    } else {
      ss << " " << *a_start << std::endl;
      ++a_start, ++b_start;
    }
  }
  return ss.str();
}

template<typename A, typename B>
std::vector<int> diff(A a_start, A a_end, B b_start, B b_end)
{
  std::vector<int> rval;
  std::vector<std::vector<int>> mat = lcs(a_start,a_end,b_start,b_end);
  int i = 0, j = 0;
  int a_len(a_end - a_start), b_len(b_end - b_start);

  while (i < a_len || j < b_len) {
    if (i == a_len) {
      rval.emplace_back(1);
      ++b_start, ++j;
    } else if (j == b_len) {
      rval.emplace_back(-1);
      ++a_start, ++i;
    } else if (*a_start == *b_start) {
      rval.emplace_back(0);
      ++a_start, ++b_start, ++i, ++j;
    } else if (mat[i + 1][j] >= mat[i][j + 1]) {
      rval.emplace_back(-1);
      ++a_start, ++i;
    } else {
      rval.emplace_back(1);
      ++b_start, ++j;
    }
  }

  return rval;
}

template<typename A, typename B>
std::vector<std::vector<int>> lcs(A a_start, A a_end, B b_start, B b_end)
{
  int a_len(a_end - a_start), b_len(b_end - b_start);
  std::vector<std::vector<int>> rval;
  while(rval.size() < a_len) {
    rval.emplace_back();
    while(rval.back().size() < b_len) {
      rval.back().emplace_back(-1);
    }
  }

  std::function<int(int,int,A,B)> impl = [&](int i, int j, A a, B b)->int {
    if (a == a_end || b == b_end) {
      return 0;
    }
    if (rval[i][j] < 0) {
      if (*a == *b) {
        rval[i][j] = 1 + impl(i + 1,j + 1,a + 1,b + 1);
      } else {
        rval[i][j] = std::max(impl(i + 1,j,a + 1,b),impl(i,j + 1,a,b + 1));
      }
    }
    return rval[i][j];
  };
  impl(0,0,a_start,b_start);

  return rval;
}

#endif
