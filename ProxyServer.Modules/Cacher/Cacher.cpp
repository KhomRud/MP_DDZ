#include "Cacher.h"

Cacher::Cacher(const char* cacheFolder, int cacheSize, int timeIntervalInMin)
{
    _cacheFolder = cacheFolder;
    _cacheSize   = cacheSize;
    _timeIntervalInMin = timeIntervalInMin;
    
    //Проверка на созданность.
    if(std::ifstream(cacheFolder).good())
        return;
    
    //Создаем директорию.
    if(mkdir(cacheFolder, 0777))
    {
        perror("Не удалось создать директорию для кэша");
        exit(3);
    }
}

Cacher::~Cacher()
{
    // Производим удаление закешированных данных.
    for(std::list<CacheInfo>::iterator it = _data.begin(); it != _data.end(); ++it)
    {
        DeleteNote(it--);
        continue;
    }
}

void Cacher::Put(const std::string& request, const std::string& answer)
{    
       
    std::list<CacheInfo>::iterator pos = FindInCache(request);
    
    //Если найдено в кеше, то отчистить объект листа.
    if(pos != _data.end())
        DeleteNote(pos);

    // Если кеш переполнен, удалить самую ранню запись.
    if(IsCacheFull())
        DeleteEarlyNote();
    
    CacheInfo note;

    //Заполняем объект листа данным
    note.Request = request;
      
    // Генерируем имя файла для ответа.
    note.AnswerFile = GenerateNewFileName();
    note.Time = time(NULL);
    
    _data.push_back(note);
    
    //Создаем и записываем ответ в файл.
    WriteToFile((_cacheFolder + note.AnswerFile).c_str(), answer);
}

std::string Cacher::Get(const std::string& request)
{
    //
    std::list<CacheInfo>::iterator pos = FindInCache(request);
    
    //Если ничего не найдено по заданному запросу
    if(pos == _data.end())
        
        // Выводим пустую строку, обрабатывается далее.
        return "";
    
    std::string filename = pos->AnswerFile;
    
    //Считываем информацию из файла.
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
    
    // Запись данных в результат
    std::string result((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    
    in.close();
    
    //Возвращаем ответ.
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
    
    out.close();
}

std::string Cacher::GenerateNewFileName()
{
    
    bool success = false; // информация о существовании данного имени.
    std::string name; // Имя файла.
    int j = 0;
    
    
    // Формирование имени файла.
    while(success == false)
    {
        time_t currentTime = time(NULL);
        success = true;
        
        //Генерируем имя по заданному алгоритму.
        for(int i = 20; i < 50; ++i)
        {
            char temp = 'A' + (currentTime % (i + j));
            name += temp;
        }
        
        //Проверяем на имя на эксклюзивность.
        for(std::list<CacheInfo>::iterator it = _data.begin(); it != _data.end() && success == true; ++it)
        {
            if(it->AnswerFile == name)
            {
                success = false;
                ++j;
            }
        }
    }
    
    // Возвращаем имя файла.
    return name;
}

std::list<CacheInfo>::iterator Cacher::FindInCache(const std::string& request)
{
    time_t currentTime = time(NULL); // Текущее время.
    
    //Поиск данных в листе.
    for(std::list<CacheInfo>::iterator it = _data.begin(); it != _data.end(); ++it)
    {
        
        //Проверка на время хранения в кеше
        if(currentTime - it->Time > _timeIntervalInMin * 60)
        {
            DeleteNote(it--);
            continue;
        }
        
        //Проверка на совпадение.
        if(it->Request == request)
            
            //Возвращаем итератор на найденный объект листа.
            return it;
    }
    
    //Возвращаем фиктивный итератор, если не найдено.
    return _data.end();
}

bool Cacher::IsCacheFull()
{
    //Проверка на размер кеша.
    return _data.size() == _cacheSize;
}

void Cacher::DeleteNote(std::list<CacheInfo>::iterator pos)
{
    
    std::string file = pos->AnswerFile; // Имя файла с ответом.
    
    // Удаление объекта листа.
    _data.erase(pos);
    
    // Удаление файла с закешированной информацией.
    remove((_cacheFolder + file).c_str());
}
    
void Cacher::DeleteEarlyNote()
{
      
    std::list<CacheInfo>::iterator minIt = _data.begin();
    time_t minTime = minIt->Time;

    //Находим самую ранню запись перебором.
    for(std::list<CacheInfo>::iterator it = _data.begin(); it != _data.end(); ++it)
    {
        if(minTime > it->Time)
        {
            minIt = it;
            minTime = it->Time;
        }
    }
    
    // Удаляем самую ранню запись.
    DeleteNote(minIt);
}