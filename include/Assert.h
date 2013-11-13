#ifndef H_ASSERT
#define H_ASSERT

#include <exception>
#include <string>
#include <regex>
#include <sstream>

#include "TestInfo.h"

#define ASSERT_FAIL()\
  Assert::assert_fail(__FILE__,__LINE__)

#define ASSERT_TRUE(condition)\
  Assert::assert_true(condition,#condition,__FILE__,__LINE__)

#define ASSERT_SAME(expected,actual)\
  Assert::assert_same(expected,actual,__FILE__,__LINE__)

#define ASSERT_EQUAL(expected,actual)\
  Assert::assert_equal(expected,actual,__FILE__,__LINE__)

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

#define ASSERT_INSTANCE_OF(expected,actual)\
  Assert::assert_instance_of<expected>(actual,__FILE__,__LINE__)

#define ASSERT_EXCEPTION(expected,lambda)\
  Assert::assert_exception<expected>(lambda,__FILE__,__LINE__)

class Assert {

  public:

  class Error : public std::exception {

    public:

    Error(const TestInfo &info_val) noexcept;

    TestInfo info() const noexcept;

    private:

    TestInfo info_val;
  };

  static void assert_fail(const std::string &filename, int lineno);

  static void assert_true(bool cond, const std::string &expr,
    const std::string &filename, int lineno);

  template<typename A, typename B>
  static void assert_same(const A &expected, const B &actual,
    const std::string &filename, int lineno);

  template<typename A, typename B>
  static void assert_equal(const A &expected, const B &actual,
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

  template<typename Expected, typename Actual>
  static void assert_instance_of(const Actual &actual,
    const std::string &filename, int lineno);

  template<typename Expected>
  static void assert_exception(const std::function<void()> &f,
    const std::string &filename, int lineno);
};

template<typename A, typename B>
void Assert::assert_same(const A &expected, const B &actual,
  const std::string &filename, int lineno)
{
  if (&expected != &actual) {
    std::stringstream ss;
    ss << "expected \"" << &actual << "\" to be the same as \"" << &expected << "\"";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename A, typename B>
void Assert::assert_equal(const A &expected, const B &actual,
  const std::string &filename, int lineno)
{
  if (!(actual == expected)) {
    std::stringstream ss;
    ss << "expected \"" << actual << "\" be equal to \"" << expected << "\"";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename A, typename B>
void Assert::assert_less(const A &expected, const B &actual,
  const std::string &filename, int lineno)
{
  if (!(actual < expected)) {
    std::stringstream ss;
    ss << "expected \"" << actual << "\" to be less than \"" << expected << "\"";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename A, typename B>
void Assert::assert_less_or_equal(const A &expected, const B &actual,
  const std::string &filename, int lineno)
{
  if (!(actual <= expected)) {
    std::stringstream ss;
    ss << "expected \"" << actual << "\" to be less than or equal to \"" << expected << "\"";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename A, typename B>
void Assert::assert_greater(const A &expected, const B &actual,
  const std::string &filename, int lineno)
{
  if (!(actual > expected)) {
    std::stringstream ss;
    ss << "expected \"" << actual << "\" to be greater than \"" << expected << "\"";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename A, typename B>
void Assert::assert_greater_or_equal(const A &expected, const B &actual,
  const std::string &filename, int lineno)
{
  if (!(actual >= expected)) {
    std::stringstream ss;
    ss << "expected \"" << actual << "\" to be greater than or equal to \"" << expected << "\"";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}

template<typename Expected, typename Actual>
void Assert::assert_instance_of(const Actual &actual,
  const std::string &filename, int lineno)
{
  if (!dynamic_cast<const Expected*>(actual)) {
    std::stringstream ss;
    ss << "expected \"" << typeid(actual).name() << "\" to be instance of \"" << typeid(Expected).name() << "\"";
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
  }
}


template<typename Expected>
void Assert::assert_exception(const std::function<void()> &f,
  const std::string &filename, int lineno)
{
  std::stringstream ss;
  try {
    f();
    ss << "expected exception \""  << typeid(Expected).name() << "\", none thrown";
  } catch (Expected) {
    return;
  } catch (std::exception &e) {
    ss << "expected exception \""  << typeid(Expected).name() << "\", caught \"" << typeid(e).name()<< "\"";
  } catch(...) {
    ss << "expected exception \""  << typeid(Expected).name() << "\", caught unknown";
  }
    throw Assert::Error(TestInfo::failed(ss.str(), filename, lineno));
}

#endif
