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

struct database 
{
	int n_nodes;
	struct node * first;
};
/******************************************************************************/
int init_database();
int insert_record(char * id, unsigned short port, unsigned long ip);
int consult_record(char * id, unsigned short * port, unsigned long * ip);
int delete_record(char * id);
void print_database();
void delete_database();

/******************************************************************************/
#endif
