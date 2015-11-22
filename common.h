/***
 *
 */
#ifndef __COMMON_H
#define __COMMON_H
/******************************************************************************/
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
/******************************************************************************/
#define MAXSIZE 	256
#define MAXBUFLEN 	264
#define PORT_SERVER	2222
#define PORT_CLIENTE	12344
#define VERSION 	0x01

#define OP_NUEVO_REG	0x01
#define OP_CONSULTA_REG	0x02
#define OP_ELIM_REG	0x03

#define OP_OK 		0x00
#define OP_KO		0x01
/******************************************************************************/
struct nuevo_registro	/* 1 + 1 + 2 + 4 + 256 = 264 bytes */
{
	char version;
	char op;
	unsigned short puerto;
	unsigned long ip;
	char id[MAXSIZE];
} __attribute__((packed));

struct respuesta	/* 1 + 1 + 256 = 258 bytes */
{
	char version;
	char op;
	char mensaje[MAXSIZE];
} __attribute__((packed));

struct resp_consulta	/* 1 + 1 + 2 + 4 = 8 bytes */
{
	char version;
	char op;
	unsigned short puerto;
	unsigned long ip;
} __attribute__((packed));
/******************************************************************************/
int envia_paquete(int sockfd, struct sockaddr_in server_addr, void * paquete, int len);
char * recibe_paquete(int sockfd, struct sockaddr_in server_addr);
/******************************************************************************/
#endif
