#include "NewCacher.h"


NewCacher::NewCacher(const char * cacheFolder, int cacheSize, int timeIntervalInMin)
{
    _cacheFolder = cacheFolder;
    _cacheSize   = cacheSize;
    _timeIntervalInMin = timeIntervalInMin;
}

//Add info to cache.
void NewCacher::Put(std::string& request, std::string& answer)
{    
    std::list<CacheInfo>::iterator pos = FindInCache(request);
    
    if(pos != _data.end())
        DeleteNote(pos);

    if(IsCacheFull())
        DeleteEarlyNote();
    
    CacheInfo note;

    note.Request = request;
    note.AnswerFile = GenerateNewFileName();
    note.Time = time(NULL);
            
    _data.push_back(note);
    
    WriteToFile((_cacheFolder + note.AnswerFile).c_str(), answer);
}

//Loading data from cache.
std::string NewCacher::Get(std::string& request)
{
    std::list<CacheInfo>::iterator pos = FindInCache(request);
    
    if(pos == _data.end())
        return "";
    
    std::string filename = pos->AnswerFile;
    
    return ReadFromFile(filename.c_str());
}

std::string NewCacher::ReadFromFile(const char* filename)
{
    return "Hi";
    /* побайтово */
}

void NewCacher::WriteToFile(const char* filename, std::string& data)
{
    /* побайтово */
}

std::string NewCacher::GenerateNewFileName()
{
    return "Random" + time(NULL);
}

std::list<CacheInfo>::iterator NewCacher::FindInCache(std::string& request)
{
    time_t currentTime = time(NULL); 
    
    for(std::list<CacheInfo>::iterator it = _data.begin(); it != _data.end(); ++it)
    {
        /* Нужно оптимизировать и тестировать итераторы как гуляют */
        
        if(currentTime - it->Time > _timeIntervalInMin)
            DeleteNote(it);
            
        if(it->Request == request)
            return it;
    }
    
    return _data.end();
}

bool NewCacher::IsCacheFull()
{
    /* какая-то реалзация */
    return false;
}

void NewCacher::DeleteNote(std::list<CacheInfo>::iterator pos)
{
    std::string file = pos->AnswerFile;
    
    _data.erase(pos);
    
    /* И удалить файл с именем _cacheFolder + file */
}
    
void NewCacher::DeleteEarlyNote()
{
    /* Перебором удалить самую давнюю запись */
}