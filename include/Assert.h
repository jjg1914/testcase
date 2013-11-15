#ifndef H_ASSERT
#define H_ASSERT

#include <exception>
#include <string>
#include <regex>
#include <sstream>

#include "backtrace.h"
#include "TestInfo.h"

#define ASSERT_FAIL(message)\
  Assert::assert_fail(message,__FILE__,__LINE__)

#define ASSERT_TRUE(condition)\
  Assert::assert_true(condition,#condition,__FILE__,__LINE__)

#define ASSERT_FALSE(condition)\
  Assert::assert_false(condition,#condition,__FILE__,__LINE__)
  
#define ASSERT_SAME(expected,actual)\
  Assert::assert_same(expected,actual,__FILE__,__LINE__)

#define ASSERT_NOT_SAME(expected,actual)\
  Assert::assert_not_same(expected,actual,__FILE__,__LINE__)

#define ASSERT_EQUAL(expected,actual)\
  Assert::assert_equal(expected,actual,__FILE__,__LINE__)

#define ASSERT_NOT_EQUAL(expected,actual)\
  Assert::assert_not_equal(expected,actual,__FILE__,__LINE__)

#define ASSERT_LESS(expected,actual)\
  Assert::assert_less(expected,actual,__FILE__,__LINE__)

#define ASSERT_LESS_OR_EQUAL(expected,actual)\
  Assert::assert_less_or_equal(expected,actual,__FILE__,__LINE__)

#define ASSERT_GREATER(expected,actual)\
  Assert::assert_greater(expected,actual,__FILE__,__LINE__)

#define ASSERT_GREATER_OR_EQUAL(expected,actual)\
  Assert::assert_greater_or_equal(expected,actual,__FILE__,__LINE__)

#define ASSERT_MATCH(expected,actual)\
  Assert::assert_match(expected,actual,__FILE__,__LINE__)

#define ASSERT_NOT_MATCH(expected,actual)\
  Assert::assert_not_match(expected,actual,__FILE__,__LINE__)

#define ASSERT_INSTANCE_OF(expected,actual)\
  Assert::assert_instance_of<expected>(actual,__FILE__,__LINE__)

#define ASSERT_NOT_INSTANCE_OF(expected,actual)\
  Assert::assert_not_instance_of<expected>(actual,__FILE__,__LINE__)

#define ASSERT_EXCEPTION(expected,lambda)\
  Assert::assert_exception<expected>(lambda,__FILE__,__LINE__)

#define ASSERT_NO_EXCEPTION(lambda)\
  Assert::assert_no_exception(lambda,__FILE__,__LINE__)

#define ASSERT_ARRAY_EQUAL(expected,actual,size)\
  Assert::assert_array_equal(expected,actual,size,__FILE__,__LINE__)

#define ASSERT_ARRAY_NOT_EQUAL(expected,actual,size)\
  Assert::assert_array_not_equal(expected,actual,size,__FILE__,__LINE__)

#define ASSERT_CONTAINER_EQUAL(expected,actual)\
  Assert::assert_container_equal(expected,actual,__FILE__,__LINE__)

#define ASSERT_CONTAINER_NOT_EQUAL(expected,actual)\
  Assert::assert_container_not_equal(expected,actual,__FILE__,__LINE__)

#define ASSERT_CONTAINER_EMPTY(container)\
  Assert::assert_container_empty(container,__FILE__,__LINE__)

#define ASSERT_CONTAINER_NOT_EMPTY(container)\
  Assert::assert_container_not_empty(container,__FILE__,__LINE__)

#define ASSERT_CONTAINER_SIZE(expected,actual)\
  Assert::assert_container_size(expected,actual,__FILE__,__LINE__)

#define ASSERT_CONTAINER_NOT_SIZE(expected,actual)\
  Assert::assert_container_not_size(expected,actual,__FILE__,__LINE__)

class Assert {

  public:

  class Error : public std::exception {

    public:

    Error(const TestInfo &info_val) noexcept;

    TestInfo info() const noexcept;

    private:

    TestInfo info_val;
  };

  static void assert_fail(const std::string message,
    const std::string &filename, int lineno);

  static void assert_true(bool cond, const std::string &expr,
    const std::string &filename, int lineno);

  static void assert_false(bool cond, const std::string &expr,
    const std::string &filename, int lineno);

  template<typename A, typename B>
  static void assert_same(const A &expected, const B &actual,
    const std::string &filename, int lineno);

  template<typename A, typename B>
  static void assert_not_same(const A &expected, const B &actual,
    const std::string &filename, int lineno);

  template<typename A, typename B>
  static void assert_equal(const A &expected, const B &actual,
    const std::string &filename, int lineno);

  template<typename A, typename B>
  static void assert_not_equal(const A &expected, const B &actual,
    const std::string &filename, int lineno);

  template<typename A, typename B>
  static void assert_less(const A &expected, const B &actual,
    const std::string &filename, int lineno);

  template<typename A, typename B>
  static void assert_less_or_equal(const A &expected, const B &actual,
    const std::string &filename, int lineno);

  template<typename A, typename B>
  static void assert_greater(const A &expected, const B &actual,
    const std::string &filename, int lineno);

  template<typename A, typename B>
  static void assert_greater_or_equal(const A &expected, const B &actual,
    const std::string &filename, int lineno);

  static void assert_match(const std::string &expected,
    const std::string &actual, const std::string &filename, int lineno);

  static void assert_not_match(const std::string &expected,
    const std::string &actual, const std::string &filename, int lineno);

  template<typename Expected, typename Actual>
  static void assert_instance_of(const Actual &actual,
    const std::string &filename, int lineno);

  template<typename Expected, typename Actual>
  static void assert_not_instance_of(const Actual &actual,
    const std::string &filename, int lineno);

  template<typename Expected>
  static void assert_exception(const std::function<void()> &f,
    const std::string &filename, int lineno);

  static void assert_no_exception(const std::function<void()> &f,
    const std::string &filename, int lineno);

  template<typename Expected, typename Actual>
  static void assert_array_equal(const Expected *expected,
    const Actual *actual, int size, const std::string &filename, int lineno);

  template<typename Expected, typename Actual>
  static void assert_array_not_equal(const Expected *expected,
    const Actual *actual, int size, const std::string &filename, int lineno);

  template<typename Expected, typename Actual>
  static void assert_container_equal(const Expected &expected,
    const Actual &actual, const std::string &filename, int lineno);

  template<typename Expected, typename Actual>
  static void assert_container_not_equal(const Expected &expected,
    const Actual &actual, const std::string &filename, int lineno);

  template<typename T>
  static void assert_container_empty(const T &container,
    const std::string &filename, int lineno);

  template<typename T>
  static void assert_container_not_empty(const T &container,
    const std::string &filename, int lineno);

  template<typename T>
  static void assert_container_size(size_t expected, const T &actual,
    const std::string &filename, int lineno);

  template<typename T>
  static void assert_container_not_size(size_t expected, const T &actual,
    const std::string &filename, int lineno);

  static int num_asserts();

  private:

  static int num_asserts_val;
};

template<typename A, typename B>
void Assert::assert_same(const A &expected, const B &actual,
  const std::string &filename, int lineno)
{
  ++num_asserts_val;
  if (&expected != &actual) {
    std::stringstream ss;
    ss << "expected \"" << &actual;
    ss << "\" to be the same as \"" << &expected << "\"";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename A, typename B>
void Assert::assert_not_same(const A &expected, const B &actual,
  const std::string &filename, int lineno)
{
  ++num_asserts_val;
  if (&expected == &actual) {
    std::stringstream ss;
    ss << "expected \"" << &actual;
    ss << "\" to not be the same as \"" << &expected << "\"";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename A, typename B>
void Assert::assert_equal(const A &expected, const B &actual,
  const std::string &filename, int lineno)
{
  ++num_asserts_val;
  if (!(actual == expected)) {
    std::stringstream ss;
    ss << "expected \"" << actual;
    ss << "\" to be equal to \"" << expected << "\"";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename A, typename B>
void Assert::assert_not_equal(const A &expected, const B &actual,
  const std::string &filename, int lineno)
{
  ++num_asserts_val;
  if (!(actual != expected)) {
    std::stringstream ss;
    ss << "expected \"" << actual;
    ss << "\" to not be equal to \"" << expected << "\"";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename A, typename B>
void Assert::assert_less(const A &expected, const B &actual,
  const std::string &filename, int lineno)
{
  ++num_asserts_val;
  if (!(actual < expected)) {
    std::stringstream ss;
    ss << "expected \"" << actual;
    ss << "\" to be less than \"" << expected << "\"";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename A, typename B>
void Assert::assert_less_or_equal(const A &expected, const B &actual,
  const std::string &filename, int lineno)
{
  ++num_asserts_val;
  if (!(actual <= expected)) {
    std::stringstream ss;
    ss << "expected \"" << actual;
    ss << "\" to be less than or equal to \"" << expected << "\"";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename A, typename B>
void Assert::assert_greater(const A &expected, const B &actual,
  const std::string &filename, int lineno)
{
  ++num_asserts_val;
  if (!(actual > expected)) {
    std::stringstream ss;
    ss << "expected \"" << actual;
    ss << "\" to be greater than \"" << expected << "\"";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename A, typename B>
void Assert::assert_greater_or_equal(const A &expected, const B &actual,
  const std::string &filename, int lineno)
{
  ++num_asserts_val;
  if (!(actual >= expected)) {
    std::stringstream ss;
    ss << "expected \"" << actual;
    ss << "\" to be greater than or equal to \"" << expected << "\"";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename Expected, typename Actual>
void Assert::assert_instance_of(const Actual &actual,
  const std::string &filename, int lineno)
{
  ++num_asserts_val;
  if (!dynamic_cast<const Expected*>(actual)) {
    std::stringstream ss;
    ss << "expected \"" << demangle(typeid(*actual));
    ss << "\" to be instance of \"" << demangle(typeid(Expected)) << "\"";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename Expected, typename Actual>
void Assert::assert_not_instance_of(const Actual &actual,
  const std::string &filename, int lineno)
{
  ++num_asserts_val;
  if (dynamic_cast<const Expected*>(actual)) {
    std::stringstream ss;
    ss << "expected \"" << demangle(typeid(*actual));
    ss << "\" to not be instance of \"" << demangle(typeid(Expected)) << "\"";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename Expected>
void Assert::assert_exception(const std::function<void()> &f,
  const std::string &filename, int lineno)
{
  ++num_asserts_val;
  std::stringstream ss;
  ss << "expected exception \""  << demangle(typeid(Expected)) << "\", ";
  try {
    f();
    ss << "none thrown";
  } catch (Expected) {
    return;
  } catch (std::exception &e) {
    ss << "caught \"" << demangle(typeid(e))<< "\"";
  } catch(...) {
    ss << "caught unknown";
  }
  throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
}

template<typename Expected, typename Actual>
void Assert::assert_array_equal(const Expected *expected,
  const Actual *actual, int size, const std::string &filename, int lineno)
{
  ++num_asserts_val;
  std::vector<int> d(diff(expected, expected + size, actual, actual + size));
  if (d.size() > size) {
    std::stringstream ss;
    ss << "expected {";
    for (int i = 0; i < size; ++i) {
      if (i) ss << ", ";
      ss << '"' << actual[i] << '"';
    }
    ss << "} to be equal to {";
    for (int i = 0; i < size; ++i) {
      if (i) ss << ", ";
      ss << '"' << expected[i] << '"';
    }
    ss << "}";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno)
      .diff(sdiff(d,expected,actual)));
  }
}

template<typename Expected, typename Actual>
void Assert::assert_array_not_equal(const Expected *expected,
  const Actual *actual, int size, const std::string &filename, int lineno)
{
  ++num_asserts_val;
  std::vector<int> d(diff(expected, expected + size, actual, actual + size));
  if (d.size() == size) {
    std::stringstream ss;
    ss << "expected {";
    for (int i = 0; i < size; ++i) {
      if (i) ss << ", ";
      ss << '"' << actual[i] << '"';
    }
    ss << "} to not be equal to {";
    for (int i = 0; i < size; ++i) {
      if (i) ss << ", ";
      ss << '"' << expected[i] << '"';
    }
    ss << "}";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename Expected, typename Actual>
void Assert::assert_container_equal(const Expected &expected,
  const Actual &actual, const std::string &filename, int lineno)
{
  ++num_asserts_val;
  std::vector<int> d(diff(begin(expected), end(expected), begin(actual), end(actual)));
  if (d.size() > distance(begin(expected), end(expected))) {
    std::stringstream ss;
    bool first = true;
    ss << "expected {";
    for (const auto &i : actual) {
      if (!first) ss << ", ";
      ss << '"' << i << '"';
      first = false;
    }
    first = true;
    ss << "} to be equal to {";
    for (const auto &i : expected) {
      if (!first) ss << ", ";
      ss << '"' << i << '"';
      first = false;
    }
    ss << "}";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno)
      .diff(sdiff(d,begin(expected),begin(actual))));
  }
}

template<typename Expected, typename Actual>
void Assert::assert_container_not_equal(const Expected &expected,
  const Actual &actual, const std::string &filename, int lineno)
{
  ++num_asserts_val;
  std::vector<int> d(diff(begin(expected), end(expected), begin(actual), end(actual)));
  if (d.size() == distance(begin(expected), end(expected))) {
    std::stringstream ss;
    bool first = true;
    ss << "expected {";
    for (const auto &i : actual) {
      if (!first) ss << ", ";
      ss << '"' << i << '"';
      first = false;
    }
    first = true;
    ss << "} to not be equal to {";
    for (const auto &i : expected) {
      if (!first) ss << ", ";
      ss << '"' << i << '"';
      first = false;
    }
    ss << "}";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename T>
void Assert::assert_container_empty(const T &container,
  const std::string &filename, int lineno)
{
  ++num_asserts_val;
  if (distance(begin(container),end(container)) != 0) {
    std::stringstream ss;
    bool first = true;
    ss << "expected {";
    for (const auto &i : container) {
      if (!first) ss << ", ";
      ss << '"' << i << '"';
      first = false;
    }
    ss << "} to be empty";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename T>
void Assert::assert_container_not_empty(const T &container,
  const std::string &filename, int lineno)
{
  ++num_asserts_val;
  if (distance(begin(container),end(container)) == 0) {
    std::stringstream ss;
    bool first = true;
    ss << "expected {";
    for (const auto &i : container) {
      if (!first) ss << ", ";
      ss << '"' << i << '"';
      first = false;
    }
    ss << "} to not be empty";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename T>
void Assert::assert_container_size(size_t expected, const T &actual,
  const std::string &filename, int lineno)
{
  ++num_asserts_val;
  if (distance(begin(actual),end(actual)) != expected) {
    std::stringstream ss;
    bool first = true;
    ss << "expected {";
    for (const auto &i : actual) {
      if (!first) ss << ", ";
      ss << '"' << i << '"';
      first = false;
    }
    ss << "} to have size " << expected;
    ss << " (" << distance(begin(actual),end(actual)) << ")";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename T>
void Assert::assert_container_not_size(size_t expected, const T &actual,
  const std::string &filename, int lineno)
{
  ++num_asserts_val;
  if (distance(begin(actual),end(actual)) == expected) {
    std::stringstream ss;
    bool first = true;
    ss << "expected {";
    for (const auto &i : actual) {
      if (!first) ss << ", ";
      ss << '"' << i << '"';
      first = false;
    }
    ss << "} to not have size " << expected;
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

#endif
