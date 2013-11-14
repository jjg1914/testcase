#include "Assert.h"

using namespace std;

Assert::Error::Error(const TestInfo &info_val) noexcept
  : info_val(info_val)
{}

TestInfo Assert::Error::info() const noexcept
{
  return info_val;
}

void Assert::assert_fail(const string message, const string &filename,
  int lineno)
{
  throw Assert::Error(TestInfo::failed(message, filename, lineno));
}

void Assert::assert_true(bool cond, const string &expr,
  const string &filename, int lineno)
{
  if (!cond) {
    stringstream ss;
    ss << "expected true (" << expr << ")";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

void Assert::assert_false(bool cond, const string &expr,
  const string &filename, int lineno)
{
  if (cond) {
    stringstream ss;
    ss << "expected false (" << expr << ")";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
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

void Assert::assert_not_match(const string &expected, const string &actual,
  const std::string &filename, int lineno)
{
  if (regex_match(actual, regex(expected))) {
    stringstream ss;
    ss << "expected \"" << actual << "\" to not match /" << expected << "/";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

void Assert::assert_no_exception(const std::function<void()> &f,
  const std::string &filename, int lineno)
{
  std::stringstream ss;
  try {
    f();
    return;
  } catch (std::exception &e) {
    ss << "expected no exception, caught \"" << demangle(typeid(e))<< "\"";
  } catch(...) {
    ss << "expected no exception, caught unknown";
  }
  throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
}
