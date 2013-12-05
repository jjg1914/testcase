#ifndef H_BACKTRACE
#define H_BACKTRACE

#include <string>
#include <functional>
#include <stdexcept>

namespace testcase {

  struct ErrorInfo {
    enum Errno {
      ESEGV,
      EFPE,
      EILL,
      EBUS,
      ETERM,
      EEXP,
      EUNKNOWN
    };

    Errno eno;
    int depth;

    ErrorInfo(Errno eno, int depth);
  };

  typedef std::function<void(const ErrorInfo&,const std::exception &e)>
    backtrace_exception_f;

  std::string sbacktrace(int bottom = 0, int top = 0);

  void backtrace_exception(const backtrace_exception_f &f);

  std::string backtrace_demangle(const std::string &sym);

  int backtrace_depth();

  void backtrace_trap();
}

#endif
