#include "server_func.h"
/******************************************************************************/
/**
 *
 *
 */
void end_server(int sig) {
	printf("(server)Capturo Ctrl+C, adios!\n");
	/*remove_bbdd();*/
	exit(1);
}

