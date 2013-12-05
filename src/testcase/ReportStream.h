#ifndef H_TESTCASE_REPORT_STREAM
#define H_TESTCASE_REPORT_STREAM

#include "ext/stdio_filebuf.h"
#include <iostream>
#include <stdexcept>

#include "testcase/TestInfo.h"

namespace testcase {

  class ReportStream {

    public:

    class EOFException : public std::logic_error {

      public:

      EOFException();
    };

    ReportStream();

    ReportStream &operator<<(const TestInfo &info);

    ReportStream &operator>>(TestInfo &info);

    void close_in();

    void close_out();

    private:

    struct _pipe {
      _pipe();

      const int &in;
      const int &out;

      private:

      int fileds[2];
    } p;

    int pid;

    __gnu_cxx::stdio_filebuf<char> inbuff, outbuff;

    std::istream in;

    std::ostream out;
  };
}

#endif
