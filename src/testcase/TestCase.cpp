#include "testcase/util.h"
#include "testcase/ReportStream.h"

#include "testcase/TestCase.h"

using namespace std;
using namespace testcase;

TestCase::TestCase(const TestCase::Runner &f)
  : runner(f)
{}

void TestCase::operator()(const std::string &name) const
{
  runner(TestInfo().test_case(name).status(TestInfo::SKIP));
}

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
    rs.close_in();
    handler_install(rs, timeout);
    synchronize(f);
    rs << TestInfo::passed();
  });

  rs.close_out();
  TestInfo rval;
  try {
    rs >> rval;
  } catch (ReportStream::EOFException e) {
    rval = TestInfo::error("test terminated without reporting");
  }
  if (!rval.status() && !rval.asserts()) {
    rval = rval.status(TestInfo::WARNING).what("No Assertions");
  }
  runner(rval.test_case(name));
}

void TestCase::operator()(const std::string &name, int timeout,
  const SyncCase &f) const
{
  operator()(name,timeout,[&f](const Callback &cb){ f(); cb(); });
}
