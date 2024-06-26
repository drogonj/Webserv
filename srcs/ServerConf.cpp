#include "../includes/ServerConf.hpp"
#include "../includes/Server.hpp"
#include "../includes/Mommy.hpp"

ServerConf::ServerConf() {
    this->autoindex = false;
    this->max_body_size = 100000000;
	this->port = 0;
}

ServerConf::~ServerConf() {}

ServerConf& ServerConf::operator=(const ServerConf& rhs)
{
    (void)rhs;
    return(*this);
}

ServerConf::ServerConf(const ServerConf &src)
{
    (void)src;
}

void ServerConf::serv_line(std::string& line)
{
   size_t i = 0;
   for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
   line = line.substr(i);
   if(line != "server {")
       throw std::exception();
}

void ServerConf::serv_name_line(std::string& line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 12;
	while (i < line.length())
	{
		int start = i;
		while (i < line.length() && !isspace(line[i]))
			i++;
		std::string server_name;
		server_name = line.substr(start, i - start);
		if (server_name.find(';') != std::string::npos)
			server_name.erase(server_name.length() - 1);
		if(line != "server_name " + server_name)
			throw std::logic_error("Config file : Server : Check server_name");
		frr.servers[currentServerIndex]->server_name = server_name;
		i++;
	}
}

void ServerConf::serv_port(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 7;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
	{
		if(!isdigit(line[i]))
			throw std::logic_error("Config file : Server : Check port");
		i++;
	}
	std::string sport = line.substr(start, i - start);
	if(line != "listen " + sport)
		throw std::logic_error("Config file : Server : Check port");
	int port = atoi(sport.c_str());
	if (!port || port <= 0 || port > 65535)
		throw std::invalid_argument("Invalid port number.");
	frr.servers[currentServerIndex]->port = port;
}

void ServerConf::serv_root(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 5;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string root = line.substr(start, i - start);
	if(line != "root " + root)
		throw std::logic_error("Config file : Server : Check root path");
	frr.servers[currentServerIndex]->root = root;
}

void ServerConf::serv_client_max_body_size(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 21;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string smax_body_size = line.substr(start, i - start);
	if(line != "client_max_body_size " + smax_body_size)
		throw std::logic_error("Config file : Server : Check client_max_body_size");
	unsigned long max_body_size = 0;
	char* endPtr = NULL;
	errno = 0;
	max_body_size = strtoul(smax_body_size.c_str(), &endPtr, 10);
	if(*endPtr != '\0' || (max_body_size == ULONG_MAX && errno == ERANGE) || smax_body_size.find("-") != std::string::npos)
	{
		throw std::invalid_argument("Invalid value for max_body_size.");
	}
	frr.servers[currentServerIndex]->max_body_size = max_body_size;
}

void ServerConf::serv_index(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 6;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string index = line.substr(start, i - start);
	if(index.length() > 1)
		if(index[index.length() - 1] == '/')
			throw std::logic_error("Config file : Server : Check index");
	if(index[0] != '/')
		throw std::logic_error("Config file : Server : Check index");
	if(line != "index " + index)
		throw std::logic_error("Config file : Server : Check index");
	frr.servers[currentServerIndex]->index = index;
}

void ServerConf::serv_return(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 7;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string snb = line.substr(start, i - start);
	int nb = atoi(snb.c_str());
	while (i < line.length() && isspace(line[i]))
		i++;
	start = i;
	while (i < line.length())
		i++;
	std::string path = line.substr(start, i - start);
	if(line != "return " + snb + " " + path)
		throw std::logic_error("Config file : Server : Check return");
	this->isReturn = true;
	frr.servers[currentServerIndex]->to_return[nb] = path;
}

void ServerConf::serv_upload(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 12;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string path = line.substr(start, i - start);
	if(line != "upload_path " + path)
		throw std::logic_error("Config file : Server : Check upload path");
	frr.servers[currentServerIndex]->upload_path = path;
}

