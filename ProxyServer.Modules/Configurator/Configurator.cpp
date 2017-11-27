#include "Configurator.h"

template<typename T1, typename T2>
void Add(T1& field, T2 value)
{
    std::stringstream ss;
    ss << value;
    ss >> field;
}

void AddArgument(ConfigurationData* result,std::string& argc, std::string& argv) 
{
    if(strcmp(argc.c_str(), "CacheRelevanceTime") == 0)
        Add(result->CacheRelevanceTime, argv);
        
    if(strcmp(argc.c_str(), "Host") == 0)
        Add(result->Host, argv);
    
    if(strcmp(argc.c_str(), "Port") == 0)
        Add(result->Port, argv);
}

ConfigurationData Configurator::Read(const char *filename)
{
    std::ifstream fileStream(filename);

    if (!fileStream.is_open())
    {
        perror("Не удалось открыть файл");
        exit(2);
    }
    
    std::string temp;
    ConfigurationData result;

    while(getline(fileStream, temp)) 
    {
        size_t eqPos = temp.find('=');
        std::string argc = temp.substr(0, eqPos);
        std::string argv = temp.substr(eqPos + 1);
        
        AddArgument(&result, argc, argv);
    }
    
    fileStream.close();
    
    return result;
};