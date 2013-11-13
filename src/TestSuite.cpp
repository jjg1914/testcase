#include <iostream>
#include "TestSuite.h"

using namespace std;

std::list<const TestSuite*> TestSuite::suites;

void TestSuite::run(const TestCase::Runner &f)
{
  for (auto suite : suites) {
    TestCase test([&suite,&f](const TestInfo &info){
      f(info.suite(suite->name));
    });
    suite->suite(test);
  }
}

TestSuite::TestSuite(const string &name, const TestSuite::Suite &suite)
  : name(name),
    suite(suite)
{
  suites.push_back(this);
  self = --suites.end();
}

TestSuite::TestSuite(const TestSuite &src)
  : name(src.name),
    suite(src.suite)
{
  suites.push_back(this);
  self = --suites.end();
}

TestSuite::~TestSuite()
{
  if (self != suites.end()) {
    suites.erase(self);
  }
}

TestSuite &TestSuite::operator=(const TestSuite &src)
{
  if (&src != this) {
    name = src.name;
    suite = src.suite;
  }
  return *this;
}