void ServerConf::serv_error_page(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 11;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string serror_nb = line.substr(start, i - start);
	int error_nb = atoi(serror_nb.c_str());
	if(error_nb < 100 || error_nb > 505)
		throw std::logic_error("Config file : Server : Check error_page number");
	while(i < line.length() && isspace(line[i]))
		i++;
	start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string error_path = line.substr(start, i - start);
	if(line != "error_page " + serror_nb + " " + error_path)
		throw std::logic_error("Config file : Server : Check error_page path");
	frr.servers[currentServerIndex]->errors_pages[error_nb] = error_path;
}

void ServerConf::serv_CGI(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;

	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 4;
	int start = i + 1;
	while (i < line.length() && !isspace(line[i]))
			i++;
	std::string cgi_extension = line.substr(start, i - start);
	while(isspace(line[i]))
		i++;
	start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string cgi_path = line.substr(start, i - start);
    if(line != "cgi ." + cgi_extension + " " + cgi_path)
		throw std::logic_error("Config file : Server : Check cgi");
    if (access(cgi_path.c_str(), F_OK) != 0)
        throw std::logic_error("Config file : Server : Check cgi access");
    if (access(cgi_path.c_str(), R_OK) != 0)
        throw std::logic_error("Config file : Server : Check cgi executable");
    frr.servers[currentServerIndex]->cgi_values[cgi_extension] = cgi_path;
}

void ServerConf::serv_allowed_methods(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 6;
	while (i < line.length())
	{
		int start = i;
		while (i < line.length() && !isspace(line[i]))
			i++;
		std::string methods = line.substr(start, i - start);
		if(methods != "GET" && methods != "HEAD" && methods != "POST" && methods != "DELETE" && methods != "PUT")
			throw std::logic_error("Config file : Server : Check allowed methods");
		frr.servers[currentServerIndex]->allowed_methods.push_back(methods);
		i++;
	}
	std::vector<std::string>::iterator it = frr.servers[currentServerIndex]->allowed_methods.begin();
	std::vector<std::string>::iterator itend = frr.servers[currentServerIndex]->allowed_methods.end();
	std::string check;
	while(it != itend)
	{
		check += *it + " ";
		++it;
	}
	check.erase(check.length() - 1);
	if(line != "allow " + check)
		throw std::logic_error("Config file : Server : Check allow methods");
}

void ServerConf::serv_autoindex(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 10;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string autoindex = line.substr(start, i - start);
	if(autoindex != "on" && autoindex != "off")
		throw std::logic_error("Config file : Server : Check autoindex");
	if(autoindex == "on")
		frr.servers[currentServerIndex]->autoindex = true;
	else
		frr.servers[currentServerIndex]->autoindex = false;
}

void ServerConf::location_path(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 9;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string location_path = line.substr(start, i - start);
	if(location_path[0] != '/' || line != "location " + location_path + " {" )
		throw std::logic_error("Config file : Location : Check path");
	frr.servers[currentServerIndex]->locations[currentLocationIndex]->path = location_path;
}

void ServerConf::location_root(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 5;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string root = line.substr(start, i - start);
	if(line != "root " + root)
		throw std::logic_error("Config file : Location : Check root path");
	frr.servers[currentServerIndex]->locations[currentLocationIndex]->root = root;
}

void ServerConf::location_allowed_methods(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 6;
	while (i < line.length())
	{
		int start = i;
		while (i < line.length() && !isspace(line[i]))
			i++;
		std::string methods = line.substr(start, i - start);
		if(methods != "GET" && methods != "HEAD" && methods != "POST" && methods != "DELETE" && methods != "PUT")
			throw std::logic_error("Config file : Location : Check allowed methods");
		frr.servers[currentServerIndex]->locations[currentLocationIndex]->allowed_methods.push_back(methods);
		i++;
	}
	std::vector<std::string>::iterator it = frr.servers[currentServerIndex]->locations[currentLocationIndex]->allowed_methods.begin();
	std::vector<std::string>::iterator itend = frr.servers[currentServerIndex]->locations[currentLocationIndex]->allowed_methods.end();
	std::string check;
	while(it != itend)
	{
		check += *it + " ";
		++it;
	}
	check.erase(check.length() - 1);
	if(line != "allow " + check)
		throw std::logic_error("Config file : Location : Check allow");
}

