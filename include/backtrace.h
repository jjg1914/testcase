#ifndef H_BACKTRACE
#define H_BACKTRACE

#include <string>

std::string sbacktrace(int bottom_offset = 0, int top_offset = 0);

std::string demangle(const std::type_info &info);

#endif
