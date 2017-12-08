#pragma once

#include <fstream>
#include <sstream>
#include <string.h>
#include <stdlib.h>

#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

struct ConfigurationData
{
    int CacheRelevanceTime;
    int CacheSize;
    std::string Host;
    int Port;
};


/** 
 * Загружает конфигурационные опции
 */
class Configurator {
public:
    /**
     * Прочесть конфигурационные опции
     * @param header        Строка, содержащая имя файлв
     * @return              Структура с конфигурационными опциями
     */
    static ConfigurationData Read(const char *filename);
};

#endif /* CONFIGURATOR_H */

