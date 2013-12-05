#include <bfd.h>
#include <execinfo.h>
#include <cxxabi.h>

#include <csignal>
#include <cmath>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <memory>

#include "testcase/backtrace.h"

using namespace std;
using namespace testcase;

#define BACKTRACE_BUF_SIZE \
  (sizeof(backtrace_buf)/sizeof(backtrace_buf[0]))

namespace {
  backtrace_exception_f exception_handler;
  void* backtrace_buf[1024];

  struct BFD_info {
    bool found;
    string filename;
    string functionname;
    unsigned int line;
  };

  struct BFD {

    static void init();
    BFD(const string &fname);
    ~BFD();

    BFD_info addr_info(const string &addr);
    bfd_vma addr_vma(const string &addr);

    private:

    static void find_addr(bfd* p, asection *section, void *data);
    
    bfd* ptr_bfd;
    map<string,bfd_vma> map_syms;

    static bfd_vma pc;
    static asymbol **info_symtab;
    static bfd_boolean info_found;
    static const char* info_filename;
    static const char* info_functionname;
    static unsigned int info_line;
  };


  std::string sym2fname(const char *sym);

  std::string sym2addr(const char *sym);

  void handle_terminate();

  void handle_sigsegv(int);

  void handle_sigfpe(int);

  void handle_sigill(int);

  void handle_sigbus(int);
}

bfd_vma BFD::pc;
asymbol **BFD::info_symtab = NULL;
bfd_boolean BFD::info_found = 0;
const char* BFD::info_filename;
const char* BFD::info_functionname;
unsigned int BFD::info_line;

void BFD::init()
{
  bfd_init();
}

BFD::BFD(const string &fname)
  : ptr_bfd(bfd_openr(fname.c_str(),NULL))
{
  char **matching;
  if (ptr_bfd &&
    !bfd_check_format(ptr_bfd, bfd_archive) &&
    bfd_check_format_matches(ptr_bfd, bfd_object, &matching) &&
    bfd_get_file_flags(ptr_bfd) & HAS_SYMS) {

    unsigned int size;
    asymbol **symtab;
    long symc = bfd_read_minisymbols(ptr_bfd, 0, (void**) &symtab, &size);
    if (symc == 0) {
      symc = bfd_read_minisymbols(ptr_bfd, 1, (void**) &symtab, &size);
    }

    for (int i = 0; i < symc; ++i) {
      symbol_info info;
      bfd_symbol_info(symtab[i], &info);
      map_syms.emplace(symtab[i]->name, info.value);
    }
  }
}

BFD::~BFD()
{
  if (ptr_bfd) {
    bfd_close(ptr_bfd);
  }
}

BFD_info BFD::addr_info(const string &addr)
{
  BFD_info rval;
  pc = addr_vma(addr);
  rval.found = (info_found = 0);
  bfd_map_over_sections(ptr_bfd,BFD::find_addr, NULL);

  if (info_found) {
    rval.found = true;
    rval.filename = (info_filename ? info_filename : "");
    rval.functionname = (info_functionname ? info_functionname :
      "Anonymous Function");
    rval.line = info_line;
  } 

  return rval;
}

bfd_vma BFD::addr_vma(const string &addr)
{
  int i = addr.find_last_of('+');
  bfd_vma base = 0;
  if (i > 0) {
    string sym = addr.substr(0,i);
    auto it = map_syms.find(sym);
    if (it != map_syms.end()) {
      base = it->second;
    }
  }
  bfd_vma offset = bfd_scan_vma(addr.substr(i + 1).c_str(), NULL, 16);
  return base + offset;
}

void BFD::find_addr(bfd* p, asection *section, void *)
{
  bfd_vma vma;
  bfd_size_type size;

  if (info_found)
    return;

  if ((bfd_get_section_flags(p, section) & SEC_ALLOC) == 0)
    return;

  vma = bfd_get_section_vma(p, section);
  if (pc < vma)
    return;

  size = bfd_get_section_size(section);
  if (pc >= vma + size)
    return;

  info_found = bfd_find_nearest_line(p, section, info_symtab, pc - vma,
    &info_filename, &info_functionname, &info_line);
}

namespace {

