#pragma once

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <sys/stat.h>

#ifndef CACHER_H
#define CACHER_H

struct CacheInfo
{
    std::string Request;
    std::string AnswerFile;
    int Time;	
};

class Cacher
{   
public:
    
    Cacher(const char* cacheFolder, int cacheSize, int timeIntervalInMin);
    ~Cacher();
    void Put(const std::string& request, const std::string& answer);
    std::string Get(const std::string& request);
    void DeleteExpiredCachedData();
    
private:
    std::list<CacheInfo>::iterator FindInCache(const std::string& request);
    bool IsCacheFull();
    
    std::string ReadFromFile(const char* filename);
    void WriteToFile(const char* filename, const std::string& data);
    std::string GenerateNewFileName();
    
    void DeleteNote(std::list<CacheInfo>::iterator pos);
    void DeleteEarlyNote();
        
    const char* _cacheFolder;    
    int         _cacheSize;
    int         _timeIntervalInMin;   
    std::list<CacheInfo> _data;
};

#endif /* NEWCACHER_H */

