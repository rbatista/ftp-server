/* server.c - Example of an Internet TPC server */
/* Compile with gcc -D_GNU_SOURCE server.c -o server */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "utils.h"


#define PORT    5555  /* The well-nown port. */

#define BUFFER_SIZE 512

int
main (int argc, char **argv)
{

  int socketfd;                       /* Socket file descriptor  */
  struct sockaddr_in server_name;     /* server socket's address */
  struct sockaddr_in client_name;     /* client socket's address */
  
  fd_set active_fd_set, read_fd_set;  /* Array of file descriptors */

  int nbytes;
  char buffer[BUFFER_SIZE];
		  

  /* Create the socket. 
  
     socket (NAMESPACE, STYLE, PROTOCOL)

       - namespace: local, other, internet: PF_INET || PF_INET6 

       - style: SOCK_STREAM, SOCK_DGRAM

       - protocol: 0 means 'auto' */

  socketfd = socket (PF_INET, SOCK_STREAM, 0); 

  if (socketfd < 0)
    fatal ();

  /* Give the socket a name. */

  server_name.sin_family = AF_INET;                   /* Ineternet family */
  server_name.sin_port = htons (PORT);                /* Well-known port  */
  server_name.sin_addr.s_addr = htonl (INADDR_ANY);   /* Any internetface */

  /* Bind the socket to the port. */

  if (bind (socketfd, (struct sockaddr *) 
	    &server_name, sizeof (server_name)) < 0)
    fatal ();

  /* Tell the OS to start to listen to the socket. */

  if (listen (socketfd, 1) < 0)
    fatal ();

      /* Initialize the set of active sockets. */
      
      FD_ZERO (&active_fd_set);
      FD_SET (socketfd, &active_fd_set);
      



  for (;;)
    {
      int i;

      /* Block until there is activity in the socket. */

      read_fd_set = active_fd_set;

      if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
	fatal ();

      
      /* Service all the sockets with input pending. */

      for (i = 0; i < FD_SETSIZE; ++i)
	{
	  if (FD_ISSET (i, &read_fd_set))
	    {
	      
	      if (i == socketfd)
		{
		  /* Connection request on original socket. */
		  
		  int new;
		  size_t size;
		  
		  /* Accept the the connection and associate it
		     with a new socket. */
		  
		  size = sizeof (client_name);
		  new = accept (socketfd,
				(struct sockaddr *) &client_name,
				&size);
		  if (new < 0)
		    fatal ();
		  
		  fprintf (stderr,
			   "Server: connect from host %s, port %hd.\n",
			   inet_ntoa (client_name.sin_addr),
			   ntohs (client_name.sin_port));

		  /* Add the new socket to the set of active fd */
		  
		  FD_SET (new, &active_fd_set); 
		  
		} 
	      else
		{
  
		  /* Data arriving on an already-connected socket. */
		  
		  nbytes = read (i, buffer, BUFFER_SIZE);

		  buffer[nbytes-1] = 0;

		  if (nbytes ==0) /* Cient ended the transmission */
		    {
		      printf ("End of transmission\n");
		      close (i);
		      FD_CLR (i, &active_fd_set);
		      
		    }
		  else if (nbytes < 0)      /* Read error */
		    {
		      fprintf (stderr, "Read error.\n");
		      close (i);
		      FD_CLR (i, &active_fd_set);
		    }
		  else
		    printf ("Client on socket %d said %s\n", i, buffer);
		}
	    }
	}
	    
    }

}




