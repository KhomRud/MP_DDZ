/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   CacherTester.h
 * Author: user
 *
 * Created on Nov 27, 2017, 12:27:13 PM
 */

#ifndef CACHERTESTER_H
#define CACHERTESTER_H

#include <cppunit/extensions/HelperMacros.h>

class CacherTester : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(CacherTester);

    CPPUNIT_TEST(Test1);

    CPPUNIT_TEST_SUITE_END();

public:
    CacherTester();
    virtual ~CacherTester();
    void setUp();
    void tearDown();

private:
    void Test1();
};

#endif /* CACHERTESTER_H */

