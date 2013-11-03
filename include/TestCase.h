#ifndef H_TEST_CASE
#define H_TEST_CASE

#include <functional>

#include "TestInfo.h"

class TestCase {

  public:

  typedef std::function<void()> Callback;

  typedef std::function<void(const Callback&)> AsyncCase;

  typedef std::function<void()> SyncCase;

  TestCase(const AsyncCase &f);

  TestCase(const SyncCase &f);

  const TestInfo &result() const;

  private:

  TestInfo result_val;
};

#endif