  std::string sym2fname(const char *sym) {
    string base(sym);
    int i = base.size() - 1;
    while (base[i] != '(') {
      --i;
    }
    return base.substr(0,i);
  }

  std::string sym2addr(const char *sym) {
    string base(sym);
    int e = base.size() - 1, s;
    while (base[e] != ')') {
      --e;
    }
    s = e;
    while (base[s] != '(') {
      --s;
    }
    if (e - s == 1) {
      e = base.size() - 1;
      while (base[e] != ']') {
        --e;
      }
      s = e;
      while (base[s] != '[') {
        --s;
      }
    }
    return base.substr(s + 1,e - s - 1);
  }

  void handle_terminate()
  {
    static bool recursive_guard = 0;
    static int init_depth = 0;
    ErrorInfo info(ErrorInfo::ETERM, backtrace_depth());
    try {
      if (recursive_guard) {
        exception e;
        info.depth = init_depth;
        exception_handler(info, e);
      } else {
        recursive_guard = true;
        init_depth = info.depth;;
        throw;
      }
    } catch (exception &e) {
      info.eno = ErrorInfo::EEXP;
      exception_handler(info, e);
    } catch (...) {
      exception e;
      info.eno = ErrorInfo::EUNKNOWN;
      exception_handler(info, e);
    }
  }

  void handle_sigsegv(int)
  {
    ErrorInfo info(ErrorInfo::ESEGV, backtrace_depth());
    exception e;
    exception_handler(info, e);
  }

  void handle_sigfpe(int)
  {
    ErrorInfo info(ErrorInfo::EFPE, backtrace_depth());
    exception e;
    exception_handler(info, e);
  }

  void handle_sigill(int)
  {
    ErrorInfo info(ErrorInfo::EILL, backtrace_depth());
    exception e;
    exception_handler(info, e);
  }

  void handle_sigbus(int)
  {
    ErrorInfo info(ErrorInfo::EBUS, backtrace_depth());
    exception e;
    exception_handler(info, e);
  }
}

testcase::ErrorInfo::ErrorInfo(Errno eno, int depth)
  : eno(eno),
    depth(depth)
{}

std::string testcase::sbacktrace(int bottom, int top)
{
  stringstream ss;
  int size = backtrace((void**)&backtrace_buf,BACKTRACE_BUF_SIZE);
  char **sym = backtrace_symbols(backtrace_buf, size);

  BFD::init();
  map<string,BFD> str2bfd;

  int s = size - bottom + 1;
  int width = log10(size - top - 1 - s) + 1;
  for (int i = s; i < size - top - 1; ++i) {
    string fname(sym2fname(sym[i]));
    BFD& rbfd = str2bfd.emplace(fname, fname).first->second;
    BFD_info info = rbfd.addr_info(sym2addr(sym[i]));

    if (info.found) {
      ss << setw(width) << (i - s);
      ss << ": " << backtrace_demangle(info.functionname) << endl;
      ss << string(width + 2,' ');
      ss << "[" << info.filename << ":" << info.line << "]" << endl;
      ss << string(width + 2,' ');
      ss << fname << "(+0x" << hex << rbfd.addr_vma(sym2addr(sym[i])) << ")";
      ss << dec << endl;
    } else {
      ss << setw(width) << (i - s) << ": " << sym[i] << endl;
    }
  }
  free(sym);
  return ss.str();
}

void testcase::backtrace_exception(const std::function<void(const ErrorInfo&,
  const std::exception &e)> &f)
{
  exception_handler = f;
  set_terminate(handle_terminate);
  signal(SIGSEGV,handle_sigsegv);
  signal(SIGFPE,handle_sigfpe);
  signal(SIGILL,handle_sigill);
  signal(SIGBUS,handle_sigbus);
}

std::string testcase::backtrace_demangle(const std::string &sym)
{
  int status;
  unique_ptr<char> realname(abi::__cxa_demangle(sym.c_str(), 0, 0, &status));
  if (status) {
    return sym;
  } else {
    return realname.get();
  }
}

int testcase::backtrace_depth()
{
  return backtrace((void**)&backtrace_buf,BACKTRACE_BUF_SIZE) - 1;
}

void testcase::backtrace_trap()
{
  raise(SIGTRAP);
}
