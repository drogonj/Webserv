#pragma once

#include "incs.hpp"
#include "Server.hpp"

class Request;
class Server;
class AResponse;
class Get;

class Cgi {
public:
    Cgi(AResponse &response, Request &request, Server &server);
    ~Cgi();
private:
    void cgiBuilder(const Request &request, Server &server);
    void pipeCreatorAndExec(const Server &server);
    void readPipeValue(AResponse &response, Request &request);
    void closeAllPipe();

    std::vector<const char *> _argv;
    std::vector<const char *> _env;
    int _pid;
    int _pipe_in[2]; //Used by server to send data to cgi
    int _pipe_out[2]; // Used by cgi to send data to server
};

