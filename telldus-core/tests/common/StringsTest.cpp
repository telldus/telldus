#include "StringsTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (StringsTest);

void StringsTest :: setUp (void)
{
}

void StringsTest :: tearDown (void)
{
}

void StringsTest :: addTest (void)
{
    // check subtraction results
    CPPUNIT_ASSERT_EQUAL (1, 1);
    CPPUNIT_ASSERT_EQUAL (1, 2);
}

void StringsTest :: subTest (void)
{
    // check addition results
    CPPUNIT_ASSERT_EQUAL (1, 1);
}


void StringsTest :: equalTest (void)
{
    // test successful, if true is returned
    CPPUNIT_ASSERT (1 == 1);
}
