#ifndef SENDER_H
#define SENDER_H

/** 
 * Устанавливает соединение с хостом по протоколу и отправляет сообщения с возможностью получения 
 */
class Sender {
public:
    
    /**
     * @param protocol  Протокол, по которому класс будет связываться с хостом
     * @param host      Адрес хоста.
     */
    Sender(std::string protocol, std::string host);
    ~Sender();

    /**
     * Оправить GET запрос на хост.
     * @param path      Путь, по которому идёт запрос
     * @param params    Параметры запроса
     * @return          Ответ сервера
     * dfgdg
     */
    std::string SendGET(std::string path, std::vector< std::pair<std::string, std::string> > params);

    /**
     * Оправить произвольный запрос на хост.
     * @param header        Строка, содержащая запрос
     * @param length        Длина запроса
     * @param needAnswer    Необходимость ожидания ответа (по умолчанию true)
     * @return              Ответ сервера
     */
    std::string Send(const char * header, size_t length, bool needAnswer = true);

private:

    /**
     * Функция по преобразованию вектора параметров в строку 
     * @param params    Параметры
     * @return          Строка в GET-виде с параметрами.
     */
    std::string ConvertParams(std::vector< std::pair< std::string, std::string> > params);

private:
    int _socket; // Идентификатор сокета
    std::string _protocol; // Название протокола, по которому идёт соединение
    std::string _host; // Имя хоста
};

#endif /* SENDER_H */ 