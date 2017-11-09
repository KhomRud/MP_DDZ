#ifndef CACHER_H
#define CACHER_H

#endif /* CACHER_H */

#define casherFolder "./CachingData.txt"

#include <vector>
#include <time.h>
#include <fstream>
#include "regex.h"

using namespace std;


struct cacheInfo
{
    string _request;
    string _time;
    string _answer;              
};

class Cacher
{
    
private:
    cacheInfo _cacheData;
    
public:
    
    Cacher(std::string request, string asnwer, string cachedTime = 0)
    {    
        _cacheData._request = request;
        _cacheData._time = cachedTime;
        _cacheData._answer = asnwer;        
    }
    
    void AddToCache()
    {
        ofstream casheFile;
        casheFile.open(casherFolder,ios::app);
        string finalStr = "{\n" + _cacheData._request + "\n" + _cacheData._time + "\n" + _cacheData._answer + "\n}\n\n";
        casheFile << finalStr;
        casheFile.close();
    }
    
    cacheInfo * findAllStructs ()
    {
               string str;
	ifstream casheFile;
	casheFile.open (casherFolder);
        
        while(!casheFile.eof)
        {
	        getline(casheFile,str); // Saves the line in STRING.
	        cout<<str; // Prints our STRING.
        }
        
        
        int structsCount = 0;
        
        for (size_t pos = 0; pos < str.size(); ++pos )
        {
          pos = str.find('{', pos);
          if (pos != std::string::npos)
          {
            ++structsCount;
          }
          else
          {
            break;
          }
        }
        
        cacheInfo * infoStructs = new cacheInfo[structsCount];
        

        for(size_t pos = 0; pos < str.size(); ++pos)
        {
            pos[0] = str.find('\n', pos);
            if (pos != std::string::npos)
            {
              ++structsCount;
            }
            else
            {
              break;
            }
        }
        
        #f
        for(size_t i = 0; i < structsCount; ++i)
        {
            pos = str.find('\n', pos);
            
            infoStructs[i]._request = 
            infoStructs[i]._time    = 
            infoStructs[i]._answer  =
        }
        
        if ()
        {
            
        }
        
        return ;
        
    }
    
    
    bool IsInCache()
    {
        cacheInfo* dataStructsArray  = findAllStructs ();
        return 0;
    }
    
};






//
//
//
//
//
//// Stores the cache results, can be inherited from or held as a member.
//class ResultCache
//{
//public:
//  template <class T> void AddToCache(const T & value, const std::string & name)
//  {
//    // Search if it is there
//    for (std::shared_ptr<CacheInterface> & cache : _cached_items)
//    {
//      if (cache->GetName() == name)
//      {
//        shared_ptr<CachedValue<T>> cache_value (boost::reinterpret_pointer_cast<CachedValue<T>>(cache));
//        cache_value->_value = value;
//        return;
//      }
//    }
//
//    // Make a new cache value and add it
//    shared_ptr<CacheInterface> cached_item(new CachedValue<T>(value, name));
//    _cached_items.push_back(cached_item);
//  }
//
//  // Returns true if cache exists for a name
//  bool IsInCache(const std::string & name)
//  {
//    for (shared_ptr<CacheInterface> & cache : _cached_items)
//    {
//      if (cache->GetName() == name)
//      {
//        return true;
//      }
//    }
//    return false;
//  }
//
//  // Reads a value from the cache
//  template <class T> void ReadFromCache(T & output, const std::string & name)
//  {
//    for (shared_ptr<CacheInterface> & cache : _cached_items)
//    {
//      if (cache->GetName() == name)
//      {
//        // Is reinterpret cast safe?
//        shared_ptr<CachedValue<T>> cache_value (boost::reinterpret_pointer_cast<CachedValue<T>>(cache));
//        output = cache_value->_value;
//      }
//    }
//  }

  // Clears the cache values
//  void ClearCache()
//  {
//    _cached_items.clear();
//  }
//
//  // Clear cache on copy and assign?
//
//
//private:
//  vector<int> _cached_items;
//};
//
//// Macros simplified use in code.
//#define IS_IN_CACHE(x) IsInCache(#x)
//#define READ_FROM_CACHE(x) ReadFromCache(x, #x)
//#define CACHE_RESULT(x) AddToCache(x, #x)