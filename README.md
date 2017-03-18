## Tiny Web

#### About

Based on the web server project in CSAPP. Supports 3 types of HTTP methods: GET, HEAD and POST. The robust I/O and socket packages are seperately compiled and can be used in other projects.

#### Project

robust_io and socket_pack are two basic modules for echo_server and web_server. The echo_server package is basically used to test robust_io and socket_pack. The web server serves static files and dynamic files in the web_server directory and cgi-bin directory, respectively.  The source files for dynamic files are in cgi-source, which are to be compiled before dynamic files can be served.

#### Build

Change to web_server and make:

> $ cd web_server
>
> $ make

#### Clean

Change to web_server and make clean:

> $ cd web_server
>
> $ make clean