#ifndef H_ASSERT
#define H_ASSERT

#include <exception>
#include <string>

#include "TestInfo.h"

#define ASSERT_FAIL() Assert::assert_fail(__FILE__,__LINE__)

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
};

#endif
