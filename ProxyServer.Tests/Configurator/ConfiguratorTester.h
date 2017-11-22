#ifndef CONFIGURATORTESTER_H
#define CONFIGURATORTESTER_H

#include <cppunit/extensions/HelperMacros.h>

class ConfiguratorTester : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(ConfiguratorTester);

    CPPUNIT_TEST(SimpleTest);

    CPPUNIT_TEST_SUITE_END();

public:
    ConfiguratorTester();
    virtual ~ConfiguratorTester();
    void setUp();
    void tearDown();

private:
    void SimpleTest();
};

#endif /* CONFIGURATORTESTER_H */

