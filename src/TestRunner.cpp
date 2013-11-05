#include <vector>

#include "TestSuite.h"

#include "TestRunner.h"

using namespace std;

void TestRunner::Text(ostream &o)
{
  vector<TestInfo> failures;
  int num_fails = 0, num_pass = 0;
  o << "\e[1m";
  o.flush();
  TestSuite::run([&o,&failures,&num_pass,&num_fails](const TestInfo& info){
    if (info.status()) {
      o << "\e[32m.";
      ++num_pass;
    } else {
      o << "\e[31mF";
      ++num_fails;
      failures.push_back(info);
    }
    o.flush();
  });
  o << endl << endl;

  int i = 0;
  for (auto failure : failures) {
    cout << "\e[31m(" << i + 1 << "/" << num_fails << ") ";
    cout << failure.suite() << "::" << failure.test_case() << endl;
    cout << "  \e[35m[" << failure.filename() << ":" << failure.lineno() << "]" << endl;
    cout << endl;
    ++i;
  }

  cout << "\e[36m" << num_fails + num_pass << " test, \e[31m" << num_fails << " failures\e[0m" << endl;
}
