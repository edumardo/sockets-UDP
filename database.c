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
	struct node * aux_node;

	aux_node = db->first;

	while(aux_node) {
		if (strcmp(aux_node->id, id) == 0) {
			*port = aux_node->port;
			*ip = aux_node->ip;
			return 0;
		}
		aux_node = aux_node->next;
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
	struct node * aux_node; 
	struct node * bef_node;

	aux_node = bef_node = db->first;
	while(aux_node) {
		if (strcmp(aux_node->id, id) == 0) {
			if (aux_node == db->first) {
				db->first = db->first->next;
				db->n_nodes--;
				free(aux_node->id);
				free(aux_node);
				return 0;
			}
			else {
				bef_node->next = aux_node->next;
				db->n_nodes--;
				free(aux_node->id);
				free(aux_node);
				return 0;
			}
		}
		bef_node = aux_node;
		aux_node = aux_node->next;	
	}

	return -1;
}
/******************************************************************************/
/**
 * Print the database.
 */
void print_database()
{
	int i;
	struct node * aux_node;
	struct in_addr dir;

	aux_node = db->first;
	printf("Number of nodes: %d\n", db->n_nodes);
	while (aux_node) {
		printf("\tid: %s\n", aux_node->id);
		printf("\tport: %d\n", aux_node->port);
		dir.s_addr = aux_node->ip;
		printf("\tip: %s\n\n", inet_ntoa(dir));
		aux_node = aux_node->next;
	}
}
/******************************************************************************/
/**
 * Delete the database, free the memory.
 */
void delete_database()
{
	struct node * to_delete;
	struct node * aux_node;
	int i;
	
	aux_node = db->first;
	i = 0;
	while (aux_node) {
		i++;
		to_delete = aux_node;
		aux_node = aux_node->next;
		free(to_delete->id);
		free(to_delete);
	}
	
	free(db);
	printf("(database) deleted %d records\n", i);
}
