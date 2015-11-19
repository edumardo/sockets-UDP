/***
 *
 */
#include "common.h"
/**
 * Envia un paquete al servidor
 * @param sockfd: socket de comunicacion.
 * @param server_addr: direccion del servidor.
 * @param paquete: datos a enviar.
 * @param len: longitud del paquete a enviar
 * @return 0 si todo fue bien, -1 en caso contrario.
*/
/******************************************************************************/
int envia_paquete(int sockfd, struct sockaddr_in server_addr, void * paquete, int len)
{
	int numbytes;

	if ((numbytes =  sendto(sockfd, 
													paquete, 
													len, 
													0, 
													(struct sockaddr *) &server_addr,
													sizeof(struct sockaddr))) == -1) {
		perror("(common) sendto");
		return -1;
	}

	return 0;
}
/******************************************************************************/
/**
 * Recibe un paquete del servidor
 * @param sockfd: socket de comunicacion.
 * @param server_addr: direccion del servidor.
 * @return el paquete si todo fue bien, NULL en caso contrario.
*/
char * recibe_paquete(int sockfd, struct sockaddr_in server_addr)
{
	int numbytes;
	char buffer[MAXBUFLEN];
	socklen_t addr_len;

	memset(buffer, 0, MAXBUFLEN);
	addr_len = sizeof(struct sockaddr);	

	if ((numbytes = recvfrom(sockfd,
													 buffer,
													 MAXBUFLEN - 1,
													 0,
													 (struct sockaddr *) &server_addr,
													 &addr_len)) == -1) {
		perror("(common) recvfrom");
		return NULL;
	}

	buffer[numbytes] = '\0';
	return buffer;
}
