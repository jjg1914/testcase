#include <string>
#include <list>
#include <stdexcept>

#include "testcase/testcase.h"

using namespace std;
using namespace testcase;

struct A { virtual ~A() {}; } a;
struct B : public A { virtual ~B() {}; } b;

int throw_sigfpe(int a,int b)
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
      throw_sigfpe(0,0);
    });

    test("test 15", []{
      throw_sigseg(NULL);
    });

    test("test 16", []{
      throw_logic_error();
    });

    test("test 17", []{
      ASSERT_EXCEPTION(runtime_error, []{
        throw 1;
      });
    });

    test("test 18", []{
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
  });

  TestRunner::Text();
}
