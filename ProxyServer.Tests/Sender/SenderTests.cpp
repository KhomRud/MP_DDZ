#include "SenderTests.h"
#include "../../ProxyServer.Modules/Sender/Sender.h"


CPPUNIT_TEST_SUITE_REGISTRATION(SenderTests);

SenderTests::SenderTests() {
}

SenderTests::~SenderTests() {
}

void SenderTests::setUp() {
}

void SenderTests::tearDown() {
}

// Тест для http://192.168.27.141/info.php
void SenderTests::SimpleTest1() {
    
    std::vector< std::pair< std::string, std::string> > params;
    
    Sender testSender("http", "192.168.27.141", "80");
    
    std::string result = testSender.SendGET("info.php", params);
    
    std::string test = "Hi, boss.";
    for(int i = 0; i < test.size(); ++i)
        CPPUNIT_ASSERT(test[i] == result[i]);
}

void SenderTests::SimpleTest2() {
    CPPUNIT_ASSERT(true);
}