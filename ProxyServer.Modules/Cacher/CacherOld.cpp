#include "CacherOld.h"
#include <time.h>

//Check is file exist?
bool FileExists(const char *fileName)
{
    ifstream ifile(fileName);
    return (bool)ifile;
}

Cache::Cache(const char * cacheFolder, int cacheSize, int timeIntervalInMin)
{
    _cacheFolder = cacheFolder;
    _cacheSize   = cacheSize;
    _timeIntervalInMin = timeIntervalInMin;
}

//Add info to cache.
void Cache::Put(string request, string answer)
{    
    time_t currentTime = time(NULL); 
    
    if (!Cache::IsInCache(request) && !Cache::IsCacheFull())
    {
        
        string postTime = toString(currentTime);        
        
        string strForFile = "Request:"  + toString(request.length()) + ':' + request +
                            "Answer:"   + toString(answer.length())  + ':' + answer  +
                            "Time:"     + toString(postTime.length())    + ':' + postTime + '\n';

        ofstream casheFile;

        casheFile.open(_cacheFolder, ios::app | ios::in);
        casheFile << strForFile;

        casheFile.close();
    }
}

//Loading data from cache.
string Cache::Get(string request)
{
//     size_t end = request.find("\r\n");
//    
//    request = request.substr(0, end); 
    string answerByRequest = "";
    
    vector <CacheInfo> structVector = Cache::GetFullCachedData();
    
    size_t structCount = structVector.size();
    for (size_t i = 0; i < structCount; ++i)
    {
//        end = structVector[i].Request.find("\r\n");
//   
//        string cacheRequest = structVector[i].Request.substr(0, end); 
//
//        size_t begin = structVector[i].Request.find("GET");
//        
//        cacheRequest = cacheRequest.substr(begin, end);
//
//        if (cacheRequest == request)
//            answerByRequest = structVector[i].Answer;
        
        if (structVector[i].Request == request)
            answerByRequest = structVector[i].Answer;
    }
        
    return answerByRequest;
}

//Reading structure form cache.
vector <CacheInfo> Cache::GetFullCachedData()
{
    CacheInfo cacheData;
    ifstream casheFile;
    casheFile.open(_cacheFolder, ios::out);

    vector <CacheInfo> structVector;
    char buf[100];

    while (!casheFile.eof())
    {
        casheFile.getline(buf, 100, ':');

        if (casheFile.eof())
            break;

        casheFile.getline(buf, 100, ':');
        int size = atoi(buf);
        char* request = new char[size + 1];
        request[size] = 0;
        casheFile.read(request, size);

        casheFile.getline(buf, 100, ':');
        casheFile.getline(buf, 100, ':');
        size = atoi(buf);
        cout << "Answer size = " << size << endl;
        
        char* answer = new char[size + 1];
        answer[size] = 0;
        casheFile.read(answer, size);

        casheFile.getline(buf, 100, ':');
        casheFile.getline(buf, 100, ':');
        size = atoi(buf);
        char* time = new char[size + 1];
        time[size] = 0;
        casheFile.read(time, size);
        casheFile.get();

        cacheData.Request = request;
        cacheData.Answer = answer;
        cacheData.Time = atoi(time);

        structVector.push_back(cacheData);


        delete request;
        delete answer;
        delete time;
    }

    casheFile.close();

    return structVector;
}

//Checking being request in the cache already.
bool Cache::IsInCache(string request)
{
    bool isInCache = false;
//    cout << "Request before : " << request << endl;
//    size_t end = request.find("\r\n");
//    
//    request = request.substr(0, end); 
//    
//    cout << "Request after : " << request << endl;
    
    if (!FileExists(_cacheFolder))
        return false;
    
    vector <CacheInfo> structVector = Cache::GetFullCachedData();

    size_t structCount = structVector.size();
       
    for (size_t i = 0; i < structCount; ++i)
    {
//        end = structVector[i].Request.find("\r\n");
//   
//        string cacheRequest = structVector[i].Request.substr(0, end); 
//   cout << "cacheRequest before : " << cacheRequest << endl;     
//        size_t begin = structVector[i].Request.find("GET");
//        
//        cacheRequest = cacheRequest.substr(begin, end);
//         cout << "cacheRequest after : " << cacheRequest << endl;
//        if (cacheRequest == request)
//            return true;
            
        if (structVector[i].Request == request)
            isInCache = true;
    }
       
    return false;
}

//Checking is data quantity more then cache size.
bool Cache::IsCacheFull()
{
    if (!FileExists(_cacheFolder))
        return false;
    
    vector <CacheInfo> structVector = Cache::GetFullCachedData();
    
    size_t structCount = structVector.size();
    
    return (structVector.size() >= _cacheSize);
}

void Cache::DeleteExpiredCachedData()
{
        
    time_t currentTime = time(NULL); 
    bool isExpired = false;
    vector <CacheInfo> dataVector = Cache::GetFullCachedData();
    
    size_t requestCount = dataVector.size();
    
    for (size_t i = 0; i < requestCount; ++i)
    {
        if (currentTime - dataVector[i].Time >= _timeIntervalInMin * 60)
        {
            isExpired = true;
            dataVector.erase(dataVector.begin() + i);
        }
    }
    
    //If time is in interval.
    if(!isExpired)
        return;       
    
    //If file doesn't exists.
    if(remove(_cacheFolder) != 0)
        return;
    
    requestCount = dataVector.size();
    
    if (isExpired)
    {
        string finalData;
        
        for(size_t i = 0; i < requestCount; ++i)
        {           
            string oldTime = toString(dataVector[i].Time);
            string strForFile = "Request:"  + toString(dataVector[i].Request.length())  + ':' + dataVector[i].Request  +
                                "Answer:"   + toString(dataVector[i].Answer.length())   + ':' + dataVector[i].Answer   +
                                "Time:"     + toString(oldTime.length()) + ':' + oldTime + '\n';
            finalData += strForFile;
        }
            ofstream casheFile;

            casheFile.open(_cacheFolder, ios::app | ios::in);
            casheFile << finalData;

            casheFile.close();
    }
    
}