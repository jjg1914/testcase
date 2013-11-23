#include <cmath>

#include "testcase/Assert.h"

using namespace std;
using namespace testcase;

int Assert::num_asserts_val = 0;

int Assert::num_asserts()
{
  return num_asserts_val;
}

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
  ++Assert::num_asserts_val;
  throw Assert::Error(TestInfo::failed(message, filename, lineno));
}

void Assert::assert_true(bool cond, const string &expr,
  const string &filename, int lineno)
{
  ++Assert::num_asserts_val;
  if (!cond) {
    stringstream ss;
    ss << "expected true (" << expr << ")";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

void Assert::assert_false(bool cond, const string &expr,
  const string &filename, int lineno)
{
  ++Assert::num_asserts_val;
  if (cond) {
    stringstream ss;
    ss << "expected false (" << expr << ")";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

void Assert::assert_match(const string &expected, const string &actual,
  const string &filename, int lineno)
{
  ++Assert::num_asserts_val;
  if (!regex_match(actual, regex(expected))) {
    stringstream ss;
    ss << "expected \"" << actual << "\" to match /" << expected << "/";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

void Assert::assert_not_match(const string &expected, const string &actual,
  const string &filename, int lineno)
{
  ++Assert::num_asserts_val;
  if (regex_match(actual, regex(expected))) {
    stringstream ss;
    ss << "expected \"" << actual << "\" to not match /" << expected << "/";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

void Assert::assert_no_exception(const std::function<void()> &f,
  const string &filename, int lineno)
{
  ++Assert::num_asserts_val;
  stringstream ss;
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

void Assert::assert_float_equal(float expected, float actual, float diff,
  const string &filename, int lineno)
{
  ++Assert::num_asserts_val;
  if (fabs(expected - actual) > diff) {
    stringstream ss;
    ss << "expected " << actual << " to be equal to " << expected << ", within " << diff;
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

void Assert::assert_float_equal(double expected, double actual, double diff,
  const string &filename, int lineno)
{
  ++Assert::num_asserts_val;
  if (fabs(expected - actual) > diff) {
    stringstream ss;
    ss << "expected " << actual << " to be equal to " << expected << ", within " << diff;
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

void Assert::assert_float_not_equal(double expected, double actual, double diff,
  const string &filename, int lineno)
{
  ++Assert::num_asserts_val;
  if (fabs(expected - actual) <= diff) {
    stringstream ss;
    ss << "expected " << actual << " to not be equal to " << expected << ", within " << diff;
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}
