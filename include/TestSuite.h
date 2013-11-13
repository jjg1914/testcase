#ifndef H_TEST_SUITE
#define H_TEST_SUITE

#include <functional>
#include <string>
#include <list>

#include "TestCase.h"
#include "TestInfo.h"

class TestSuite {

  public:

  typedef std::function<void(const TestCase&)> Suite;

  static void run(const TestCase::Runner &f);

  TestSuite(const std::string &name, const Suite &suite);

  TestSuite(const TestSuite &src);

  ~TestSuite();

  TestSuite &operator=(const TestSuite &src);

  private:

  static std::list<const TestSuite*> suites;

  std::list<const TestSuite*>::iterator self;

  std::string name;

  Suite suite;
};

#endif
