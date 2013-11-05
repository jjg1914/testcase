#ifndef H_TEST_SUITE
#define H_TEST_SUITE

#include <functional>
#include <string>
#include <list>

#include "TestCase.h"
#include "TestInfo.h"

class TestSuite {

  public:

  class Test {

    typedef std::function<void(const std::string&, const TestCase&)> Callback;

    public:

    void operator()(const std::string &name,
      const TestCase::AsyncCase &f) const;

    void operator()(const std::string &name,
      const TestCase::SyncCase &f) const;

    private:

    Test(const Callback &callback); 

    Callback callback;

    friend class TestSuite;
  };

  typedef std::function<void(const Test&)> Suite;

  typedef std::function<void(const TestInfo&)> Runner;

  static void run(const Runner &f);

  TestSuite(const std::string &name, const Suite &suite);

  TestSuite(const TestSuite &src);

  ~TestSuite();

  TestSuite &operator=(const TestSuite &src);

  private:

  static std::list<const TestSuite*> suites;

  void run_suite(const Runner &f) const;

  std::list<const TestSuite*>::iterator self;

  std::string name;

  Suite suite;
};

#endif
