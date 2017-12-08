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
    
    std::string header = "";
    
    header += "GET ";
    header += "http://192.168.27.141/info.php\r\n";
    header += (std::string)" HTTP/1.1\r\n";
    header += (std::string)"Host: " + "http://192.168.27.141\r\n";
    header += (std::string)"User-Agent: Mozilla/5.0\r\n";
    //header += (std::string)"Accept: text/html" + "\r\n";
    header += (std::string)"Accept-Language: ru,en-us;q=0.7,en;q=0.3\r\n";
    header += (std::string)"Accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7\r\n";
    header += (std::string)"Connection: keep-alive\r\n";
    header += "\r\n";
    
    
    Sender testSender("http", "192.168.27.141", 80);
    
    std::string result = testSender.Send(header.c_str(), header.length());
    
    std::string test = "Hi, boss.";
    for(int i = 0; i < test.size(); ++i)
        CPPUNIT_ASSERT(test[i] == result[i]);
}

// Тест для google.ru
void SenderTests::SimpleTest2() {
    
    std::string header =
            "GET http://google.ru/ HTTP/1.1\\r\\n"
            "Host: google.ru\\r\\n" 
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:57.0) Gecko/20100101 Firefox/57.0\\r\\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\\r\\n"
            "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\\r\\n"
            "Accept-Encoding: gzip, deflate\\r\\nConnection: keep-alive\\r\\n"
            "Upgrade-Insecure-Requests: 1\\r\\n\\r\\n\\377\\177";
    
    Sender testSender("http", "google.ru", 80);
    
    std::string result = testSender.Send(header.c_str(), header.length());
}

// Тест для google.ru
void SenderTests::SimpleTest3() {
    
    std::string header =
            "GET http://vk.com/ HTTP/1.1"
            "Host: vk.com"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:57.0) Gecko/20100101 Firefox/57.0"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
            "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3"
            "Accept-Encoding: gzip, deflate"
            "Cookie: remixlang=0; remixlhk=7b08b09b430cb6c245; remixstid=975534942_3b8cb0572501e56ae6; remixflash=0.0.0; remixscreen_depth=24; remixdt=0"
            "Connection: keep-alive"
            "Upgrade-Insecure-Requests: 1";
    
    Sender testSender("http", "vk.com", 80);
    
    std::string result = testSender.Send(header.c_str(), header.length());
}
