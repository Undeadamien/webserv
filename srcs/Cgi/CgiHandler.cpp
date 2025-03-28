#include "CgiHandler.hpp"

CgiHandler::CgiHandler() {}

CgiHandler::CgiHandler(const CgiHandler &src) { *this = src; }

CgiHandler::~CgiHandler(void) {}

CgiHandler &CgiHandler::operator=(const CgiHandler &src) {
	if (this != &src) {
		this->_output = src._output;
		this->_isChunked = src._isChunked;
	}
	return *this;
}

Response CgiHandler::CgiMaker(const Request *request,
							  const BlockLocation *location,
							  BlockServer *server) {
	this->_target = request->parsePath();
	this->_cgi_extension = parseFileExtension(_target);
	this->_cgi_path = location->getCgiPath(_cgi_extension);
	this->_target = location->getRoot() + this->_target;
	Log::log(Log::DEBUG, "Target : %s | Extension: %s | Path Extension : %s",
			 _target.c_str(), _cgi_extension.c_str(), _cgi_path.c_str());

	return executeCgi(request, server);
}

#define CGI_TIMEOUT 5  // Timeout en secondes

Response CgiHandler::executeCgi(const Request *request, BlockServer *server) {
	int pipe_fd[2];
	if (pipe(pipe_fd) == -1) {
		return createResponseError(server, "HTTP/1.1", "500",
								   "Internal Server Error");
	}

	pid_t pid = fork();
	if (pid == -1) {
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return createResponseError(server, "HTTP/1.1", "500",
								   "Internal Server Error");
	}

	if (pid == 0) {			// Processus enfant
		close(pipe_fd[0]);	// Ferme la lecture du pipe
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);

		// Création de l'environnement CGI
		std::vector<std::string> env_list;
		env_list.push_back("SCRIPT_FILENAME=" + _target);
		env_list.push_back("REQUEST_METHOD=" +
						   e_Methods_to_String(request->getMethod()));
		env_list.push_back("QUERY_STRING=" + request->parseQuery());
		env_list.push_back("GATEWAY_INTERFACE=CGI/1.1");
		env_list.push_back("SERVER_PROTOCOL=HTTP/1.1");
		env_list.push_back("REDIRECT_STATUS=200");

		MapHeaders headers = request->getHeaders();
		if (headers.find("Content-Type") != headers.end())
			env_list.push_back("CONTENT_TYPE=" + headers["Content-Type"]);
		if (headers.find("Content-Length") != headers.end())
			env_list.push_back("CONTENT_LENGTH=" + headers["Content-Length"]);

		// Conversion `std::vector<std::string>` en `char *envp[]`
		std::vector<char *> envp;
		for (size_t i = 0; i < env_list.size(); i++)
			envp.push_back(const_cast<char *>(env_list[i].c_str()));
		envp.push_back(NULL);

		std::string ch_path = _target.substr(0, _target.rfind('/'));
		_target = _target.substr(_target.rfind('/') + 1);
		if (chdir(ch_path.c_str()) == -1) exit(1);

		// Arguments pour execve
		char *args[] = {const_cast<char *>(_cgi_path.c_str()),
						const_cast<char *>(_target.c_str()), NULL};

		// Exécuter le CGI
		execve(_cgi_path.c_str(), args, &envp[0]);

		Log::log(Log::DEBUG, "Execve has failed");
	}
	// Processus parent
	close(pipe_fd[1]);	// Ferme l'écriture

	// Gestion du timeout
	alarm(CGI_TIMEOUT);	 // Déclenche une alarme dans CGI_TIMEOUT secondes

	int status;
	pid_t result = waitpid(pid, &status, 0);
	if (status != 0) Log::log(Log::ERROR, "Interval Server Error");

	alarm(0);  // Désactive l'alarme si le processus s'est terminé à temps

	if (result == -1) {
		kill(pid, SIGKILL);		// Tue le processus CGI s'il est bloqué
		waitpid(pid, NULL, 0);	// Nettoie le processus zombie
		return createResponseError(server, "HTTP/1.1", "504",
								   "Gateway Timeout");
	}
	// Lire la sortie du CGI
	std::string output;
	char buffer[1024];
	ssize_t bytes_read;
	while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1)) > 0) {
		buffer[bytes_read] = '\0';
		output += buffer;
	}
	close(pipe_fd[0]);

	// Vérifier le statut de sortie du CGI
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
		return createResponseError(server, "HTTP/1.1", "502", "Bad Gateway");
	}

	output = "HTTP/1.1 200 OK\r\n" + output;
	std::cout << output << std::endl;

	return Response(output);  // Transformer la sortie CGI en réponse HTTP
}
