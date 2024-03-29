#pragma once

#include "incs.hpp"

class Location
{
    public:
        Location();
        ~Location();
        Location(const Location &src);
        Location& operator=(const Location &rhs);

        std::string path;    // The location's path
        std::string root;    // Set a new root folder / Set the final path to PATH=root+uri
        std::vector<std::string> allowed_methods; // Set HTTP methods to true or false (Methods will be an enum)
        std::vector<std::string> cgi_paths; // Define cgi path in the same order than cgi_extensions;
        std::vector<std::string> cgi_extensions; // Define cgi_extensions
        std::string index;
        std::map<int, std::string> errors_pages;
        unsigned long max_body_size;
        std::map<unsigned int, std::string> to_return; // Set what to return
        bool autoindex;
        bool isReturn;
        std::string upload_path;
};