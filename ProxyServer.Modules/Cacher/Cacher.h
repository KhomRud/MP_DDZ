#pragma once

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <sys/stat.h>

#ifndef CACHER_H
#define CACHER_H

// Информация о кеширующихся данных.
struct CacheInfo
{
    std::string Request;
    std::string AnswerFile;
    int Time;	
};
/**
 * Класс для работы с кеширумой информацией.
 */
class Cacher
{   
public:
    /**
     * 
     * @param cacheFolder           Директория для записи ответов.
     * @param cacheSize             Размер кеша.
     * @param timeIntervalInMin     Время нахождения актуальной информации в кеше.
     */
    Cacher(const char* cacheFolder, int cacheSize, int timeIntervalInMin);
    ~Cacher();
    
    /**
     * Кладем информацию в кеш.
     * @param request Запрос.
     * @param answer  Ответ.
     */
    void Put(const std::string& request, const std::string& answer);
    
    /**
     * Достаем информацию из кеша.
     * @param request Запрос.
     * @return        Возвращает ответ на заданный вопрос или пустую строку.
     */
    std::string Get(const std::string& request);
    
private:
    
    /**
     * Поиск информации в кеше.
     * @param request Запрос.
     * @return        Возвращает итератор на объект листа, с заданным запросом.
     */
    std::list<CacheInfo>::iterator FindInCache(const std::string& request);
    
    /**
     * Проверка на заполненность кеша.
     * @return 
     */
    bool IsCacheFull();
    
    /**
     * Считываем ответ из файла.
     * @param filename имя файла.
     * @return         возвращет прочитанный ответ из файла.
     */
    std::string ReadFromFile(const char* filename);
    
    /**
     * Записываем ответ в файл.
     * @param filename
     * @param data
     */
    void WriteToFile(const char* filename, const std::string& data);
    
    /**
     * Генерируем имя файла. 
     * @return Возвращает имя файла.
     */
    std::string GenerateNewFileName();
    
    /**
     * Удаляем информацию из кеша.
     * @param pos итератор на объект класса.
     */
    void DeleteNote(std::list<CacheInfo>::iterator pos);
    
    /**
     * Удаляем самую ранню запись.
     */
    void DeleteEarlyNote();
        
    const char* _cacheFolder;        // Имя директории   
    int         _cacheSize;          // Размер кеша.
    int         _timeIntervalInMin;  // Время хранения кеша.
    
    //Лист для хранения информации о кеше.
    std::list<CacheInfo> _data;
};

#endif /* NEWCACHER_H */

