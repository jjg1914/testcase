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
  operator()(name,0,f);
}

void TestCase::operator()(const string &name, const SyncCase &f) const
{
  operator()(name,0,[&f](const Callback &cb){ f(); cb(); });
}

void TestCase::operator()(const std::string &name, int timeout,
  const AsyncCase &f) const
{
  ReportStream rs;
  synchronize_on_fork([&f,&rs,timeout](){
    handler_install(&rs, timeout);
    synchronize(f);
    rs << TestInfo::passed();
  });

  TestInfo rval;
  rs >> rval;
  runner(rval.test_case(name));
}

void TestCase::operator()(const std::string &name, int timeout,
  const SyncCase &f) const
{
  operator()(name,timeout,[&f](const Callback &cb){ f(); cb(); });
}
