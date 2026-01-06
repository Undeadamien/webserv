# Webserv

A custom HTTP server written in C++98 for handling HTTP requests, CGI scripts, and file uploads.

## Features

HTTP Methods: Supports GET, POST, and DELETE.
CGI Scripts: Execute Python, Bash, and Perl scripts.
File Uploads: Simple and advanced upload modes.
Autoindex: Directory listing for folders.
Custom Error Pages: Personalized 404, 500, etc.
Configuration: Define server blocks, locations, and error pages.

## Setup

Build the server:

```
make
```

Run the server:

```
./webserv config/goodweb.conf
```

Test the server:  
Open your browser and navigate to http://localhost:3434.

Configuration:  
Edit config/goodweb.conf to customize server settings:

```
server {
listen 3434;
root ./goodweb/;
index index.html;

    location /uploads {
        root ./goodweb/main;
        cgi_extension .py /usr/bin/python3;
        upload_path ./goodweb/list/;
        allow_methods GET POST DELETE;
    }

}
```
