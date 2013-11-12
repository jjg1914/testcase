#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>

#include <mutex>
#include <condition_variable>

#include "ReportStream.h"
#include "Assert.h"

#include "TestCase.h"

using namespace std;

namespace {
  ReportStream report_stream;

  void async(const TestCase::AsyncCase &f);

  string backtrace_str()
  {
    stringstream ss;
    void *buf[32];
    int size = backtrace((void**)&buf,32);
    char **trace = backtrace_symbols(buf,size);
    for (int i = 0; i < size; ++i) {
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

  void async(const TestCase::AsyncCase &f)
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

  void terminate_handler()
  {
    try {
      throw;
    } catch (Assert::Error &e) {
      report_stream << e.info().status(TestInfo::FAILED);
    } catch (exception &e) {
      report_stream << TestInfo().status(TestInfo::ERROR).what(string("Exception: ") + e.what() + string(" (") +  typeid(e).name() + string(")")).backtrace(backtrace_str());
    }
    abort();
  }

  void sigfpe_handler(int)
  {
    report_stream << TestInfo().status(TestInfo::ERROR).what("Error: Segmentation Fault").backtrace(backtrace_str());
    abort();
  }

  void sigsegv_handler(int)
  {
    report_stream << TestInfo().status(TestInfo::ERROR).what("Error: Arithmetic Error").backtrace(backtrace_str());
    abort();
  }

  TestInfo test(const TestCase::AsyncCase &f)
  {
    TestInfo rval;
    int pid;
    if ((pid = fork())) {
      int info = 0;
      waitpid(pid, &info, 0);
      report_stream >> rval;
    } else {
      set_terminate(terminate_handler);
      signal(SIGFPE,sigfpe_handler);
      signal(SIGSEGV,sigsegv_handler);
      async(f);
      report_stream << rval.status(TestInfo::PASSED);
      quick_exit(0);
    }
    return rval;
  }
}

TestCase::TestCase(const TestCase::AsyncCase &f)
  : result_val(test(f))
{}

TestCase::TestCase(const TestCase::SyncCase &f)
  : result_val(test([&f](const Callback &cb){ f(), cb(); }))
{
}

const TestInfo &TestCase::result() const
{
  return result_val;
}
