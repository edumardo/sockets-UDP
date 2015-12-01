/***
 *
 */
#include "client_func.h"
/******************************************************************************/
int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in servidor_addr;
	char id[MAXSIZE];

	unsigned long ip;
	unsigned short puerto;

	/* ./cient <ip-server> <operation> [id] */
	if ((argc < 3) || (argc > 4)) {
		printf("Usage: ./%s <ip-server> <operation> [id]\n", argv[0]);
		printf("operations: \n");
		printf("\t1 - Create new record in the database\n");
		printf("\t2 - Consult a record in the database\n");
		printf("\t3 - Delete a record in the database\n");
		exit(1);
	}

	/* Creamos el socket */
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("(cliente) error al crear el socket");
		exit(1);
	}
	
	/* puerto e ip del servidor */
	servidor_addr.sin_family = AF_INET;
	servidor_addr.sin_port = htons(PORT_SERVER);
	servidor_addr.sin_addr.s_addr = inet_addr(argv[1]);
	memset(&(servidor_addr.sin_zero), '\0', 8);

	/* Que operacion pide al servidor */
	switch(atoi(argv[2])) {
		
		/* Crear nueva entrada en el registro */
		case 1:
			if (argc == 4){
				if (strlen(argv[3]) >= sizeof(id)){
					printf("(cliente)El id es demasiado largo, prueba con uno mas corto\n");
					exit(1);
				}
				strcpy(id, argv[3]);
			}
			else {
				printf("(cliente) Introduce el identificador del nuevo registro: ");
				scanf("%s", id);
			}
			if (registra_registro(sockfd, servidor_addr, id) == -1) {
				printf("(cliente) Imposible crear el nuevo registro\n");
				exit(1);
			}
			break;
	
		/* Consultar entrada del registro */
		case 2:
			if (argc == 4){
				if (strlen(argv[3]) >= sizeof(id)){
					printf("(cliente)El id es demasiado largo, prueba con uno mas corto\n");
					exit(1);
				}
				strcpy(id, argv[3]);
			}
			else {
				printf("(cliente) Introduce el identificador del registro a consultar: ");
				scanf("%s", id);
			}
			if (consulta_registro(sockfd, servidor_addr, id, &ip, &puerto) == -1) {
				printf("(cliente) Imposible consultar el registro\n");
				exit(1);
			}
			struct in_addr dir;
			dir.s_addr = ip;
			printf("(cliente) Datos de %s: %s:%d\n", id, inet_ntoa(dir), puerto);
			break;

		/* Eliminar entrada del registro */
		case 3:
			if (argc == 4){
				if (strlen(argv[3]) >= sizeof(id)){
					printf("(cliente)El id es demasiado largo, prueba con uno mas corto\n");
					exit(1);
				}
				strcpy(id, argv[3]);
			}
			else {
				printf("(cliente) Introduce el identificador del registro a borrar: ");
				scanf("%s", id);
			}
			if (elimina_registro(sockfd, servidor_addr, id) == -1) {
				printf("(cliente) Imposible eliminar el registro\n");
				exit(1);
			}

			break;
		
		default:
			printf("Usage: ./%s <ip-server> <operation> [id]\n", argv[0]);
			printf("operations: \n");
			printf("\t1 - Create new record in the database\n");
			printf("\t2 - Consult a record in the database\n");
			printf("\t3 - Delete a record in the database\n");
			exit(1);
		
	}
	close(sockfd);
	return 0;
}
