#ifndef H_SYNCHRONIZE
#define H_SYNCHRONIZE

#include "TestCase.h"

void synchronize(const TestCase::AsyncCase &f);

int synchronize_on_fork(const std::function<void(void)> &f);

#endif
