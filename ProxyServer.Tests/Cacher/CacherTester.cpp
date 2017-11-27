#include "CacherTester.h"
#include "../../ProxyServer.Modules/Cacher/Cacher.h"

CPPUNIT_TEST_SUITE_REGISTRATION(CacherTester);

CacherTester::CacherTester()
{
}

CacherTester::~CacherTester()
{
}

void CacherTester::setUp()
{
}

void CacherTester::tearDown()
{
}

void CacherTester::Test1()
{
    Cacher test("testCacheDir/", 5, 5);
    
    test.Put("req1", "Boss");
    test.Put("req2", "Zombi");
    
    std::string temp = test.Get("req2");
    
    CPPUNIT_ASSERT(temp == "Zombi");
}
