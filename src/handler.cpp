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
      (*report_stream) << e.info().status(TestInfo::FAILED);
    } catch (exception &e) {
      (*report_stream) << TestInfo().status(TestInfo::ERROR).what(string("Exception: ") + e.what() + string(" (") +  typeid(e).name() + string(")")).backtrace(sbacktrace());
    }
    abort();
  }

  void handle_sigfpe(int)
  {
    (*report_stream) << TestInfo().status(TestInfo::ERROR).what("Error: Segmentation Fault").backtrace(sbacktrace());
    abort();
  }

  void handle_sigsegv(int)
  {
    (*report_stream) << TestInfo().status(TestInfo::ERROR).what("Error: Arithmetic Error").backtrace(sbacktrace());
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
