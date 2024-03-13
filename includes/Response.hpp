#pragma once

#include "incs.hpp"
#include "Request.hpp"
#include "Cgi.hpp"

class Client;
class Cgi;

class Response {
	public:
        Response();
		Response(Server & server, Request &request);

		std::string &getHeader();
		std::string &getContent();
        void setContent(const std::string &content);
        std::string &getUri();
        bool getGenerated();
        void handleRequestError(Server* server, const std::string & uri);
        bool handleReturn(Server *server);
        bool findStatusCode(std::map<unsigned int, std::string>::iterator itf, std::map<unsigned int, std::string>& error_code);
        bool findReturnLocations(Server* server);
        bool isCgi(const std::string &file_type);

        std::ifstream *_contentFile; //For local files
        long long _contentSize;
	private:
        void headerGenBuilder(std::string file_type);
        void headerFileBuilder(std::string file_type);
        void generateAutoindex(Request & req);
        std::string getCodeHeader(std::string * path, Server* server, const std::string & uri);

        Server *server; // Attached server
        std::string _uri;
		std::string _header;
        std::string _content; // For auto-generated content like autoindex
        bool _isGenerated;
};
