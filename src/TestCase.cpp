#include <unistd.h>
#include <sys/wait.h>

#include <mutex>
#include <condition_variable>

#include "TestCase.h"

using namespace std;

namespace {
  void async(const TestCase::AsyncCase &f) {
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

  void terminate_handler() {
    abort();
  }

  void test(const TestCase::AsyncCase &f) {
    int pid, info = 0;
    if ((pid = fork())) {
      waitpid(pid, &info, 0);
    } else {
      set_terminate(terminate_handler);
      async(f);
      quick_exit(0);
    }
  }
}

TestCase::TestCase(const TestCase::AsyncCase &f)
{
  test(f);
}

TestCase::TestCase(const TestCase::SyncCase &f)
{
  test([&f](const Callback &cb){ f(), cb(); });
}
