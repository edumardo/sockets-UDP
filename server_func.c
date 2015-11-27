#include "server_func.h"
/******************************************************************************/
/**
 * End the server. Delete the database and exit. Now, the sig parameter is the 
 * signal SIGINT (Ctrl + C).
 * @param sig: catched signal identifier.
 */
void end_server(int sig) {
	printf("\n");
	delete_database();
	printf("(server) Time to say goodbye!, signal received: %d\n", sig);
	exit(0);
}

