#include "backtrace.h"
#include "handler.h"
#include "synchronize.h"
#include "ReportStream.h"

#include "TestCase.h"

using namespace std;

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
