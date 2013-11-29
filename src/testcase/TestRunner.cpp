#include <vector>
#include <sstream>

#include "testcase/TestSuite.h"

#include "testcase/TestRunner.h"

using namespace std;
using namespace testcase;

void TestRunner::Text(ostream &o)
{
  vector<TestInfo> failures;
  struct {
    int num_tests;
    int num_fails;
    int num_passes;
    int num_errors;
    int num_asserts;
    int num_timeouts;
  } counts = { 0, 0, 0, 0 ,0 };
  o << "\e[1m";
  o.flush();
  TestSuite::run([&o,&failures,&counts](const TestInfo& info){
    ++counts.num_tests;
    if (info.status()) {
      if (info.status() == TestInfo::FAILED) {
        o << "\e[31mF";
        ++counts.num_fails;
      } else if (info.status() == TestInfo::TIMEOUT) {
        o << "\e[36mT";
        ++counts.num_timeouts;
      } else {
        o << "\e[36mE";
        ++counts.num_errors;
      }
      failures.push_back(info);
    } else {
      o << "\e[32m.";
      ++counts.num_passes;
    }
    counts.num_asserts += info.asserts();
    o.flush();
  });
  o << endl << endl;

  int i = 0;
  for (auto failure : failures) {
    o << "\e[31m(" << i + 1 << "/" << failures.size() << ") ";
    o << failure.suite() << "::" << failure.test_case() << endl;
    o << "  \e[36m";
    if (failure.status() == TestInfo::TIMEOUT) {
      o << "Timeout";
    } else {
      if (failure.status() == TestInfo::FAILED) {
        o << "Failure: ";
      } else {
        o << "Error: ";
      }
      o << failure.what() << endl;
      if (failure.status() == TestInfo::FAILED) {
        if (failure.diff().size()) {
          o << endl;// << "  \e[36mDiff:" << endl;
          stringstream ss(failure.diff());
          string line;
          while (getline(ss,line)) {
            o << "  ";
            if (line[0] == '+') {
              o << "\e[32m";
            } else if (line[0] == '-') {
              o << "\e[31m";
            } else {
              o << "\e[35m";
            }
            o << line << endl;
          }
        }

        o << "  \e[35m[" << failure.filename() << ":";
        o << failure.lineno() << "]" << endl;
      } else {
        stringstream ss(failure.backtrace());
        string line;
        while (getline(ss,line)) {
          o << "  \e[35m" << line << endl;;
        }
      }
    }
    o << endl;
    ++i;
  }

  o << "\e[35m" << counts.num_tests << " tests, ";
  o << "\e[32m" << counts.num_passes << " passes, ";
  o << "\e[31m" << counts.num_fails << " failures, ";
  o << "\e[36m" << counts.num_errors << " errors, ";
  o << "\e[36m" << counts.num_timeouts << " timeouts, ";
  o << "\e[34m" << counts.num_asserts << " asserts";
  o << "\e[0m" << endl;
}
