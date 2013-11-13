#include <cstdlib>
#include <iostream>
#include <fstream>


#include "backtrace.h"
#include "handler.h"
#include "synchronize.h"
#include "ReportStream.h"
#include "Assert.h"

#include "TestCase.h"

using namespace std;

namespace {
  TestInfo test(const TestCase::AsyncCase &f)
  {
    ReportStream rs;
    synchronize_on_fork([&f,&rs](){
      handler_install(&rs);
      synchronize(f);
      rs << TestInfo().status(TestInfo::PASSED);
    });

    TestInfo rval;
    rs >> rval;
    return rval;
  }
}

TestCase::TestCase(const TestCase::AsyncCase &f)
  : result_val(test(f))
{}

TestCase::TestCase(const TestCase::SyncCase &f)
  : result_val(test([&f](const Callback &cb){ f(), cb(); }))
{
}

const TestInfo &TestCase::result() const
{
  return result_val;
}
