#include <csignal>

#include "backtrace.h"
#include "Assert.h"

#include "handler.h"

using namespace std;

namespace {
  
  ReportStream *report_stream(NULL);

  void handle_terminate()
  {
    try {
      throw;
    } catch (Assert::Error &e) {
      (*report_stream) << e.info();
    } catch (exception &e) {
      (*report_stream) << TestInfo::error(string(e.what()) + " (" + typeid(e).name() + ")")
        .backtrace(sbacktrace());
    }
    abort();
  }

  void handle_sigfpe(int)
  {
    (*report_stream) << TestInfo::error("Arithmetic Error").backtrace(sbacktrace());
    abort();
  }

  void handle_sigsegv(int)
  {
    (*report_stream) << TestInfo::error("Segmentation Fault").backtrace(sbacktrace());
    abort();
  }
}

void handler_install(ReportStream *rs)
{
  report_stream = rs;
  set_terminate(handle_terminate);
  signal(SIGFPE,handle_sigfpe);
  signal(SIGSEGV,handle_sigsegv);
}