void ServerConf::location_CGI_path(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 9;
	while(i < line.length())
	{
		int start = i;
		while (i < line.length() && !isspace(line[i]))
				i++;
		std::string cgi_path = line.substr(start, i - start);
		frr.servers[currentServerIndex]->locations[currentLocationIndex]->cgi_paths.push_back(cgi_path);
		i++;
	}
	std::vector<std::string>::iterator it = frr.servers[currentServerIndex]->locations[currentLocationIndex]->cgi_paths.begin();
	std::vector<std::string>::iterator itend = frr.servers[currentServerIndex]->locations[currentLocationIndex]->cgi_paths.end();
	std::string check;
	while(it != itend)
	{
		check += *it + " ";
		++it;
	}
	check.erase(check.length() - 1);
	if(line != "cgi_path " + check)
		throw std::logic_error("Config file : Location : Check CGI path");
}

void ServerConf::location_CGI_ext(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 8;
	while(i < line.length())
	{
		int start = i;
		while (i < line.length() && !isspace(line[i]))
				i++;
		std::string cgi_extension = line.substr(start, i - start);
		if(cgi_extension != ".py")
			throw std::logic_error("Config file : Location : Check CGI ext (only python ? bonus ?)");
		frr.servers[currentServerIndex]->locations[currentLocationIndex]->cgi_extensions.push_back(cgi_extension);
		i++;
	}
	std::vector<std::string>::iterator it = frr.servers[currentServerIndex]->locations[currentLocationIndex]->cgi_extensions.begin();
	std::vector<std::string>::iterator itend = frr.servers[currentServerIndex]->locations[currentLocationIndex]->cgi_extensions.end();
	std::string check;
	while(it != itend)
	{
		check += *it + " ";
		++it;
	}
	check.erase(check.length() - 1);
	if(line != "cgi_ext " + check)
		throw std::logic_error("Config file : Location : Check CGI ext");
}

void ServerConf::location_index(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 6;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string index = line.substr(start, i - start);
	if(index.length() > 1)
		if(index[index.length() - 1] == '/')
			throw std::logic_error("Config file : Location : Check index");
	if(index[0] != '/')
			throw std::logic_error("Config file : Location : Check index");
	if(line != "index " + index)
		throw std::logic_error("Config file : Location : Check index");
	frr.servers[currentServerIndex]->locations[currentLocationIndex]->index = index;
}

void ServerConf::location_error_page(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 11;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string serror_nb = line.substr(start, i - start);
	int error_nb = atoi(serror_nb.c_str());
	if(error_nb < 100 || error_nb > 505)
		throw std::logic_error("Config file : Location : Check error_page number");
	while(i < line.length() && isspace(line[i]))
		i++;
	start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string error_path = line.substr(start, i - start);
	if(line != "error_page " + serror_nb + " " + error_path)
		throw std::logic_error("Config file : Location : Check error_page path");
	frr.servers[currentServerIndex]->locations[currentLocationIndex]->errors_pages[error_nb] = error_path;
}

void ServerConf::location_client_max_body_size(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 21;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string smax_body_size = line.substr(start, i - start);
	if(line != "client_max_body_size " + smax_body_size)
		throw std::logic_error("Config file : Location : Check client_max_body_size");
	unsigned long max_body_size = 0;
	char* endPtr = NULL;
	errno = 0;
	max_body_size = strtoul(smax_body_size.c_str(), &endPtr, 10);
	if(*endPtr != '\0' || (max_body_size == ULONG_MAX && errno == ERANGE))
	{
		throw std::invalid_argument("Config file : Location : Cnvalid value for max_body_size.");
	}
	frr.servers[currentServerIndex]->locations[currentLocationIndex]->max_body_size = max_body_size;
}

