/***
 *
 */
#include "database.h"
/******************************************************************************/
struct database * db;
/******************************************************************************/
/**
 * Crea la estructura para la base de datos.
 * @return 0 si todo fue bien, -1 si no se pudo reservar memoria.
 */
int init_database()
{
	db = (struct database *) malloc(sizeof(struct database *));
	if (db == NULL) {
		fprintf(stderr, "(database) fallo al reservar memoria\n");
		return -1;
	}

	db->n_nodes = 0;
	db->first = NULL;

	return 0;
}
/******************************************************************************/
/**
 * Inserta un registro en la base de datos.
 * @param id: identificador del registro.
 * @param puerto: puerto de la maquina que registra el registro.
 * @param ip: direccion ip de la maquina que registra el registro.
 * @return 0 si todo fue bien, -1 si no se pudo reservar memoria, -2 si el registro ya está en la base de datos.
 */
int insert_record(char * id, unsigned short port, unsigned long ip)
{
	struct node * nuevo;
	struct node * aux;

	/* La base de datos está vacia */
	if (db->n_nodes == 0) {
		db->first = (struct node *) malloc(sizeof(struct node *));
		if (db->first == NULL) {
			fprintf(stderr, "(database) fallo al reservar memoria\n");
			return -1;
		}

		db->first->id = strdup(id);
		db->first->port = port;
		db->first->ip = ip;
		db->first->next = NULL;
	}

	/* Insertamos el registro en la primera posicion de la lista */
	else {
		/* Si el registro ya existe, no lo insertamos */
		aux = db->first;
		while(aux) {
			if (strcmp(aux->id, id) == 0)
				return -2;
			aux = aux->next;
		}

		nuevo = (struct node *) malloc(sizeof(struct node *));
		if (nuevo == NULL) {
			fprintf(stderr, "(database) fallo al reservar memoria\n");
			return -1;
		}
		nuevo->id = strdup(id);
		nuevo->port = port;
		nuevo->ip = ip;
		nuevo->next = db->first;
		db->first = nuevo;
	}
	db->n_nodes++;

	return 0;
}
/******************************************************************************/
/**
 * Consulta un registro en la base de datos.
 * @param id: identificador del registro.
 * @param puerto: puerto de la maquina que registro el registro, toma su valor al finalizar la funcion.
 * @param ip: direccion ip de la maquina que registro el registro, toma su valor al finalizar la funcion.
 * @return 0 si todo fue bien, -1 si no se encuentra el registro en la base de datos.
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
 * Elimina un registro en la base de datos.
 * @param id: identificador del registro.
 * @return 0 si todo fue bien, -1 si no se encuentra el registro en la base de datos.
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
				return 0;
			}
			else {
				ant->next = aux->next;
				db->n_nodes--;
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
 * Imprime el contenido de la base de datos.
 */
void print_database()
{
	int i;
	struct node * aux;
	struct in_addr dir;

	aux = db->first;
	printf("Numero de nodos: %d\n", db->n_nodes);
	while (aux) {
		printf("\tid: %s\n", aux->id);
		printf("\tpuerto: %d\n", aux->port);
		dir.s_addr = aux->ip;
		printf("\tip: %s\n\n", inet_ntoa(dir));
		aux = aux->next;
	}
}
/******************************************************************************/
/**
 * Remove the database, free the memory.
 */
int delete_database()
{
	printf("Time to remove the db, bye!\n");
	return 0;
}

