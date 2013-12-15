SUBDIRS = testcase

check_PROGRAMS = test
TESTS = $(check_PROGRAMS)

test_SOURCES = test.cpp
test_CPPFLAGS = -std=c++11
test_CXXFLAGS = -Og -fno-inline -Wno-pmf-conversions
test_LDFLAGS = -rdynamic
test_LDADD = testcase/libtestcase.la
