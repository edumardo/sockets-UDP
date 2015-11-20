/***
 *
 */
#ifndef __BBDD_H
#define __BBDD_H
/******************************************************************************/
#include <stdlib.h>
#include <stdio.h> 
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/******************************************************************************/
struct nodo
{
	struct nodo * sgte;
	char * id;
	unsigned short puerto;
	unsigned long ip;
};

struct base_datos
{
	int nnodos;
	struct nodo * primero;
};
/******************************************************************************/
int crea_bbdd();
int inserta_registro(char * id, unsigned short puerto, unsigned long ip);
int consulta_registro(char * id, unsigned short * puerto, unsigned long * ip);
int elimina_registro(char * id);
void imprime_bbdd();
int remove_bbdd();
/******************************************************************************/
#endif
