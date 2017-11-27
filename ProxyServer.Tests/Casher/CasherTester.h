#ifndef CASHERTESTER_H
#define CASHERTESTER_H

#include <cppunit/extensions/HelperMacros.h>

class CasherTester : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(CasherTester);

    CPPUNIT_TEST(Test1);
    CPPUNIT_TEST(Test2);
    CPPUNIT_TEST(Test3);
    CPPUNIT_TEST(Test4);

    CPPUNIT_TEST_SUITE_END();

public:
    CasherTester();
    virtual ~CasherTester();
    void setUp();
    void tearDown();

private:
    void Test1();
    void Test2();  
    void Test3();
    void Test4();
};

#endif /* CASHERTESTER_H */

