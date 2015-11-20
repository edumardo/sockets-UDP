/***
 *
 */
#ifndef __CLIENTE_FUNC_H
#define __CLIENTE_FUNC_H
/******************************************************************************/
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <unistd.h>
#include "common.h"
/******************************************************************************/
/* Esperamos "SEGUNDOS,MICROSEGUNDOS" segundos a que el servidor responda */
#define SEGUNDOS		2
#define MICROSEGUNDOS	500000
/******************************************************************************/
int registra_registro(int sockfd, struct sockaddr_in server_addr, char * id);
int elimina_registro(int sockfd, struct sockaddr_in server_addr, char * id);
int consulta_registro(int sockfd, struct sockaddr_in server_addr, char * id, unsigned long * ip, unsigned short * puerto);
int solicitud_no_bloqueante(int sockfd);
char * getmyip ();
/******************************************************************************/
#endif