void ServerConf::location_return(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 7;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string snb = line.substr(start, i - start);
	int nb = atoi(snb.c_str());
	while (i < line.length() && isspace(line[i]))
		i++;
	start = i;
	while (i < line.length())
		i++;
	std::string path = line.substr(start, i - start);
	if(line != "return " + snb + " " + path)
		throw std::logic_error("Config file : Server : Check return");
	frr.servers[currentServerIndex]->locations[currentLocationIndex]->isReturn = true;
	frr.servers[currentServerIndex]->locations[currentLocationIndex]->to_return[nb] = path;
}


void ServerConf::location_autoindex(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 10;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string autoindex = line.substr(start, i - start);
	if(autoindex != "on" && autoindex != "off")
		throw std::logic_error("Config file : Location : Check autoindex");
	if(autoindex == "on")
		frr.servers[currentServerIndex]->locations[currentLocationIndex]->autoindex = true;
	else
		frr.servers[currentServerIndex]->locations[currentLocationIndex]->autoindex = false;
}

void ServerConf::location_upload(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 12;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string path = line.substr(start, i - start);
	if(line != "upload_path " + path)
		throw std::logic_error("Config file : Location : Check upload path");
	frr.servers[currentServerIndex]->locations[currentLocationIndex]->upload_path = path;
}

bool ServerConf::requirements_serv(Mommy& frr, size_t currentServerIndex)
{
	if(currentServerIndex < frr.servers.size())
		if(frr.servers[currentServerIndex]->port == 0 && frr.servers[currentServerIndex]->server_name.length() == 0)
			return (false);
	return(true);
}

void ServerConf::check_multiple_port(Mommy &frr)
{
	std::vector<Server*>::iterator it = frr.servers.begin();
	for(; it != frr.servers.end(); it++)
	{
        int port = (*it)->port;
		std::vector<Server*>::iterator it2 = it;
		it2++;
		for(; it2 != frr.servers.end(); it2++)
		{
			if(port == (*it2)->port)
				throw std::logic_error("Same port multiple times");
		}
	}
}


