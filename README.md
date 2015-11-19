# sockets-UDP
Client-server model example with UDP sockets in C.

##Basic usage (only linux)
Compile the code:
```sh
$ make
```
run the server:
```sh
$ ./servidor
```
run the clients:
```sh
$ ./cliente <ip-server> <operation>
```
- ip-server: ip of the server
- operation:
    1. add new entry into the registry
    2. read entry from the server
    3. remove entry from the server	
