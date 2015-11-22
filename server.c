/***
 *
 */
#include "common.h"
#include "server_func.h"
#include <signal.h>
/******************************************************************************/

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in mi_addr; 
	struct sockaddr_in cliente_addr;
	socklen_t addr_len;

	struct respuesta * paquete;		/* paquete generico que recibe el servidor */
	struct respuesta resp;			/* paquete de respuesta */
	struct respuesta * elim_reg;		/* paquete de elimina registro */
	struct nuevo_registro * nuevo_reg;	/* paquete de nuevo registro */
	struct respuesta * consulta_reg;	/* paquete de consulta registro */
	struct resp_consulta resp_con;		/* respuesta a consulta registro */

	struct in_addr dir;			/* Para pasar ip de ulong a formato punto */

	char buffer[MAXBUFLEN];
	int numbytes;
	int len;
	int status;
	unsigned short puerto;
	unsigned long ip;

	/* Creamos la estructura de la base de datos */
	if (init_database() == -1) {
		fprintf(stderr, "(servidor) error al crear la base de datos");
		exit(1);
	}
	
	/* catch SIGINT signal to avoid exit without free memory */
	signal(SIGINT, end_server);
	
	/* Creamos el socket */
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("(servidor) error al crear el socket");
		exit(1);
	}

	/* puerto e ip del servidor */
	mi_addr.sin_family = AF_INET;         
	mi_addr.sin_port = htons(PORT_SERVER);       
	mi_addr.sin_addr.s_addr = INADDR_ANY; 
	memset(&(mi_addr.sin_zero), '\0', 8); 

	/* Asociamos el socket a la direccion del servidor */
	if (bind(sockfd, (struct sockaddr *) &mi_addr, sizeof(struct sockaddr)) == -1) {
		perror("(servidor) bind");
		exit(1);
	}
	
	addr_len = sizeof(struct sockaddr);

	printf("-------------------------------------------------------------\n");
	printf("(servidor) esperando conexiones de los clientes...\n");

	while(1) {
		printf("-------------------------------------------------------------\n");
		memset(buffer, 0, MAXBUFLEN);
		memset(&resp, 0, sizeof(struct respuesta));
		memset(&resp_con, 0, sizeof(struct resp_consulta));

		/* Recibimos un paquete del cliente */
		if ((numbytes = recvfrom(sockfd, 
					buffer, 
					MAXBUFLEN-1, 
					0, 
					(struct sockaddr *) &cliente_addr, 
					&addr_len)) == -1) {
			perror("(servidor) recvfrom");
			exit(1);
		}
		
		paquete = (struct respuesta *) buffer;

		/* Comprobamos la version del protocolo de cada paquete que recibimos */
		if (paquete->version != VERSION) {
			resp.version = VERSION;
			resp.op = OP_KO;
			strcpy(resp.mensaje, "Version del protocolo incorrecta");
			len = sizeof(char) * 2 + strlen(resp.mensaje);
			if (envia_paquete(sockfd, cliente_addr, &resp, len) == -1)
				fprintf(stderr, "(servidor) fallo al enviar la respuesta al cliente\n");

			exit(1);
		}

		/* Que operacion va a realizar el servidor */
		switch(paquete->op) {
			
			/* Nuevo registro */
			case OP_NUEVO_REG:

				nuevo_reg = (struct nuevo_registro *) buffer;
				dir.s_addr = nuevo_reg->ip;
				memset(&resp, 0, sizeof(struct respuesta));
				printf("(servidor) Insertar nuevo registro en la base de datos\n");
				printf("\t\tidentificador: %s\n\t\tpuerto: %d\n\t\tdireccion: %s\n\n", nuevo_reg->id, ntohs(nuevo_reg->puerto), inet_ntoa(dir));

				if ((status = insert_record(nuevo_reg->id, ntohs(nuevo_reg->puerto), nuevo_reg->ip)) != 0) {
					resp.version = VERSION;
					resp.op = OP_KO;
				
					if (status == -1)
						strcpy(resp.mensaje, "Error en la base de datos, no se insertó el elemento");
					else
						strcpy(resp.mensaje, "El registro ya existe en la base de datos");

					printf("(servidor) %s\n", resp.mensaje);				

					len = sizeof(char) * 2 + strlen(resp.mensaje); 
					if (envia_paquete(sockfd, cliente_addr, &resp, len) == -1)
						fprintf(stderr, "(servidor) fallo al enviar la respuesta al cliente\n");
					
					break;
				}

				resp.version = VERSION;
				resp.op = OP_OK;
				len = sizeof(char) * 2;
				if (envia_paquete(sockfd, cliente_addr, &resp, len) == -1) {
					fprintf(stderr, "(servidor) fallo al enviar la respuesta al cliente\n");
					break;
				}
				printf("(servidor) insercion realizada con exito\n");
				break;

			/* Consulta registro */
			case OP_CONSULTA_REG:
				consulta_reg = (struct respuesta *) buffer;
				memset(&resp, 0, sizeof(struct respuesta));
				printf("(servidor) Consultar registro de la base de datos\n");
				printf("\t\tidentificador: %s\n\n", consulta_reg->mensaje);

				if (consult_record(consulta_reg->mensaje, &puerto, &ip) == -1) {
					resp.version = VERSION;
					resp.op = OP_KO;
					strcpy(resp.mensaje, "El registro no existe");
					printf("(servidor) %s\n", resp.mensaje);
					len = sizeof(char) * 2 + strlen(resp.mensaje);
					if (envia_paquete(sockfd, cliente_addr, &resp, len) == -1)
						fprintf(stderr, "(servidor) fallo al enviar la respuesta al cliente\n");
				
					break;
				}

				resp_con.version = VERSION;
				resp_con.op = OP_OK;
				resp_con.puerto = htons(puerto);
				resp_con.ip = htonl(ip);
				len = sizeof(char) * 2 + sizeof(unsigned short) + sizeof(unsigned long);
				if (envia_paquete(sockfd, cliente_addr, &resp_con, len) == -1) {
					fprintf(stderr, "(servidor) fallo al enviar la respuesta al cliente\n");
					break;
				}
				printf("(servidor) consulta realizada con exito\n");
				break;

			/* Elimina registro */
			case OP_ELIM_REG:
				elim_reg = (struct respuesta *) buffer;
				memset(&resp, 0, sizeof(struct respuesta));

				printf("(servidor) Eliminar registro de la base de datos\n");
				printf("\t\tidentificador: %s\n\n", elim_reg->mensaje);

				if (delete_record(elim_reg->mensaje) == -1) {
					resp.version = VERSION;
					resp.op = OP_KO;
					strcpy(resp.mensaje, "El registro no existe");
					printf("(servidor) %s\n", resp.mensaje);
					len = sizeof(char) * 2 + strlen(resp.mensaje); 
					if (envia_paquete(sockfd, cliente_addr, &resp, len) == -1)
						fprintf(stderr, "(servidor) fallo al enviar la respuesta al cliente\n");

					break;
				}
				
				resp.version = VERSION;
				resp.op = OP_OK;
				len = sizeof(char) * 2;
				if (envia_paquete(sockfd, cliente_addr, &resp, len) == -1) {
					fprintf(stderr, "(servidor) fallo al enviar la respuesta al cliente\n");
					break;
				}
				printf("(servidor) eliminacion realizada con exito\n");
				break;
		
			default:
				printf("(servidor) operacion no soportada, adios\n");
				exit(1);
		}
	}

	close(sockfd);
}
