/***
 *
 */
#include "database.h"
/******************************************************************************/
struct base_datos * bbdd;
/******************************************************************************/
/**
 * Crea la estructura para la base de datos.
 * @return 0 si todo fue bien, -1 si no se pudo reservar memoria.
 */
int crea_bbdd()
{
	bbdd = (struct base_datos *) malloc(sizeof(struct base_datos *));
	if (bbdd == NULL) {
		fprintf(stderr, "(bbdd) fallo al reservar memoria\n");
		return -1;
	}

	bbdd->nnodos = 0;
	bbdd->primero = NULL;

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
int inserta_registro(char * id, unsigned short puerto, unsigned long ip)
{
	struct nodo * nuevo;
	struct nodo * aux;

	/* La base de datos está vacia */
	if (bbdd->nnodos == 0) {
		bbdd->primero = (struct nodo *) malloc(sizeof(struct nodo *));
		if (bbdd->primero == NULL) {
			fprintf(stderr, "(bbdd) fallo al reservar memoria\n");
			return -1;
		}

		bbdd->primero->id = strdup(id);
		bbdd->primero->puerto = puerto;
		bbdd->primero->ip = ip;
		bbdd->primero->sgte = NULL;
	}

	/* Insertamos el registro en la primera posicion de la lista */
	else {
		/* Si el registro ya existe, no lo insertamos */
		aux = bbdd->primero;
		while(aux) {
			if (strcmp(aux->id, id) == 0)
				return -2;
			aux = aux->sgte;
		}

		nuevo = (struct nodo *) malloc(sizeof(struct nodo *));
		if (nuevo == NULL) {
			fprintf(stderr, "(bbdd) fallo al reservar memoria\n");
			return -1;
		}
		nuevo->id = strdup(id);
		nuevo->puerto = puerto;
		nuevo->ip = ip;
		nuevo->sgte = bbdd->primero;
		bbdd->primero = nuevo;
	}
	bbdd->nnodos++;

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
int consulta_registro(char * id, unsigned short * puerto, unsigned long * ip)
{
	struct nodo * aux;

	aux= bbdd->primero;

	while(aux) {
		if (strcmp(aux->id, id) == 0) {
			*puerto = aux->puerto;
			*ip = aux->ip;
			return 0;
		}
		aux = aux->sgte;
	}
	return -1;

}
/******************************************************************************/
/**
 * Elimina un registro en la base de datos.
 * @param id: identificador del registro.
 * @return 0 si todo fue bien, -1 si no se encuentra el registro en la base de datos.
 */
int elimina_registro(char * id)
{
	struct nodo * aux, * ant;

	aux = ant = bbdd->primero;
	while(aux) {
		if (strcmp(aux->id, id) == 0) {
			if (aux == bbdd->primero) {
				bbdd->primero = bbdd->primero->sgte;
				bbdd->nnodos--;
				return 0;
			}
			else {
				ant->sgte = aux->sgte;
				bbdd->nnodos--;
				return 0;
			}
		}
		ant = aux;
		aux = aux->sgte;	
	}

	return -1;
}
/******************************************************************************/
/**
 * Imprime el contenido de la base de datos.
 */
void imprime_bbdd()
{
	int i;
	struct nodo * aux;
	struct in_addr dir;

	aux = bbdd->primero;
	printf("Numero de nodos: %d\n", bbdd->nnodos);
	while (aux) {
		printf("\tid: %s\n", aux->id);
		printf("\tpuerto: %d\n", aux->puerto);
		dir.s_addr = aux->ip;
		printf("\tip: %s\n\n", inet_ntoa(dir));
		aux = aux->sgte;
	}
}
/******************************************************************************/
/**
 * Remove the database, free the memory.
 */
int remove_bbdd()
{
	printf("Time to remove the db, bye!\n");
	return 0;
}

