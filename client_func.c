/***
 *
 */
#include "client_func.h"
#define h_addr h_addr_list[0]
/******************************************************************************/
/**
 * Registra un recurso en el servidor.
 * @param sockfd: socket de comunicacion.
 * @param server_addr: direccion del servidor.
 * @param id: identificador del registro.
 * @return 0 si todo fue bien, -1 en caso contrario.
*/
int registra_registro(int sockfd, struct sockaddr_in server_addr, char * id)
{	
	struct nuevo_registro reg;
	struct respuesta * resp;
	char * buffer;
	int numbytes;
	int len;

	memset(&reg, 0, sizeof(struct nuevo_registro));
	reg.version = VERSION;
	reg.op = OP_NUEVO_REG;
	reg.puerto = htons(PORT_CLIENTE);
	/*reg.ip = inet_addr(getmyip());*/
	strcpy(reg.id, id);
	
	/* Enviamos una peticion de creacion de nuevo registro */
	len = sizeof(char) * 2 + sizeof(unsigned short) + sizeof(unsigned long) + strlen(reg.id);
	if (envia_paquete(sockfd, server_addr, &reg, len, NOVERBOSE) == -1) {
		fprintf(stderr, "(cliente) sendto");
		return -1;
	}

	/* Solicitud no bloqueante */
	if (solicitud_no_bloqueante(sockfd) == -1) {
		fprintf(stderr, "(cliente) Finalizó el tiempo de espera\n");
		return -1;
	}

	/* Recibimos la respuesta */
	if ((buffer = recibe_paquete(sockfd, server_addr)) == NULL) {
		fprintf(stderr, "(cliente) recvfrom");
		return -1;

	}
	resp = (struct respuesta *) buffer;

	/* Comprobamos la version del protocolo */
	if (resp->version != VERSION)	{
		fprintf(stderr,"(cliente) mensaje recibido del servidor con protocolo incorrecto\n");
		return -1;
	}

	/* Resultado de la operacion? */
	switch(resp->op) {
		
		case OP_OK:
			printf("(cliente) Registro de %s correcto \n", id);
			break;

		case OP_KO:
			printf("(cliente) Fallo el registro de %s: %s\n", id, resp->mensaje);
			return -1;

		default:
			return -1;
	}

	return 0;
}
/******************************************************************************/
/**
 * Elimina un recurso en el servidor.
 * @param sockfd: socket de comunicacion.
 * @param server_addr: direccion del servidor.
 * @param id: identificador del registro.
 * @return 0 si todo fue bien, -1 en caso contrario.
*/
int elimina_registro(int sockfd, struct sockaddr_in server_addr, char * id)
{
	struct respuesta elim_reg;
	struct respuesta * resp;
	char * buffer;
	int numbytes;
	int len;

	memset(&elim_reg, 0, sizeof(struct respuesta));
	elim_reg.version = VERSION;
	elim_reg.op = OP_ELIM_REG;
	strcpy(elim_reg.mensaje, id);

	/* Enviamos una peticion de consulta de registro */
	len = sizeof(char) * 2 + strlen(elim_reg.mensaje);
	if (envia_paquete(sockfd, server_addr, &elim_reg, len, NOVERBOSE) == -1)	{
		fprintf(stderr, "(cliente) sendto");
		return -1;
	}

	/* Solicitud no bloqueante */
	if (solicitud_no_bloqueante(sockfd) == -1) {
		fprintf(stderr, "(cliente) Finalizó el tiempo de espera\n");
		return -1;
	}

	/* Recibimos la respuesta */
	if ((buffer = recibe_paquete(sockfd, server_addr)) == NULL) {
		fprintf(stderr, "(cliente) recvfrom");
		return -1;

	}
	resp = (struct respuesta *) buffer;

	/* Comprobamos la version del protocolo */
	if (resp->version != VERSION) {
		fprintf(stderr,"(cliente) mensaje recibido del servidor con protocolo incorrecto\n");
		return -1;
	}

	/* Resultado de la operacion? */
	switch(resp->op) {
		/* Eliminacion correcta */
		case OP_OK:
			printf("(cliente) Eliminacion del registro %s correcta\n", id);
			break;

		/* Fallo en la eliminacion */
		case OP_KO:
			printf("(cliente) Falló la eliminacion del registro %s: %s\n", id, resp->mensaje);
			return -1;

		/* Operacion desconocida */
		default:
			return -1;
	}

	return 0;
}
/******************************************************************************/
/**
 * Consulta un recurso en el servidor.
 * @param sockfd: socket de comunicacion.
 * @param server_addr: direccion del servidor.
 * @param id: identificador del registro.
 * @param ip: direccion ip de la maquina que registró el recurso, toma su valor al finalizar la funcion.
 * @param puerto: puerto de la maquina que registró el recurso, toma su valor al finalizar la funcion.
 * @return 0 si todo fue bien, -1 en caso contrario.
*/
int consulta_registro(int sockfd, struct sockaddr_in server_addr, char *id, unsigned long * ip, unsigned short * puerto)
{
	struct respuesta consulta_reg;
	struct respuesta * resp;
	struct resp_consulta * resp_ok;
	char * buffer;
	int numbytes;
	int len;

	memset(&consulta_reg, 0, sizeof(struct respuesta));
	consulta_reg.version = VERSION;
	consulta_reg.op = OP_CONSULTA_REG;
	strcpy(consulta_reg.mensaje, id);

	/* Enviamos una peticion de eliminacion de registro */
	len = sizeof(char) * 2 + strlen(consulta_reg.mensaje);
	if (envia_paquete(sockfd, server_addr, &consulta_reg, len, NOVERBOSE) == -1)	{
		fprintf(stderr, "(cliente) sendto");
		return -1;
	}
	
	/* Solicitud no bloqueante */
	if (solicitud_no_bloqueante(sockfd) == -1) {
		fprintf(stderr, "(cliente) Finalizó el tiempo de espera\n");
		return -1;
	}
	/* Recibimos la respuesta */
	if ((buffer = recibe_paquete(sockfd, server_addr)) == NULL) {
		fprintf(stderr, "(cliente) recvfrom");
		return -1;

	}
	resp = (struct respuesta *) buffer;

	/* Comprobamos la version del protocolo */
	if (resp->version != VERSION) {
		fprintf(stderr,"(cliente) mensaje recibido del servidor con protocolo incorrecto\n");
		return -1;
	}

	/* Resultado de la operacion? */
	switch(resp->op) {
		
		case OP_OK:
			resp_ok = (struct resp_consulta *) buffer;
			*ip = ntohl(resp_ok->ip);
			*puerto = ntohs(resp_ok->puerto);
			break;

		case OP_KO:
			printf("(cliente) Falló la consulta del registro %s: %s\n", id, resp->mensaje);
			return -1;

		default:
			return -1;
	}

	return 0;
}
/******************************************************************************/
/**
 * Evita que el cliente se quede bloqueado si no recibe una respuesta del servidor
 * @param sockfd: socket de comunicacion.
 * @return 0 si todo fue bien, -1 en caso contrario.
*/
int solicitud_no_bloqueante(int sockfd)
{
	struct timeval tv;
	fd_set readfds;
	tv.tv_sec = SEGUNDOS;
	tv.tv_usec = MICROSEGUNDOS;
	FD_ZERO(&readfds);
	FD_SET(sockfd, &readfds);

	select(sockfd + 1, &readfds, NULL, NULL, &tv);
	if (!FD_ISSET(sockfd, &readfds)) {
		return -1;
	}
	
	return 0;
}
/******************************************************************************/
/**
 * Obtiene la dirección ip de esta maquina.
 * @return direccion ip en notacion punto.
*/
char * getmyip ()
{
        char name[255];
        struct hostent *he;

        /* get my name */
        if (gethostname(name, 255) == -1) {
                perror("(cliente) error al obtener mi IP-getMyName");
                return NULL;
        }

	/*printf("name: %s\n", name);*/

        /* get my IP address */
        if ((he = gethostbyname (name)) == NULL) {
                herror ("(cliente) error al obtener mi IP-getMuIpAddress");
                return NULL;
        }
        return inet_ntoa(*((struct in_addr *) he->h_addr));
}
