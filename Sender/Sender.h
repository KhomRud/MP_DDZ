#ifndef SENDER_H
#define SENDER_H

#endif /* SENDER_H */

class Sender
{
public:
    Sender(std::string protocol, std::string host, std::string path, std::vector< std::pair<std::string, std::string> > params);
    std::string ConvertParams(std::vector< std::pair< std::string, std::string> > params );
    std::string SendGET();
    
private:
    std::string _protocol;
    std::string _host;
    std::string _path;
    std::string _params;
};
