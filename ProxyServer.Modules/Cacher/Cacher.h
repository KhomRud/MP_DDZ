#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdexcept>
#include <ctime>
#include <stdio.h>

using namespace std;

struct CacheInfo
{
    string Request;
    string Answer;
    int    Time;	
};

template <typename T>
string toString(T val)
{
    ostringstream oss;
    oss << val;
    return oss.str();
}

template<typename T> 
T fromString(const std::string& s) 
{
  istringstream iss(s);
  T res;
  iss >> res;
  return res;
}

class Cache
{   
public:
    
    Cache(const char* cacheFolder, int cacheSize, int timeIntervalInMin);
    
    void Put(string request, string answer);
    
    string Get(string request);
    
    void DeleteExpiredCachedData();
    
    bool IsTimeExpired();
    
private:
    
    bool IsInCache(string request);
    bool IsCacheFull();
    
    vector <CacheInfo>  GetFullCachedData();
        
    const char* _cacheFolder;    
    int         _cacheSize;
    int         _timeIntervalInMin;    
};
