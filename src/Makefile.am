bin_PROGRAMS = test
test_SOURCES = Assert.cpp backtrace.cpp handler.cpp ReportStream.cpp synchronize.cpp test.cpp TestCase.cpp TestInfo.cpp TestRunner.cpp TestSuite.cpp
test_CPPFLAGS = -I$(top_builddir)/include -std=c++11
test_LDFLAGS = -pthread -rdynamic
test_LDADD = -ldl
