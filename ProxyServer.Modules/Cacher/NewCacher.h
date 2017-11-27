#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdexcept>
#include <ctime>
#include <stdio.h>
#include <time.h>
#include <list>

#ifndef NEWCACHER_H
#define NEWCACHER_H

struct CacheInfo
{
    std::string Request;
    std::string AnswerFile;
    int Time;	
};
/*
template <typename T>
std::string toString(T val)
{
    std::ostringstream oss;
    oss << val;
    return oss.str();
}

template<typename T> 
T fromString(const std::string& s) 
{
  std::istringstream iss(s);
  T res;
  iss >> res;
  return res;
}
*/
class NewCacher
{   
public:
    
    NewCacher(const char* cacheFolder, int cacheSize, int timeIntervalInMin);
    void Put(std::string& request, std::string& answer);
    std::string Get(std::string& request);
    void DeleteExpiredCachedData();
    
private:
    std::list<CacheInfo>::iterator FindInCache(std::string& request);
    bool IsCacheFull();
    
    std::string ReadFromFile(const char* filename);
    void WriteToFile(const char* filename, std::string& data);
    std::string GenerateNewFileName();
    
    void DeleteNote(std::list<CacheInfo>::iterator pos);
    void DeleteEarlyNote();
        
    const char* _cacheFolder;    
    int         _cacheSize;
    int         _timeIntervalInMin;   
    std::list<CacheInfo> _data;
};

#endif /* NEWCACHER_H */

