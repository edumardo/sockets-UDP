#include "server_func.h"
/******************************************************************************/
/**
 *
 *
 */
void end_server(int sig) {
	printf("\n");
	delete_database();
	printf("(server) Time to say goodbye!, signal received: %d\n", sig);
	exit(0);
}

