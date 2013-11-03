#include "TestInfo.h"

using namespace std;

TestInfo::TestInfo()
  : status_val(0),
    filename_val(),
    lineno_val(0),
    test_case_val(),
    suite_val()
{}

int TestInfo::status() const
{
  return status_val;
}

TestInfo TestInfo::status(int status_val) const
{
  TestInfo rval(*this);
  rval.status_val = status_val;
  return rval;
}

const string &TestInfo::filename() const
{
  return filename_val;
}

TestInfo TestInfo::filename(const string &filename_val) const
{
  TestInfo rval(*this);
  rval.filename_val = filename_val;
  return rval;
}

int TestInfo::lineno() const
{
  return lineno_val;
}

TestInfo TestInfo::lineno(int lineno_val) const
{
  TestInfo rval(*this);
  rval.lineno_val = lineno_val;
  return rval;
}

const string &TestInfo::test_case() const
{
  return test_case_val;
}

TestInfo TestInfo::test_case(const string &test_case_val) const
{
  TestInfo rval(*this);
  rval.test_case_val = test_case_val;
  return rval;
}

const string &TestInfo::suite() const
{
  return suite_val;
}

TestInfo TestInfo::suite(const string &suite_val) const
{
  TestInfo rval(*this);
  rval.suite_val = suite_val;
  return rval;
}
