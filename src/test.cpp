#include <string>
#include <stdexcept>

#include "TestRunner.h"
#include "TestSuite.h"
#include "Assert.h"

using namespace std;

struct A { virtual ~A() {}; } a;
struct B : public A { virtual ~B() {}; } b;

int main() {
  TestSuite suite("test suite", [](const TestSuite::Test &test) {
    A a;
    B b;

    test("test 1", []{
      ASSERT_FAIL();
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
      int a = 2, b = 1;
      int c = a / --b;
      cout << c << endl;
    });

    test("test 15", []{
      int *i = NULL;
      cout << *i << endl;
    });

    test("test 16", []{
      throw logic_error("logic error");
    });

    test("test 17", []{
      ASSERT_EXCEPTION(runtime_error, []{
        throw 1;
      });
    });

  });

  TestRunner::Text();
}
