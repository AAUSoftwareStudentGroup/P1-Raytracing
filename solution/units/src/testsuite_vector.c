#include "testsuite_vector.h"

CuSuite* suite_vector() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_vector_add);
    return suite;
}

void test_vector_add(CuTest *tc) {
    Vector a, b;
    a.x =  2; a.y = 3; a.z = 7;
    b.x = -2; b.y = 4; b.z = 2;

    Vector actual = vector_add(a, b);
    Vector expected = {0,7,9};

    CuAssertDblEquals(tc, expected.x, actual.x, 1e-6);
    CuAssertDblEquals(tc, expected.y, actual.y, 1e-6);
    CuAssertDblEquals(tc, expected.z, actual.z, 1e-6);
}