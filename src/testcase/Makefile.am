lib_LTLIBRARIES = libtestcase.la

libtestcase_la_includedir = $(includedir)/testcase
libtestcase_la_include_HEADERS =  Assert.h ReportStream.h testcase.h TestCase.h TestInfo.h TestRunner.h TestSuite.h util.h

libtestcase_la_pkgconfigdir = $(libdir)/pkgconfig
libtestcase_la_pkgconfig_DATA = testcase.pc

libtestcase_la_SOURCES = Assert.cpp ReportStream.cpp TestCase.cpp TestInfo.cpp TestRunner.cpp TestSuite.cpp util.cpp
libtestcase_la_CPPFLAGS = -I$(top_builddir)/src -std=c++11
libtestcase_la_LDFLAGS = -pthread -rdynamic -version-info 1:0:1
libtestcase_la_LIBADD = -ldl
