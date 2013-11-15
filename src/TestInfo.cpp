#include <iostream>
#include <sstream>

#include "Assert.h"

#include "TestInfo.h"

using namespace std;

namespace {
  string encode_string(const string &src)
  {
    stringstream in(src), out;
    int c;
    while (c = in.get(), !in.eof()) {
      if (iscntrl(c) || c == '&') {
        out << "&" << c << ";";
      } else {
        out.put(c);
      }
    }
    return out.str();
  }

  string decode_string(const string &src)
  {
    stringstream in(src), out;
    int c;
    while (c = in.get(), !in.eof()) {
      if (c == '&') {
        stringstream val;
        while (c = in.get(), c != ';') {
          val.put(c);
        }
        val >> c;
      }
      out.put(c);
    }
    return out.str();
  }
}

TestInfo TestInfo::passed()
{
  return TestInfo().status(PASSED).asserts(Assert::num_asserts());
}

TestInfo TestInfo::failed(const std::string &what, const std::string &file, int line)
{
  return TestInfo().status(FAILED)
    .what(what)
    .filename(file)
    .lineno(line)
    .asserts(Assert::num_asserts());
}

TestInfo TestInfo::error(const std::string &what)
{
  return TestInfo().status(ERROR).what(what).asserts(Assert::num_asserts());
}

TestInfo::TestInfo()
  : status_val(FAILED),
    filename_val(),
    lineno_val(0),
    test_case_val(),
    suite_val(),
    backtrace_val(),
    what_val(),
    asserts_val(0),
    diff_val()
{}

TestInfo TestInfo::deserialize(const string &src)
{
  TestInfo info;
  stringstream ss(src);
  string line;

  while (getline(ss, line), !ss.eof()) {
    size_t delim = line.find(':');
    string attr = line.substr(0,delim);
    string value = line.substr(delim + 1);
    if (attr == "status") {
      stringstream toi(value);
      toi >> *(int*)&info.status_val;
    } else if (attr == "filename") {
      info.filename_val = decode_string(value);
    } else if (attr == "lineno") {
      stringstream toi(value);
      toi >> info.lineno_val;
    } else if (attr == "test_case") {
      info.test_case_val = decode_string(value);
    } else if (attr == "suite") {
      info.suite_val = decode_string(value);
    } else if (attr == "backtrace") {
      info.backtrace_val = decode_string(value);
    } else if (attr == "what") {
      info.what_val = decode_string(value);
    } else if (attr == "asserts") {
      stringstream toi(value);
      toi >> info.asserts_val;
    } else if (attr == "diff") {
      info.diff_val = decode_string(value);
    }
  }

  return info;
}

string TestInfo::serialize() const
{
  stringstream ss;
  ss << "status:" << status_val << endl;
  ss << "filename:" << encode_string(filename_val) << endl;
  ss << "lineno:" << lineno_val << endl;
  ss << "test_case:" << encode_string(test_case_val) << endl;
  ss << "suite:" << encode_string(suite_val) << endl;
  ss << "backtrace:" << encode_string(backtrace_val) << endl;
  ss << "what:" << encode_string(what_val) << endl;
  ss << "asserts:" << asserts_val << endl;
  ss << "diff:" << encode_string(diff_val) << endl;
  return ss.str();
}

TestInfo::Status TestInfo::status() const
{
  return status_val;
}

TestInfo TestInfo::status(TestInfo::Status status_val) const
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

const string &TestInfo::backtrace() const
{
  return backtrace_val;
}

TestInfo TestInfo::backtrace(const string &backtrace_val) const
{
  TestInfo rval(*this);
  rval.backtrace_val = backtrace_val;
  return rval;
}

const string &TestInfo::what() const
{
  return what_val;
}

TestInfo TestInfo::what(const string &what_val) const
{
  TestInfo rval(*this);
  rval.what_val = what_val;
  return rval;
}

int TestInfo::asserts() const
{
  return asserts_val;
}

TestInfo TestInfo::asserts(int asserts_val) const
{
  TestInfo rval(*this);
  rval.asserts_val = asserts_val;
  return rval;
}

const string &TestInfo::diff() const
{
  return diff_val;
}

TestInfo TestInfo::diff(const string &diff_val) const
{
  TestInfo rval(*this);
  rval.diff_val = diff_val;
  return rval;
}
