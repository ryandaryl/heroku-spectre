#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "spectre.h"

int open_connection(int port) {
  int sock;
  struct sockaddr_in addr_in;

  sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock == -1) {
    printf("Failed to create socket (%d)\n", errno);
    exit(EXIT_FAILURE);
  }

  addr_in.sin_family = AF_INET;
  addr_in.sin_port = htons(port);
  addr_in.sin_addr.s_addr = INADDR_ANY;

  bind(sock, (struct sockaddr *) &addr_in, sizeof(struct sockaddr_in));

  if (listen(sock, 8) == -1) {
    printf("Failed to get socket to listen (%d)\n", errno);
    exit(EXIT_FAILURE);
  }

  return sock;
}

void accept_client(int sock) {
  struct sockaddr_in client_addr;
  socklen_t clientaddr_len;
  FILE *f;

  int client_sock = accept(sock, (struct sockaddr *)&client_addr, &clientaddr_len);
  if (client_sock == -1) {
    printf("Failed to accept connection (%d)\n", errno);
    exit(EXIT_FAILURE);
  }

  // Redirect printf logs to text file.
  char* logfile = "output.txt";
  freopen(logfile, "w", stdout);

  spectre();

  // Read contents of logfile and return HTTP response.

  f = fdopen(client_sock, "w+");
  FILE *fp;
  fp = fopen(logfile, "r");
  char buff[255];
  fprintf(f, "HTTP/1.1 200 OK\r\n");
  fprintf(f, "Content-Type: application/json\r\n");
  fprintf(f, "\r\n");
  fprintf(f, "{\"%s\": \"", "ouput");
  for (int i = 0; i < 36; i++) {
    fgets(buff, 255, (FILE*)fp);
    fprintf(f, "%s", buff);
  }
  fprintf(f, "\"}");
  remove(logfile);
  fclose(f);

  // stdout needs to be flushed in order for heroku to read the logs
  fflush(stdout);
}

int main(int argc, char **argv) {
  int port;

  if (argc != 2) {
    printf("%s [port-number]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  port = atoi(argv[1]);
  int sock = open_connection(port);

  while (1) {
    accept_client(sock);
  }

  close(sock);
  exit(EXIT_SUCCESS);
}