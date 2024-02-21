#pragma once

#include "Location.hpp"
#include <cstdlib>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

class Mommy;

class ServerConf
{
    public:
        ServerConf();
        ~ServerConf();
        ServerConf(const ServerConf &src);
        ServerConf& operator=(const ServerConf &rhs);

        void inputParsing(std::string argv, Mommy& frr);
        void check_serv_line(std::string &line);
        void check_serv_name_line(std::string& line, size_t currentServerIndex, Mommy& frr);
        void check_serv_port(std::string &line, size_t currentServerIndex, Mommy& frr);
        void check_serv_root(std::string &line, size_t currentServerIndex, Mommy& frr);
        void check_client_max_body_size(std::string &line, size_t currentServerIndex, Mommy& frr);

        const std::string& getRoot() const { return this->root; }
        const std::string& getServerName() const { return this->server_name; }
        int getPort() const { return this->port; }
        const std::vector<std::string>& getAllowedMethods() const { return this->allowed_methods; }
        const std::string& getIndex() const { return this->index; }
        const std::map<int, std::string>& getErrorsPages() const { return this->errors_pages; }
        unsigned long getMaxBodySize() const { return this->max_body_size; }
        const std::vector<Location>& getLocations() const { return this->locations; }
        const std::vector<std::string>& getCGIPaths() const { return this->cgi_paths; }
        const std::vector<std::string>& getCGIExtensions() const { return this->cgi_extensions; }
        bool getAutoindex() const { return this->autoindex; }
        const std::string& getToReturn() const { return this->to_return; }
        //je laisse en public ceux que je n'ai pas encore fait
        std::vector<Location> locations; // Contain all the locations that are in an other class
        std::string index;    // contain indexes (if several index given, we try to load in the given order)
        bool autoindex; // Set on/off autoindex (directory listing page. If an index is given and founded, the index is returned, otherwise the directory listing page appear to replace the index)
        std::vector<std::string> allowed_methods; // Set HTTP methods to true or false (methods will be an enum)
        std::string root; // Define the root directory
        std::map<int, std::string> errors_pages; // contain errors page paths
        std::vector<std::string> cgi_paths; // Define cgi path in the same order than cgi_extensions;
        std::vector<std::string> cgi_extensions; // Define cgi_extensions
        std::string to_return; // Set what to return

    private:
        std::string server_name;    // contain the server_name
        int port; // contain ports to listen to
        unsigned long max_body_size;  // HTTP request's body max size (usefull for PUT and POST)
};