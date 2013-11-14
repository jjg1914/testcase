#include <vector>
#include <sstream>

#include "TestSuite.h"

#include "TestRunner.h"

using namespace std;

void TestRunner::Text(ostream &o)
{
  vector<TestInfo> failures;
  int num_fails = 0, num_pass = 0, num_errors = 0;
  o << "\e[1m";
  o.flush();
  TestSuite::run([&o,&failures,&num_pass,&num_fails,&num_errors](const TestInfo& info){
    if (info.status()) {
      if (info.status() == TestInfo::FAILED) {
        o << "\e[31mF";
        ++num_fails;
      } else {
        o << "\e[36mE";
        ++num_errors;
      }
      failures.push_back(info);
    } else {
      o << "\e[32m.";
      ++num_pass;
    }
    o.flush();
  });
  o << endl << endl;

  int i = 0;
  for (auto failure : failures) {
    o << "\e[31m(" << i + 1 << "/" << failures.size() << ") ";
    o << failure.suite() << "::" << failure.test_case() << endl;
    o << "  \e[36m";
    if (failure.status() == TestInfo::FAILED) {
      o << "Failure: ";
    } else {
      o << "Error: ";
    }
    o << failure.what() << endl;
    if (failure.status() == TestInfo::FAILED) {
      o << "  \e[35m[" << failure.filename() << ":";
      o << failure.lineno() << "]" << endl;
    } else {
      stringstream ss(failure.backtrace());
      string line;
      while (getline(ss,line)) {
        o << "  \e[35m" << line << endl;;
      }
    }
    o << endl;
    ++i;
  }

  o << "\e[35m" << num_fails + num_pass << " test, ";
  o << "\e[31m" << num_fails << " failures, ";
  o << "\e[36m" << num_errors << " errors";
  o << "\e[0m" << endl;
}
