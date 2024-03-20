#include "../../includes/Response/AResponse.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/Server.hpp"

defaultErrorCodes __defaultErrorCodes;
defaultErrorPages __defaultErrorPages;

AResponse::AResponse() : _contentFile(0), _isGenerated(false) {}
AResponse::AResponse(Server & server) : server(&server), _contentFile(0), _isGenerated(false) {
    if (DEBUG)
	    std::cout << "New response is under building.." << std::endl;
}

void AResponse::defaultFileBuilder(const Request &request) {
    this->_contentFile = new std::ifstream;
    this->_contentFile->open(this->_uri.c_str(), MimeUtils::getOpenMode(request.getExtension()));
    if (!this->_contentFile->is_open() && DEBUG) {
        std::cerr << RED << "Error: " << strerror(errno) << RESET << std::endl;
        return;
    }
    this->_contentFile->seekg(0, std::ios::end);
    this->_contentSize = this->_contentFile->tellg();
    this->_contentFile->seekg(0, std::ios::beg);
}

void AResponse::headerGenBuilder(std::string file_type) {
    std::stringstream header_tmp;

    if (this->_isGenerated) {
        file_type = "text/html";
    }
    header_tmp << "HTTP/1.1 200 OK\nContent-Type: " << file_type << "\nContent-Length: " << this->_content.size() << "\n\n";
    this->_header = header_tmp.str();
}

std::string AResponse::getCodeHeader(std::string * path, Server* server,  const std::string & uri) {
    if (path)
        *path = server->getRootFrom(uri) + "/";
    if (errno == MISSINGSLASH || errno == INVALIDSLASH) {
        return ("HTTP/1.1 301 Moved Permanently\n");
    } else if (errno == EACCES || errno == EROFS) {
        try {
            *path += server->getErrorPage(403, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[403];
        }
        return ("HTTP/1.1 403 Forbidden\n");
    } else if (errno == ENAMETOOLONG || errno == TOOLONGREQUEST) {
        try {
            *path += server->getErrorPage(414, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[414];
        }
        return ("HTTP/1.1 414 Uri Too Long\n");
    } else if (errno == ENOENT) {
        try {
            *path += server->getErrorPage(404, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[404];
        }
        return ("HTTP/1.1 404 Not Found\n");
    }else if (errno == ENOTDIR || errno == EINVAL || errno == EROFS || errno == ISDIRECTORY || errno == BADHEADER) {
        try {
            *path += server->getErrorPage(400, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[400];
        }
        return ("HTTP/1.1 400 Bad Request\n");
    }else if (errno == ETXTBSY) {
        try {
            *path += server->getErrorPage(409, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[409];
        }
        return ("HTTP/1.1 409 Conflict\n");
    }else if (errno == NOTALLOWEDMETHOD) {
        try {
            *path += server->getErrorPage(405, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[405];
        }
        return ("HTTP/1.1 405 Method Not Allowed\n");
    } else {
        try {
            *path += server->getErrorPage(500, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[500];
        }
        return ("HTTP/1.1 500 Internal Server Error\n");
    }
}

void AResponse::redirectWellSlashed(const std::string & uri) {
    std::string newuri = uri;
    this->_header = getCodeHeader(0, 0, uri);
    if (errno == INVALIDSLASH) {
        size_t i = 0;
        while (i + 1 < newuri.size()) {
            if (newuri[i] == '/' && newuri[i + 1] == '/') {
                newuri.erase(i, 1);
            } else {
                i++;
            }
        }
    } else {
        newuri += "/";
    }
    this->_header += ("Location: " + newuri + "\n");
    this->_isGenerated = true;
}

void AResponse::handleRequestError(Server* server, const std::string & uri) {
    std::stringstream tmphead;
    std::string codePath;

    if (DEBUG)
        std::cout << RED << "Sending error code, reason: " << errno << RESET << std::endl;
    if (errno == MISSINGSLASH || errno == INVALIDSLASH) {
        redirectWellSlashed(uri);
        return;
    }
    tmphead << getCodeHeader(&codePath, server, uri);
    if (this->_contentFile)
        delete this->_contentFile;
    this->_contentFile = new std::ifstream;
    this->_contentFile->open(codePath.c_str(),std::ifstream::in);
    if (!this->_contentFile->is_open()) {
        if (DEBUG)
            std::cerr << RED << "error: can't access error page" << RESET << std::endl;
        this->_header = tmphead.str() + "Content-Length: 0\n\r\n\r\n";
        return;
    }
    this->_contentFile->seekg(0, std::ios::end);
    this->_contentSize = this->_contentFile->tellg();
    this->_contentFile->seekg(0, std::ios::beg);
    tmphead << "Content-Length: " << this->_contentSize << "\nContent-Type: text/html\n\n";
    this->_header = tmphead.str();
}

std::string & AResponse::getContent() {
    return this->_content;
}

void AResponse::setContent(const std::string &content) {
    this->_content = content;
}

void AResponse::setContentSize(long long size) {
    this->_contentSize = size;
}

std::string &AResponse::getUri() {
    return this->_uri;
}

std::string &AResponse::getHeader() {
    return this->_header;
}

bool AResponse::getGenerated() {
    return this->_isGenerated;
}

std::ifstream * AResponse::getContentFile() {
    return this->_contentFile;
}

long long AResponse::getContentSize() {
    return this->_contentSize;
}
