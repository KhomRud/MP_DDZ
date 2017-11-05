#ifndef SENDER_H
#define SENDER_H

#endif /* SENDER_H */

class Sender
{
public:
    Sender(std::string protocol, std::string host);
    ~Sender();
    
    std::string SendGET(std::string path, std::vector< std::pair<std::string, std::string> > params);
    std::string Send(std::string header);
    
private:
    std::string ConvertParams(std::vector< std::pair< std::string, std::string> > params );
    
private:
    int _socket;
    std::string _protocol;
    std::string _host;
};
