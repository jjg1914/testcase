#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include <cstdlib>

#include <sstream>
#include <memory>

#include "backtrace.h"

using namespace std;

std::string sbacktrace(int bottom_offset, int top_offset)
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

string demangle(const type_info &info)
{
  int status;
  unique_ptr<char> realname(abi::__cxa_demangle(info.name(), 0, 0, &status));
  if (status) {
    return info.name();
  } else {
    return realname.get();
  }
}
