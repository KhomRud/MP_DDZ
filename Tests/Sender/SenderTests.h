/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   SenderTests.h
 * Author: user
 *
 * Created on Nov 4, 2017, 1:59:41 AM
 */

#ifndef SENDERTESTS_H
#define SENDERTESTS_H

#include <cppunit/extensions/HelperMacros.h>

class SenderTests : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(SenderTests);

    CPPUNIT_TEST(SimpleTest1);
    CPPUNIT_TEST(SimpleTest2);

    CPPUNIT_TEST_SUITE_END();

public:
    SenderTests();
    virtual ~SenderTests();
    void setUp();
    void tearDown();

private:
    void SimpleTest1();
    void SimpleTest2();
};

#endif /* SENDERTESTS_H */

