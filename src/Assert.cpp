#include "Assert.h"

using namespace std;

Assert::Error::Error(const TestInfo &info_val) noexcept
  : info_val(info_val)
{}

TestInfo Assert::Error::info() const noexcept
{
  return info_val;
}

void Assert::assert_fail(const string &filename, int lineno)
{
  throw Assert::Error(TestInfo::failed("failed", filename, lineno));
}

void Assert::assert_true(bool cond, const string &expr,
  const string &filename, int lineno)
{
  if (!cond) {
    throw Assert::Error(TestInfo::failed(expr, filename, lineno));
  }
}

void Assert::assert_match(const string &expected, const string &actual,
  const std::string &filename, int lineno)
{
  if (!regex_match(actual, regex(expected))) {
    stringstream ss;
    ss << "expected \"" << actual << "\" to match /" << expected << "/";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}
