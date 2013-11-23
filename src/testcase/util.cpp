#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include <cstdlib>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <memory>

#include "testcase/Assert.h"
#include "testcase/TestCase.h"

#include "testcase/util.h"

using namespace std;
using namespace testcase;

namespace {
  ReportStream *report_stream(NULL);

  void handle_terminate()
  {
    try {
      throw;
    } catch (Assert::Error &e) {
      (*report_stream) << e.info();
    } catch (exception &e) {
      (*report_stream) << TestInfo::error(string(e.what()) + " (" + demangle(typeid(e)) + ")")
        .backtrace(sbacktrace(5,18));
    } catch (...) {
      (*report_stream) << TestInfo::error("unknown exception")
        .backtrace(sbacktrace(5,18));
    }
    abort();
  }

  void handle_sigfpe(int)
  {
    (*report_stream) << TestInfo::error("Arithmetic Error")
      .backtrace(sbacktrace(3,18));
    abort();
  }

  void handle_sigsegv(int)
  {
    (*report_stream) << TestInfo::error("Segmentation Fault")
      .backtrace(sbacktrace(3,18));
    abort();
  }
}

std::string testcase::sbacktrace(int bottom_offset, int top_offset)
{
  stringstream ss;
  void *buf[32];
  int size = backtrace((void**)&buf,32);
  char **trace = backtrace_symbols(buf,size);
  for (int i = bottom_offset; i < size - top_offset; ++i) {
    Dl_info info;
    if (dladdr(buf[i], &info) && info.dli_sname) {
      char *demangled = NULL;
      int status = -1;
      if (info.dli_sname[0] == '_') {
        demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
      }
      ss << (status ? (info.dli_sname ? info.dli_sname : trace[i]) : demangled) << endl;
    } else {
      ss << trace[i] << endl;
    }
  }
  free(trace);
  return ss.str();
}

string testcase::demangle(const type_info &info)
{
  int status;
  unique_ptr<char> realname(abi::__cxa_demangle(info.name(), 0, 0, &status));
  if (status) {
    return info.name();
  } else {
    return realname.get();
  }
}

void testcase::handler_install(ReportStream *rs)
{
  report_stream = rs;
  set_terminate(handle_terminate);
  signal(SIGFPE,handle_sigfpe);
  signal(SIGSEGV,handle_sigsegv);
}

void testcase::synchronize(const TestCase::AsyncCase &f)
{
  bool done = false;;
  mutex done_mutex, wait_mutex;
  condition_variable condition;

  f([&done,&done_mutex,&condition]{
    lock_guard<mutex> lock(done_mutex);
    done = true;
    condition.notify_one();
  });

  unique_lock<mutex> wait_lock(wait_mutex);
  condition.wait(wait_lock, [&done,&done_mutex]{
    lock_guard<mutex> lock(done_mutex);
    return done;
  });
}

int testcase::synchronize_on_fork(const std::function<void(void)> &f)
{
  int pid, info = 0;
  if ((pid = fork())) {
    waitpid(pid, &info, 0);
  } else {
    f();
    quick_exit(0);
  }
  return info;
}
