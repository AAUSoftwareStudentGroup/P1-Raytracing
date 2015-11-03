#ifndef _TEST_VEC_
#define _TEST_VEC_

#include "CuTest.h"
#include "vec.h"

CuSuite* suite_vec();
void test_new_vec(CuTest *tc);
void test_free_vec(CuTest *tc);
void test_vec_add_to(CuTest *tc);
void test_vec_add(CuTest *tc);

#endif