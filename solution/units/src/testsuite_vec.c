#include "testsuite_vec.h"

CuSuite* suite_vec() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_new_vec);
    SUITE_ADD_TEST(suite, test_free_vec);
    SUITE_ADD_TEST(suite, test_vec_add_to);
    SUITE_ADD_TEST(suite, test_vec_add);
    return suite;
}

void test_new_vec(CuTest *tc) {
    vec *v = new_vec();
    v->x =  2; v->y = 3; v->z = 7;

    int actual = free_vec(v);
    int expected = 1;
    CuAssertIntEquals(tc, expected, actual);
}

void test_free_vec(CuTest *tc) {

}

void test_vec_add_to(CuTest *tc) {
    vec *a, *b;
    a = new_vec();
    b = new_vec();
    a->x =  2; a->y = 3; a->z = 7;
    b->x = -2; b->y = 4; b->z = 2;

    int actual = vec_add_to(a, b);
    int expected = 1;

    CuAssertIntEquals(tc, expected, actual);
    CuAssertDblEquals(tc, 0, a->x, 1e-6);
    CuAssertDblEquals(tc, 7, a->y, 1e-6);
    CuAssertDblEquals(tc, 9, a->z, 1e-6);
}

void test_vec_add(CuTest *tc) {
    vec *a, *b, *c;
    a = new_vec();
    b = new_vec();
    c = new_vec();
    a->x =  2; a->y = 3; a->z = 7;
    b->x = -2; b->y = 4; b->z = 2;

    int actual = vec_add(c, a, b);
    int expected = 1;

    CuAssertIntEquals(tc, expected, actual);
    CuAssertDblEquals(tc, 0, c->x, 1e-6);
    CuAssertDblEquals(tc, 7, c->y, 1e-6);
    CuAssertDblEquals(tc, 9, c->z, 1e-6);
}