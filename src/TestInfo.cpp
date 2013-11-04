#include <sstream>

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

TestInfo::TestInfo()
  : status_val(0),
    filename_val(),
    lineno_val(0),
    test_case_val(),
    suite_val(),
    report_val()
{}

TestInfo::TestInfo(const string &src)
  : status_val(0),
    filename_val(),
    lineno_val(0),
    test_case_val(),
    suite_val(),
    report_val()
{
  stringstream ss(src);
  string line;

  while (getline(ss, line), !ss.eof()) {
    size_t delim = line.find(':');
    string attr = line.substr(0,delim);
    string value = line.substr(delim + 1);
    if (attr == "status") {
      stringstream toi(value);
      toi >> status_val;
    } else if (attr == "filename") {
      filename_val = decode_string(value);
    } else if (attr == "lineno") {
      stringstream toi(value);
      toi >> lineno_val;
    } else if (attr == "test_case") {
      test_case_val = decode_string(value);
    } else if (attr == "suite") {
      suite_val = decode_string(value);
    } else if (attr == "report") {
      report_val = decode_string(value);
    }
  }
}

TestInfo::operator string() const
{
  stringstream ss;
  ss << "status:" << status_val << endl;
  ss << "filename:" << encode_string(filename_val) << endl;
  ss << "lineno:" << lineno_val << endl;
  ss << "test_case:" << encode_string(test_case_val) << endl;
  ss << "suite:" << encode_string(suite_val) << endl;
  ss << "report:" << encode_string(report_val) << endl;
  return ss.str();
}

bool TestInfo::status() const
{
  return status_val;
}

TestInfo TestInfo::status(bool status_val) const
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

const string &TestInfo::report() const
{
  return report_val;
}

TestInfo TestInfo::report(const string &report_val) const
{
  TestInfo rval(*this);
  rval.report_val = report_val;
  return rval;
}
