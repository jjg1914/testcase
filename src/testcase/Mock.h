#ifndef H_MOCK
#define H_MOCK

#include <unistd.h>
#include <sys/mman.h>

#include <cstdint>
#include <cstring>
#include <cerrno>

#include <functional>
#include <system_error>
#include <type_traits>

#define MOCK(p,f)\
  _MOCK(__COUNTER__,p,f)

#define _MOCK(i,p,f)\
  _Mock<i,decltype(p)> _mock ## i(p,f)

namespace testcase {

  template<int U,typename T>
  struct _MockInfo;

  template<int U, typename R, typename... T>
  struct _MockInfo<U,R(T...)>{
    using Stateless = R(*)(T...);
    using Stateful = R(T...);
    using Source = Stateless;

    static R stateless(T... t);
    static std::function<Stateful> stateful;
  };

  template<int U, typename R, typename C, typename... T>
  struct _MockInfo<U,R (C::*)(T...)> : public _MockInfo<U,R(C*,T...)> {
    using Source = R(C::*)(T...);
  };

  template<int U, typename R, typename C, typename... T>
  struct _MockInfo<U,R (C::*)(T...) const> : public _MockInfo<U,R(const C*,T...)> {
    using Source = R(C::*)(T...) const;
  };

  template<int U, typename R, typename C, typename... T>
  struct _MockInfo<U,R (C::*)(T...) volatile> : public _MockInfo<U,R(volatile C*,T...)> {
    using Source = R(C::*)(T...) volatile;
  };

  template<int U, typename R, typename C, typename... T>
  struct _MockInfo<U,R (C::*)(T...) const volatile> : public _MockInfo<U,R(const volatile C*,T...)> {
    using Source = R(C::*)(T...) const volatile;
  };

  template<int U, typename T>
  class _Mock {

    public:

    using Stateless = typename _MockInfo<U,T>::Stateless;
    using Stateful = typename _MockInfo<U,T>::Stateful;
    using Source = typename _MockInfo<U,T>::Source;

    _Mock(Source p, const std::function<Stateful>& f);

    ~_Mock();

    private:

    Stateless addr;

    unsigned char opcode[5];

    intptr_t pagesize;

    intptr_t pageaddr;
  };
}

template<int U, typename R, typename... T>
R testcase::_MockInfo<U,R(T...)>::stateless(T... t)
{
  return stateful(t...);
}

template<int U, typename R, typename... T>
std::function<typename testcase::_MockInfo<U,R(T...)>::Stateful>
  testcase::_MockInfo<U,R(T...)>::stateful;

template<int U, typename T>
testcase::_Mock<U,T>::_Mock(Source p, const std::function<Stateful>& f)
  : addr((Stateless) p),
    pagesize(sysconf(_SC_PAGE_SIZE)),
    pageaddr((((intptr_t) addr) / pagesize) * pagesize)
{
  _MockInfo<U,T>::stateful = f;

  unsigned char newop[5] = { 0xE9 };
  *(int32_t*)(newop + 1) = ((char*) _MockInfo<U,T>::stateless) - ((char*) addr) - 5;

  if (mprotect((void*) pageaddr, pagesize,
    PROT_READ | PROT_WRITE | PROT_EXEC)) {

    throw std::system_error(errno, std::generic_category());
  } else {
    memcpy(opcode,(void*) addr, sizeof(opcode));
    memcpy((void*) addr, newop, sizeof(newop));
    if (mprotect((void*) pageaddr, pagesize, PROT_READ | PROT_EXEC)) {
      throw std::system_error(errno, std::generic_category());
    }
  }
}

template<int U, typename T>
testcase::_Mock<U,T>::~_Mock()
{
  if (mprotect((void*) pageaddr, pagesize,
    PROT_READ | PROT_WRITE | PROT_EXEC)) {

    throw std::system_error(errno, std::generic_category());
  } else {
    memcpy((void*) addr, opcode, sizeof(opcode));
    if (mprotect((void*) pageaddr, pagesize, PROT_READ | PROT_EXEC)) {
      throw std::system_error(errno, std::generic_category());
    }
  }
}

#endif
