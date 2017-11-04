#include "SenderTests.h"
#include "../../Sender/Sender.h"


CPPUNIT_TEST_SUITE_REGISTRATION(SenderTests);

SenderTests::SenderTests() {
}

SenderTests::~SenderTests() {
}

void SenderTests::setUp() {
}

void SenderTests::tearDown() {
}

void SenderTests::SimpleTest1() {

    // std::string adress = "http://192.168.27.141/info.php";
    
    std::vector< std::pair< std::string, std::string> > params;
    Sender testSender("http", "192.168.27.141", "info.php", params);
    
    std::string result = testSender.SendGET();
    std::string test = "Hi, boss.";
    
    for(int i = 0; i < test.size(); ++i)
        CPPUNIT_ASSERT(test[i] == result[i]);
}

void SenderTests::SimpleTest2() {
    CPPUNIT_ASSERT(true);
}