void ServerConf::inputParsing(std::string argv, Mommy& frr)
{
    std::ifstream file(argv.c_str());
    if (!file.is_open())
	{
        std::cerr << "Error: could not open file.\n";
        return;
    }
    std::string line;
    bool isInsideServerSection = false, isInsideLocationSection = false;
    size_t currentLocationIndex = 0,  currentServerIndex = 0;
    while (std::getline(file, line))
	{
        if (line.empty() || line.find("#") == 0 || line.find("//") == 0) continue;
		if(line.find("location ") != std::string::npos)
		{
			isInsideLocationSection = true;
			Location *location = new Location;
			frr.servers[currentServerIndex]->locations.push_back(location);
			location_path(line, currentServerIndex, frr, currentLocationIndex);
			continue;
		}
        if (line.find("server {") == 0)
		{
			serv_line(line);
			Server* server = new Server;
			frr.servers.push_back(server);
            isInsideServerSection = true;
            continue;
        }
		if(line.find("    }") == 0)
		{
			isInsideLocationSection = false;
			++currentLocationIndex;
			continue;
		}
        if (line.find("}") == 0)
		{
			currentLocationIndex = 0;
			if(!requirements_serv(frr, currentServerIndex))
				throw std::logic_error("Config file : Server : need at least a server name and port");
            isInsideServerSection = false;
            frr.servers.back()->setup();
			++currentServerIndex;
            continue;
        }
        if (isInsideServerSection && !isInsideLocationSection && line.find("server_name ") != std::string::npos)
            serv_name_line(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("listen ") != std::string::npos)
            serv_port(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("upload_path ") != std::string::npos)
            serv_upload(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("root ") != std::string::npos)
			serv_root(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("client_max_body_size ") != std::string::npos)
			serv_client_max_body_size(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("index ") != std::string::npos && line.find("autoindex") == std::string::npos)
			serv_index(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("return ") != std::string::npos)
			serv_return(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("error_page ") != std::string::npos)
			serv_error_page(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("cgi ") != std::string::npos)
			serv_CGI(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("allow ") != std::string::npos)
			serv_allowed_methods(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("autoindex ") != std::string::npos)
			serv_autoindex(line, currentServerIndex, frr);
		else if(isInsideLocationSection && line.find("root ") != std::string::npos)
			location_root(line, currentServerIndex, frr, currentLocationIndex);
		else if(isInsideLocationSection && line.find("upload_path ") != std::string::npos)
			location_upload(line, currentServerIndex, frr, currentLocationIndex);
		else if(isInsideLocationSection && line.find("allow ") != std::string::npos)
			location_allowed_methods(line, currentServerIndex, frr, currentLocationIndex);
		else if(isInsideLocationSection && line.find("cgi_path ") != std::string::npos)
			location_CGI_path(line, currentServerIndex, frr, currentLocationIndex);
		else if(isInsideLocationSection && line.find("cgi_ext ") != std::string::npos)
			location_CGI_ext(line, currentServerIndex, frr, currentLocationIndex);
		else if(isInsideLocationSection && line.find("index ") != std::string::npos && line.find("autoindex") == std::string::npos)
			location_index(line, currentServerIndex, frr, currentLocationIndex);
		else if(isInsideLocationSection && line.find("error_page ") != std::string::npos)
			location_error_page(line, currentServerIndex, frr, currentLocationIndex);
        else if(isInsideLocationSection && line.find("autoindex ") != std::string::npos)
            location_autoindex(line, currentServerIndex, frr, currentLocationIndex);
		else if(isInsideLocationSection && line.find("client_max_body_size ") != std::string::npos)
			location_client_max_body_size(line, currentServerIndex, frr, currentLocationIndex);
		else if(isInsideLocationSection && line.find("return ") != std::string::npos)
			location_return(line, currentServerIndex, frr, currentLocationIndex);
		else
			throw std::logic_error("is detected in configuration file");
    }
	check_multiple_port(frr);
	if(isInsideLocationSection == true || isInsideServerSection == true)
		throw std::logic_error("Mismatched brackets in configuration file");
}


/*
 * Call this function with an extension (without the '.' at the beginning)
 * To determine if it is a CGI
*/

bool ServerConf::isCgi(const std::string &extension) const {
    for (std::map<std::string, std::string>::const_iterator it = this->cgi_values.begin(); it != this->cgi_values.end() ;it++) {
        if (it->first == extension)
            return true;
    }
    return false;
}

const std::string& ServerConf::getRoot() const {
	 return this->root; 
}

const std::string& ServerConf::getServerName() const {
	 return this->server_name; 
}

int ServerConf::getPort() const {
	 return this->port; 
}

const std::vector<std::string>& ServerConf::getAllowedMethods() const {
	 return this->allowed_methods; 
}

const std::string& ServerConf::getIndex() const {
	 return this->index; 
}

const std::map<unsigned int, std::string>& ServerConf::getErrorsPages() const {
	 return this->errors_pages; 
}

unsigned long ServerConf::getMaxBodySize() const {
	 return this->max_body_size; 
}

std::vector<Location*>& ServerConf::getLocations() {
	 return this->locations; 
}

const std::string &ServerConf::getCgiPathFromExtension(const std::string &extension) {
	 return this->cgi_values[extension]; 
}

bool ServerConf::getAutoindex() const {
	 return this->autoindex; 
}

std::map<unsigned int, std::string>& ServerConf::getToReturn() {
	 return this->to_return; 
}
