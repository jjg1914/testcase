#include <iostream>
#include "TestSuite.h"

using namespace std;

void TestSuite::Test::operator()(const string &name,
  const TestCase::AsyncCase &f) const
{
  callback(name, f);
}

void TestSuite::Test::operator()(const string &name,
  const TestCase::SyncCase &f) const
{
  callback(name, f);
}

TestSuite::Test::Test(const Callback &callback)
  : callback(callback)
{}

void TestSuite::run(const TestSuite::Runner &f)
{
  for (auto suite : suites) {
    suite->run_suite(f);
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

std::list<const TestSuite*> TestSuite::suites;

void TestSuite::run_suite(const Runner &runner) const
{
  Test test([this,&runner](const string& name, const TestCase &f){
    runner(f.result().test_case(name).suite(this->name));
  });
  suite(test);
}
