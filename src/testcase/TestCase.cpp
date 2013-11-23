#include "testcase/util.h"
#include "testcase/ReportStream.h"

#include "testcase/TestCase.h"

using namespace std;
using namespace testcase;

TestCase::TestCase(const TestCase::Runner &f)
  : runner(f)
{}

void TestCase::operator()(const string &name, const AsyncCase &f) const
{
  ReportStream rs;
  synchronize_on_fork([&f,&rs](){
    handler_install(&rs);
    synchronize(f);
    rs << TestInfo::passed();
  });

  TestInfo rval;
  rs >> rval;
  runner(rval.test_case(name));
}

void TestCase::operator()(const string &name, const SyncCase &f) const
{
  operator()(name,[&f](const Callback &cb){ f(); cb(); });
}
