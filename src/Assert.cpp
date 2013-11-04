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
  throw Assert::Error(TestInfo().filename(filename).lineno(lineno));
}

void Assert::assert_true(bool cond, const string &,
  const string &filename, int lineno)
{
  if (!cond) {
    throw Assert::Error(TestInfo().filename(filename).lineno(lineno));
  }
}

void Assert::assert_match(const regex &expected, const string &actual,
  const std::string &filename, int lineno)
{
  if (!regex_match(actual, expected)) {
    throw Assert::Error(TestInfo().filename(filename).lineno(lineno));
  }
}
