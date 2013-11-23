# testcase

A simple unit testing framework for C++.

* Simple to add and run tests
* Simple to extend
* Setup/teardown with RAII
* Wide variety of assertions

## Example

    #include "testcase/testcase.h"

    using namespace testcase;

    TestSuite test_suite("test_suite", [](const TestCase &test) {
      int value = 2;

      test("test", [&value]{
        ASSERT_EQUAL(2,value);
      });
    });

    int main()
    {
      TestRunner::Text();
      return 0;
    }

## Installation

Generate autotools system:

    ./auto-build

Build and install

    ./configure
    make
    make installl

Use with pkg-config

    pkg-config --cflags testcase
    pkg-config --libs testcase

## Assertions

* **ASSERT_FAIL(message)** - Always fail with a message
* **ASSERT_TRUE(condition)** - Test if the given condition is true
* **ASSERT_FALSE(condition)** - Test if the given condition is false
* **ASSERT_SAME(expected,actual)** - Test if the given reference is the same as the expected
* **ASSERT_NOT_SAME(expected,actual)** - Test if the given reference is not the same as the expected
* **ASSERT_EQUAL(expected,actual)** - Test if the given value is equal to the expected
* **ASSERT_NOT_EQUAL(expected,actual)** - Test if the given value is not equal to the expected
* **ASSERT_LESS(expected,actual)** - Test if the given value is less than the expected
* **ASSERT_LESS_OR_EQUAL(expected,actual)** - Test if the given value is less than or equal to the expected
* **ASSERT_GREATER(expected,actual)** - Test if the given value is greater than the expected
* **ASSERT_GREATER_OR_EQUAL(expected,actual)** - Test if the given value is greater than or equal to the expected
* **ASSERT_MATCH(expected,actual)** - Test if the the given string matchs the expected pattern
* **ASSERT_NOT_MATCH(expected,actual)** - Test if the the given string does not matchs the expected pattern
* **ASSERT_INSTANCE_OF(expected,actual)** - Test if the given reference is an instance of the expected type
* **ASSERT_NOT_INSTANCE_OF(expected,actual)** - Test if the given reference is not an instance of the expected type
* **ASSERT_EXCEPTION(expected,lambda)** - Test if the given lambda raise the expected exception
* **ASSERT_NO_EXCEPTION(lambda)** - Test if the given lambda raise no expcetion
* **ASSERT_ARRAY_EQUAL(expected,actual,size)** - Test if the given array equals the expected
* **ASSERT_ARRAY_NOT_EQUAL(expected,actual,size)** - Test if the given array does not equal the expected
* **ASSERT_CONTAINER_EQUAL(expected,actual)** - Test if the given container equals the expected
* **ASSERT_CONTAINER_NOT_EQUAL(expected,actual)** - Test if the given container does not equal the expected
* **ASSERT_CONTAINER_EMPTY(container)** - Test if the given container is empty
* **ASSERT_CONTAINER_NOT_EMPTY(container)** - Test if the given container not is empty
* **ASSERT_CONTAINER_SIZE(container,size)** - Test if the given container is the expected size
* **ASSERT_CONTAINER_NOT_SIZE(container,size)** - Test if the given container is not the expected size
* **ASSERT_FLOAT_EQUAL(expected,actual,diff)** - Test if the given float is equal to the expected, within absolute tolerance
* **ASSERT_FLOAT_EQUAL(expected,actual,diff)** - Test if the given float is not equal to the expected, within absolute tolerance
