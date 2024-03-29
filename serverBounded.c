#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int current_alive = 0;
int limit = 0;
int max = 0;

void trat_sigchld (int signum) {
    while (waitpid(-1, NULL, WNOHANG) > 0)
        --current_alive;
}

void doService(int fd) {
int i = 0;
char buff[80];
char buff2[80];
int ret;
int socket_fd = (int) fd;

    ret = read(socket_fd, buff, sizeof(buff));
    while(ret > 0) {
        buff[ret]='\0';
        sprintf(buff2, "Server [%d] received: %s\n", getpid(), buff);
        write(1, buff2, strlen(buff2));
        ret = write(fd, "caracola ", 8);
        if (ret < 0) {
            perror ("Error writing to socket");
            exit(1);
        }
        ret = read(socket_fd, buff, sizeof(buff));
    }
    if (ret < 0) {
            perror ("Error reading from socket");

    }
    sprintf(buff2, "Server [%d] ends service\n", getpid());
    write(1, buff2, strlen(buff2));
}

void doServiceBounded(int fd) {
    int pid = fork();
    switch (pid){
        case -1: perror("Error creating server process");
            exit(1);
        case 0:
            doService(fd);
            exit(0);
    }
    ++current_alive;
    if (current_alive > max) max = current_alive;
}


main (int argc, char *argv[])
{
  int socketFD;
  int connectionFD;
  char buffer[80];
  int ret;
  int port;


  if (argc != 3)
    {
      strcpy (buffer, "Usage: ServerSocket PortNumber [upper_bound]\n");
      write (2, buffer, strlen (buffer));
      exit (1);
    }

  if (argc == 3) {
    signal(SIGCHLD,trat_sigchld);
    limit = atoi(argv[2]);
  }
  port = atoi(argv[1]);
  socketFD = createServerSocket(port);
  if (socketFD < 0)
    {
      perror ("Error creating socket\n");
      exit (1);
    }

  while (current_alive < limit) {
	  connectionFD = acceptNewConnections (socketFD);
	  if (connectionFD < 0)
	  {
		  perror ("Error establishing connection \n");
		  deleteSocket(socketFD);
		  exit (1);
	  }

      doServiceBounded(connectionFD);
  }
  // int file = open("test", O_CREAT|O_WRONLY, 00777);
  // if (file < 0) perror("Error opening");
  // int i = 0;
  while (waitpid(-1, NULL, 0) > 0) { }
  //   i++;
  //   sprintf(buffer, "%d\n",i);
  //   write(file, buffer, strlen(buffer));
  // }
  printf("Maximum children served: %d \n", max);
}
