#ifndef STRINGSTEST_H
#define STRINGSTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Strings.h"

class StringsTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (StringsTest);
    CPPUNIT_TEST (addTest);
    CPPUNIT_TEST (subTest);
    CPPUNIT_TEST (equalTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
        void tearDown (void);

    protected:
        void addTest (void);
        void subTest (void);
        void equalTest (void);
};

#endif //STRINGSTEST_H
