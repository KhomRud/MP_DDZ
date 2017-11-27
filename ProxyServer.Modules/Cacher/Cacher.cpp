#include "Cacher.h"

#include <sys/stat.h>
#include <sys/types.h>

Cacher::Cacher(const char* cacheFolder, int cacheSize, int timeIntervalInMin)
{
    _cacheFolder = cacheFolder;
    _cacheSize   = cacheSize;
    _timeIntervalInMin = timeIntervalInMin;
    
    if(std::ifstream(cacheFolder).good())
        return;
    
    if(mkdir(cacheFolder, 0777))
    {
        perror("Не удалось создать директорию для кэша");
        exit(3);
    }
}

Cacher::~Cacher()
{
    for(std::list<CacheInfo>::iterator it = _data.begin(); it != _data.end(); ++it)
    {
        DeleteNote(it--);
        continue;
    }
}

//Add info to cache.
void Cacher::Put(const std::string& request, const std::string& answer)
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
std::string Cacher::Get(const std::string& request)
{
    std::list<CacheInfo>::iterator pos = FindInCache(request);
    
    if(pos == _data.end())
        return "";
    
    std::string filename = pos->AnswerFile;
    
    return ReadFromFile((_cacheFolder + filename).c_str());
}

std::string Cacher::ReadFromFile(const char* filename)
{
    // Открыть бинарный файл для чтения
    std::ifstream in(filename, std::ios_base::binary);

    // Проверка корректности открытия файла
    if (!in.is_open())	
    {
        perror("Ошибка открытия файла кэша");
        exit(2);
    }
    
    // http://insanecoding.blogspot.ru/2011/11/how-to-read-in-file-in-c.html
    std::string result((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    
    in.close();
    
    return result;
}

void Cacher::WriteToFile(const char* filename, const std::string& data)
{
    // Открыть бинарный файл для записи
    std::ofstream out(filename, std::ios_base::binary);

    // Проверка корректности открытия файла
    if (!out.is_open())	
    {
        perror("Не удалось создать файл для кэша");
        exit(5);
    }
    
    // Запись в бинарный файл
    out.write(data.c_str(), data.length());
    
    /*
    for(size_t i = 0; i < data; ++i)
        out.write((char *)&data[i], sizeof(char));
     */
    
    out.close();
}

std::string Cacher::GenerateNewFileName()
{
    bool success = false;
    std::string name;
    int j = 0;
    
    while(success == false)
    {
        time_t currentTime = time(NULL);
        success = true;
        
        for(int i = 20; i < 50; ++i)
        {
            char temp = 'A' + (currentTime % (i + j));
            name += temp;
        }
        
        for(std::list<CacheInfo>::iterator it = _data.begin(); it != _data.end() && success == true; ++it)
        {
            if(it->AnswerFile == name)
            {
                success = false;
                ++j;
            }
        }
    }
    
    return name;
}

std::list<CacheInfo>::iterator Cacher::FindInCache(const std::string& request)
{
    time_t currentTime = time(NULL); 
    
    for(std::list<CacheInfo>::iterator it = _data.begin(); it != _data.end(); ++it)
    {
        /* Нужно оптимизировать и тестировать итераторы как гуляют */
        
        if(currentTime - it->Time > _timeIntervalInMin * 60)
        {
            DeleteNote(it--);
            continue;
        }
        
        if(it->Request == request)
            return it;
    }
    
    return _data.end();
}

bool Cacher::IsCacheFull()
{
    return _data.size() == _cacheSize;
}

void Cacher::DeleteNote(std::list<CacheInfo>::iterator pos)
{
    std::string file = pos->AnswerFile;
    
    _data.erase(pos);
    
    remove((_cacheFolder + file).c_str());
}
    
void Cacher::DeleteEarlyNote()
{
    /* Перебором удалить самую давнюю запись */
    std::list<CacheInfo>::iterator minIt = _data.begin();
    time_t minTime = minIt->Time;

    for(std::list<CacheInfo>::iterator it = _data.begin(); it != _data.end(); ++it)
    {
        if(minTime > it->Time)
        {
            minIt = it;
            minTime = it->Time;
        }
    }
    
    DeleteNote(minIt);
}