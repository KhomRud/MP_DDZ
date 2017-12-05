#include "ConfiguratorTester.h"

#include "../../ProxyServer.Modules/Configurator/Configurator.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ConfiguratorTester);

ConfiguratorTester::ConfiguratorTester()
{
}

ConfiguratorTester::~ConfiguratorTester()
{
}

void ConfiguratorTester::setUp()
{
}

void ConfiguratorTester::tearDown()
{
}

void ConfiguratorTester::SimpleTest()
{
    ConfigurationData data = Configurator::Read("/home/user/DDZ/MP_DDZ/ProxyServer.Tests/Configurator/Test.txt");
    
    CPPUNIT_ASSERT(data.CacheRelevanceTime == 300);
    CPPUNIT_ASSERT(data.Host == "192.168.0.14");
}
