# sockets-UDP
Client-server model example with UDP sockets in C programming languaje.

Compile the code:
```sh
$ make
```
Run the server:
```sh
$ ./server
```
Run the clients:
```sh
$ ./client <ip-server> <operation> [id]
```
- ip-server: ip of the server
- operation:
    1. add new record into the registry
    2. read record from the server
    3. remove record from the server	
- id: record to manipulate.

Example, inserte record miRecord123:
```sh
$ ./client 1.2.3.4 1 miRecord123
```
