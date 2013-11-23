#ifndef H_TESTCASE_REPORT_STREAM
#define H_TESTCASE_REPORT_STREAM

#include "ext/stdio_filebuf.h"
#include <iostream>

#include "testcase/TestInfo.h"

namespace testcase {

  class ReportStream {

    public:

    ReportStream();

    ReportStream &operator<<(const TestInfo &info);

    ReportStream &operator>>(TestInfo &info);

    private:

    struct _pipe {
      _pipe();

      const int &in;
      const int &out;

      private:

      int fileds[2];
    } p;

    __gnu_cxx::stdio_filebuf<char> inbuff, outbuff;

    std::istream in;

    std::ostream out;
  };
}

#endif
