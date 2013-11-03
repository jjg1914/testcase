#ifndef H_TEST_CASE
#define H_TEST_CASE

#include <functional>

class TestCase {

  public:

  typedef std::function<void()> Callback;

  typedef std::function<void(const Callback&)> AsyncCase;

  typedef std::function<void()> SyncCase;

  TestCase(const AsyncCase &f);

  TestCase(const SyncCase &f);
};

#endif
