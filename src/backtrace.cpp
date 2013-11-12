#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include <cstdlib>

#include <sstream>

#include "backtrace.h"

using namespace std;

std::string sbacktrace()
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
