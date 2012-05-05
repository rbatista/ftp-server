/* client.c - Example of an Internet TPC client */
/* Compile with gcc -D_GNU_SOURCE client.c -o client */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "utils.h"		


#define PORT    5555  /* The well-nown port. */

#define BUFFER_SIZE 512

#define MESSAGE "Hello World of Sockets"

int
main (int argc, char **argv)
{

  int socketfd;                /* Socket file descriptor.  */

  struct sockaddr_in name;     /* The socket address.    */
  struct hostent *hostinfo;    /* The server information */

  char * hostname;

  int nbytes;
  /* char buffer[BUFFER_SIZE]; */

  /* argv[1] is the hostname where we want to connect to */

  if (argc < 2)        
    {
      fprintf (stderr, "Usage: client <server host name>\n");
      return EXIT_FAILURE;
    }
  hostname = argv[1];


  /* Create the socket. 
       
  socket (NAMESPACE, STYLE, PROTOCOL)
  
  - namespace: local, other, internet: PF_INET || PF_INET6 
  
  - style: SOCK_STREAM, SOCK_DGRAM
  
  - protocol: 0 means 'auto' */
  
  socketfd = socket (PF_INET, SOCK_STREAM, 0); 
  
  if (socketfd < 0)
    fatal ();
  
  /* Give the socket a name. */

  name.sin_family = AF_INET;                   /* Ineternet family */
  name.sin_port = htons (PORT);                /* Well-known port  */
  
  //name.sin_addr.s_addr = htonl (INADDR_ANY);   /* Any internetface */

  hostinfo = gethostbyname (hostname);

  if (!hostinfo)
    fatal ();

  name.sin_addr = *(struct in_addr *) hostinfo->h_addr;

  if ( connect (socketfd, & name, sizeof (name)))
    fatal ();

 
  nbytes = write (socketfd, MESSAGE, strlen (MESSAGE) + 1);
  
  close (socketfd);

  return EXIT_SUCCESS;

}
