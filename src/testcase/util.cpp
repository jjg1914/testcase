#include <dlfcn.h>
#include <cxxabi.h>
#include <bfd.h>
#include <cstdlib>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <memory>
#include <thread>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <map>

#include "testcase/Assert.h"
#include "testcase/TestCase.h"
#include "testcase/backtrace.h"

#include "testcase/util.h"

using namespace std;
using namespace testcase;

namespace {
  int backtrace_top = 0;
}

string testcase::demangle(const type_info &info)
{
  return backtrace_demangle(info.name());
}

void testcase::handler_install(ReportStream &rs, int timeout)
{
  backtrace_exception([&rs](const ErrorInfo& info, const exception& e){
    stringstream ss;
    switch (info.eno) {
    case ErrorInfo::ESEGV:
      ss << "Segmenation Fault";
      break;
    case ErrorInfo::EFPE:
      ss << "Invalid Arithmetic Operation";
      break;
    case ErrorInfo::EILL:
      ss << "Illegal Instruction";
      break;
    case ErrorInfo::EBUS:
      ss << "Segmenation Fault";
      break;
    case ErrorInfo::ETERM:
      ss << "Terminate";
      break;
    case ErrorInfo::EEXP:
      {
        const Assert::Error* a = dynamic_cast<const Assert::Error*>(&e);
        if (a) {
          rs << a->info();
          quick_exit(EXIT_FAILURE);
        }
      }
      ss << "Uncaugh Exception: " << e.what() << " (" << demangle(typeid(e)) << ")";
      break;
    case ErrorInfo::EUNKNOWN:
      ss << "Unknown Exception";
      break;
    };
    rs << TestInfo::error(ss.str()).backtrace(sbacktrace(info.depth,backtrace_top));
    quick_exit(EXIT_FAILURE);
  });
  if (timeout) {
    thread t([&rs,timeout]{
      this_thread::sleep_for(chrono::milliseconds(timeout));
      rs << TestInfo::timeout();
      quick_exit(EXIT_FAILURE);
    });
    t.detach();
  }
}

bool testcase::pdiff(const vector<int> &d)
{
  return all_of(d.begin(),d.end(),[](int i){ return i == 0; });
}

void testcase::synchronize(const TestCase::AsyncCase &f)
{
  backtrace_top = backtrace_depth();
  bool done = false;
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
