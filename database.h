/***
 *
 */
#ifndef __DATABASE_H
#define __DATABASE_H
/******************************************************************************/
#include <stdlib.h>
#include <stdio.h> 
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/******************************************************************************/
struct node
{
	struct node * next;
	char * id;
	unsigned short port;
	unsigned long ip;
};

struct database /* old base_datos */
{
	int n_nodes;
	struct node * first;
};
/******************************************************************************/
/*int crea_bbdd();
int inserta_registro(char * id, unsigned short puerto, unsigned long ip);
int consulta_registro(char * id, unsigned short * puerto, unsigned long * ip);
int elimina_registro(char * id);
void imprime_bbdd();
int remove_bbdd();*/

int init_database();
int insert_record(char * id, unsigned short port, unsigned long ip);
int consult_record(char * id, unsigned short * port, unsigned long * ip);
int delete_record(char * id);
void print_database();
int delete_database();

/******************************************************************************/
#endif
