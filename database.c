/***
 *
 */
#include "database.h"
/******************************************************************************/
struct database * db;
/******************************************************************************/
/**
 * Initialize database.
 * @return 0 if everything is ok, -1 otherwise.
 */
int init_database()
{
	db = (struct database *) malloc(sizeof(struct database));
	if (db == NULL) {
		fprintf(stderr, "(database) fail to allocate memory for the database\n");
		return -1;
	}

	db->n_nodes = 0;
	db->first = NULL;

	return 0;
}
/******************************************************************************/
/**
 * Insert a new record in the database.
 * @param id: identifier of the record.
 * @param port: client port.
 * @param ip: client ip.
 * @return 0 if everything is ok, -1 otherwise.
 */
int insert_record(char * id, unsigned short port, unsigned long ip)
{
	struct node * new_node;
	struct node * aux_node;
	
	new_node = (struct node *) malloc(sizeof(struct node));
	if (new_node == NULL) {
		fprintf(stderr, "(database) fail to allocate memory for the new node\n");
		return -1;
	}
	
	new_node->id = strdup(id);
	new_node->port = port;
	new_node->ip = ip;
	
	/* Database is empty */
	if (db->n_nodes == 0){
		db->first = new_node;
		db->first->next = NULL;
		db->n_nodes++;
	}
	/* Insert in the first place */
	else {
		/* check if is already in database */
		aux_node = db->first;
		while (aux_node) {
			if (strcmp(aux_node->id, id) == 0){
				free(new_node->id);
				free(new_node);
				return -1;
			}
			aux_node = aux_node->next;
		}
		
		new_node->next = db->first;
		db->first = new_node;
		db->n_nodes++;
	}
	
	return 0;
}
/******************************************************************************/
/**
 * Consult a record in the database.
 * @param id: identifier of the record, take value after the call.
 * @param port: client port, take value after the call.
 * @param ip: client ip.
 * @return 0 if everything is ok, -1 otherwise.
 */
int consult_record(char * id, unsigned short * port, unsigned long * ip)
{
	struct node * aux;

	aux= db->first;

	while(aux) {
		if (strcmp(aux->id, id) == 0) {
			*port = aux->port;
			*ip = aux->ip;
			return 0;
		}
		aux = aux->next;
	}
	return -1;

}
/******************************************************************************/
/**
 * Delete a record of the database.
 * @param id: identifier of the record.
 * @return 0 if everything is ok, -1 otherwise.
 */
int delete_record(char * id)
{
	struct node * aux, * ant;

	aux = ant = db->first;
	while(aux) {
		if (strcmp(aux->id, id) == 0) {
			if (aux == db->first) {
				db->first = db->first->next;
				db->n_nodes--;
				free(aux->id);
				free(aux);
				return 0;
			}
			else {
				ant->next = aux->next;
				db->n_nodes--;
				free(aux->id);
				free(aux);
				return 0;
			}
		}
		ant = aux;
		aux = aux->next;	
	}

	return -1;
}
/******************************************************************************/
/**
 * Print database.
 */
void print_database()
{
	int i;
	struct node * aux;
	struct in_addr dir;

	aux = db->first;
	printf("Number of nodes: %d\n", db->n_nodes);
	while (aux) {
		printf("\tid: %s\n", aux->id);
		printf("\tport: %d\n", aux->port);
		dir.s_addr = aux->ip;
		printf("\tip: %s\n\n", inet_ntoa(dir));
		aux = aux->next;
	}
}
/******************************************************************************/
/**
 * Delete the database, free the memory.
 */
void delete_database()
{
	struct node * to_delete;
	struct node * temp;
	int i;
	
	temp = db->first;
	i = 0;
	while (temp) {
		i++;
		to_delete = temp;
		temp = temp->next;
		free(to_delete->id);
		free(to_delete);
	}
	
	free(db);
	printf("(database) deleted %d records\n", i);
}

