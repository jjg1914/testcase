#ifndef H_TESTCASE_TEST_SUITE
#define H_TESTCASE_TEST_SUITE

#include <functional>
#include <string>
#include <list>

#include "testcase/TestCase.h"
#include "testcase/TestInfo.h"

namespace testcase {

  class TestSuite {

    public:

    typedef std::function<void(const testcase::TestCase&)> Suite;

    static void run(const testcase::TestCase::Runner &f);

    TestSuite(const std::string &name, const Suite &suite);

    TestSuite(const testcase::TestSuite &src);

    ~TestSuite();

    TestSuite &operator=(const testcase::TestSuite &src);

    private:

    static std::list<const testcase::TestSuite*> suites;

    std::list<const testcase::TestSuite*>::iterator self;

    std::string name;

    Suite suite;
  };
}

#endif
