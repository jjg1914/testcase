#include "Assert.h"

using namespace std;

Assert::Error::Error(const TestInfo &info_val) noexcept
  : info_val(info_val)
{}

TestInfo Assert::Error::info() const noexcept
{
  return info_val;
}

void Assert::assert_fail(const std::string &filename, int lineno)
{
  throw Assert::Error(TestInfo().filename(filename).lineno(lineno));
}
