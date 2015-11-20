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

#define MOCK_ID(i)\
  _mock ## i

#define MOCK(p,f)\
  _MOCK(__COUNTER__,p,f)

#define _MOCK_ID(i)\
  _mock ## i

#define _MOCK(i,p,f)\
  _Mock<i,decltype(p)> _MOCK_ID(i)((_Mock<i,decltype(p)>::Stateless) p,f)

namespace testcase {

  template<int U, typename T>
  class _Mock;

  template<int U, typename R, typename... T>
  class _Mock<U,R(T...)> {

    public:

    using Stateless = R(*)(T...);
    using Stateful = std::function<R(T...)>;

    _Mock(Stateless p, const Stateful& f);

    ~_Mock();

    private:

    Stateless addr;

    unsigned char opcode[5];

    intptr_t pagesize;

    intptr_t pageaddr;

    static R stateless(T... t);

    static Stateful stateful;
  };

  template<int U, typename R, typename C, typename... T>
  struct _Mock<U,R (C::*)(T...)>
    : public _Mock<U,R(C*,T...)> {
    using Super = _Mock<U,R(C*,T...)>;
    public:
    _Mock(typename Super::Stateless p,
      const typename Super::Stateful& f) : Super(p,f) {}
  };

  template<int U, typename R, typename C, typename... T>
  struct _Mock<U, R (C::*)(T...) const>
    : public _Mock<U, R(const C*,T...)> {
    using Super = _Mock<U,R(const C*,T...)>;
    public:
    _Mock(typename Super::Stateless p,
      const typename Super::Stateful& f) : Super(p,f) {}
  };

  template<int U, typename R, typename C, typename... T>
  struct _Mock<U, R (C::*)(T...) volatile>
    : public _Mock<U, R(volatile C*,T...)> {
    using Super = _Mock<U,R(volatile C*,T...)>;
    public:
    _Mock(typename Super::Stateless p,
      const typename Super::Stateful& f) : Super(p,f) {}
  };

  template<int U, typename R, typename C, typename... T>
  struct _Mock<U, R (C::*)(T...) const volatile>
    : public _Mock<U, R(const volatile C*,T...)> {
    using Super = _Mock<U,R(const volatile C*,T...)>;
    public:
    _Mock(typename Super::Stateless p,
      const typename Super::Stateful& f) : Super(p,f) {}
  };
}

template<int U, typename R, typename... T>
R testcase::_Mock<U,R(T...)>::stateless(T... t)
{
  return stateful(t...);
}

template<int U, typename R, typename... T>
typename testcase::_Mock<U,R(T...)>::Stateful
  testcase::_Mock<U,R(T...)>::stateful;

template<int U, typename R, typename... T>
testcase::_Mock<U,R(T...)>::_Mock(Stateless p, const Stateful& f)
  : addr(p),
    pagesize(sysconf(_SC_PAGE_SIZE)),
    pageaddr((((intptr_t) addr) / pagesize) * pagesize)
{
  stateful = f;

  unsigned char newop[5] = { 0xE9 };
  *(int32_t*)(newop + 1) = ((char*) stateless) - ((char*) addr) - 5;

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

template<int U, typename R, typename... T>
testcase::_Mock<U,R(T...)>::~_Mock()
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
