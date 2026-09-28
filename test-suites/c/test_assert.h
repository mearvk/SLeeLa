#ifndef SLEELA_TEST_ASSERT_H
#define SLEELA_TEST_ASSERT_H
#include <stdio.h>
#define SLEELA_TEST_ASSERT(c) do { if (!(c)) { fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #c); return 1; } } while (0)
#define SLEELA_TEST_ASSERT_EQ_INT(e,a) do { long long ee=(long long)(e), aa=(long long)(a); if (ee!=aa) { fprintf(stderr, "FAIL %s:%d expected %lld got %lld\n", __FILE__, __LINE__, ee, aa); return 1; } } while (0)
#endif
