#include <string>
#include <list>
#include <stdexcept>
#include <iostream>
#include <thread>
#include <chrono>

#include "testcase/testcase.h"
#include "testcase/Mock.h"
#include <cstdint>

using namespace std;
using namespace testcase;

struct A { virtual ~A() {}; } a;
struct B : public A { virtual ~B() {}; } b;

volatile int v = -1;

int throw_sigfpe(volatile int &a,int b)
{
  return a/b;
}

int throw_sigseg(int *i)
{
  return *i;
}

void throw_logic_error()
{
  throw logic_error("logic error");
}

void throw_int()
{
  throw 1;
}

int mocked(int i) {
  return i + 1;
}

void void_mocked(int& i) {
  i = 2;
}

struct M {
  int foo();
  int foov() volatile;
  int bar() const;
  int barv() const volatile;
  virtual int vbaz();
};

int M::foo() {
  return 0;
}

int M::foov() volatile {
  return 0;
}

int M::bar() const{
  return 0;
}

int M::barv() const volatile {
  return 0;
}

int M::vbaz() {
  return 0;
}

int main() {
  TestSuite suite("test suite", [](const TestCase &test) {
    A a;
    B b;

    test("test 1", []{
      ASSERT_FAIL("always flunk");
    });

    test("test 2", []{
      ASSERT_TRUE(2 == 3);
    });

    test("test 3", []{
      ASSERT_EQUAL(2, 3);
    });

    test("test 4", [&a,&b]{
      ASSERT_SAME(a, b);
    });

    test("test 5", []{
      ASSERT_LESS(2, 3);
    });

    test("test 6", []{
      ASSERT_LESS_OR_EQUAL(2, 3);
    });

    test("test 7", []{
      ASSERT_GREATER(2, 1);
    });

    test("test 8", []{
      ASSERT_GREATER_OR_EQUAL(2, 1);
    });

    test("test 9", []{
      ASSERT_MATCH("a*b", "aaa");
    });

    test("test 10", [a]{
      ASSERT_INSTANCE_OF(B, &a);
    });

    test("test 11", []{
      ASSERT_EXCEPTION(runtime_error, []{});
    });

    test("test 12", []{
      ASSERT_EXCEPTION(runtime_error, []{
        throw logic_error("logic error");
      });
    });

    test("test 13", []{ASSERT_TRUE(true);});

    test("test 14", []{
      string s("s");
      v = throw_sigfpe(v,0);
      cout << v << endl;
    });

    test("test 15", []{
      string s("s");
      v = throw_sigseg(NULL);
      cout << v << endl;
    });

    test("test 16", []{
      string s("s");
      throw_logic_error();
    });

    test("test 17", []{
      ASSERT_EXCEPTION(runtime_error, []{
        throw 1;
      });
    });

    test("test 18", []{
      string s("s");
      throw_int();
    });

    test("test 19", []{
      ASSERT_FALSE(2 == 2);
    });

    test("test 20", [&a]{
      ASSERT_NOT_SAME(a, a);
    });

    test("test 21", [&a]{
      ASSERT_NOT_EQUAL(2, 2);
    });

    test("test 22", []{
      ASSERT_NOT_MATCH("a*b", "aaab");
    });

    test("test 23", [a]{
      ASSERT_NOT_INSTANCE_OF(A, &a);
    });

    test("test 24", []{
      ASSERT_NO_EXCEPTION([]{
        throw logic_error("logic error");
      });
    });

    test("test 25", []{
      int x[5] = {1, 2, 2, 6, 7};
      int y[5] = {2, 3, 6, 7, 8};
      ASSERT_ARRAY_EQUAL(x,y,5);
    });

    test("test 26", []{
      int x[5] = {1, 2, 2, 6, 7};
      int y[5] = {1, 2, 2, 6, 7};
      ASSERT_ARRAY_NOT_EQUAL(x,y,5);
    });

    test("test 27", []{
      list<string> x = { "foo", "bar", "baz" };
      list<string> y = { "moo", "foo", "baz" };
      ASSERT_CONTAINER_EQUAL(x,y);
    });

    test("test 28", []{
      list<string> x = { "foo", "bar", "baz" };
      list<string> y = { "foo", "bar", "baz" };
      ASSERT_CONTAINER_NOT_EQUAL(x,y);
    });

    test("test 29", []{
      list<string> x = { "foo", "bar", "baz" };
      ASSERT_CONTAINER_EMPTY(x);
    });

    test("test 30", []{
      list<string> x = {};
      ASSERT_CONTAINER_NOT_EMPTY(x);
    });

    test("test 31", []{
      list<string> x = { "foo", "bar", "baz" };
      ASSERT_CONTAINER_SIZE(4,x);
    });

    test("test 32", []{
      list<string> x = { "foo", "bar", "baz" };
      ASSERT_CONTAINER_NOT_SIZE(3,x);
    });

    test("test 33", []{
      ASSERT_FLOAT_EQUAL(5.005,5.007,0.001);
    });

    test("test 34", []{
      ASSERT_FLOAT_NOT_EQUAL(5.005,5.007,0.003);
    });

    test("test 35", []{
      ASSERT_FLOAT_EQUAL((double)5.005,(double)5.007,(double)0.001);
    });

    test("test 36", []{
      ASSERT_FLOAT_NOT_EQUAL((double)5.005,(double)5.007,(double)0.003);
    });

    test("test 37", []{
      int x[] = { 1, 2, 3 };
      int y[] = { 1, 4, 5 };
      ASSERT_CONTAINER_EQUAL(x,y);
    });

    test("test 38", 100, []{
      this_thread::sleep_for(chrono::milliseconds(1000));
    });

    test("test 39", []{
      ASSERT_CALLBACK(3, callback);
      callback();
      callback();
    });

    test("test 40", []{
      list<int> x = { 1, 2, 3 };
      list<int> y = {};
      ASSERT_CONTAINER_EQUAL(x,y);
    });

    test("test 41", 500, [](const TestCase::Callback &cb){
      string s("s");
      thread t([cb]{
        cb();
      });
    });

    test("test 42", 500, [](const TestCase::Callback &cb){
      thread t([cb]{
        cb();
      });
      t.detach();
    });

    test("test 43", []{
      int j = 3;
      MOCK(mocked,[j](int i) {
        return i + j;
      });
      ASSERT_EQUAL(5,mocked(2));
    });

    test("test 44", []{
      int j = 3;
      {
        MOCK(mocked,[j](int i) {
          return i + j;
        });
      }
      ASSERT_EQUAL(3,mocked(2));
    });

    test("test 45", []{
      int i = 0;
      MOCK(void_mocked,[](int& j){
        j = 3;
      });
      void_mocked(i);
      ASSERT_EQUAL(3,i);
    });

    test("test 46", []{
      MOCK(&M::foo, [](M* m){
        return 1;
      });
      M m;
      ASSERT_TRUE(m.foo());
    });

    test("test 47", []{
      MOCK(&M::foov, [](volatile M* m){
        return 1;
      });
      M m;
      ASSERT_TRUE(m.foov());
    });

    test("test 48", []{
      MOCK(&M::bar, [](const M* m){
        return 1;
      });
      M m;
      ASSERT_TRUE(m.bar());
    });

    test("test 49", []{
      MOCK(&M::barv, [](const volatile M* m){
        return 1;
      });
      M m;
      ASSERT_TRUE(m.barv());
    });

    test("test 50", []{
      M m;
      MOCK(&M::vbaz, [](M* m){
        return 1;
      });
      ASSERT_TRUE(m.vbaz());
    });

    test("test 51", []{
      MOCK(&M::foo, [](M* m){
        return 1;
      });
      MOCK(&M::bar, [](const M* m){
        return 1;
      });
      M m;
      ASSERT_TRUE(m.foo() && m.bar());
    });

    test("test 51");

    test("test 52", []{});
  });

  TestRunner::Text();
}
