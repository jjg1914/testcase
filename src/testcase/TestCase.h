#ifndef H_TESTCASE_TEST_CASE
#define H_TESTCASE_TEST_CASE

#include <functional>

#include "testcase/TestInfo.h"

namespace testcase {

  class TestCase {

    public:

    typedef std::function<void()> Callback;

    typedef std::function<void(const Callback&)> AsyncCase;

    typedef std::function<void()> SyncCase;

    typedef std::function<void(const testcase::TestInfo&)> Runner;

    TestCase(const Runner &f);

    void operator()(const std::string &name, const AsyncCase &f) const;

    void operator()(const std::string &name, const SyncCase &f) const;

    private:

    Runner runner;
  };
}

#endif
