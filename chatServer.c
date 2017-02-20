#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <stdlib.h>

bool hasNewLine(char buffer[10000], size_t size){
    for (int i = 0; i < size; i++){
        if (buffer[i] == '\n'){
            return true;
        }
    }
    return false;
}


int main(){
  int servSocket, newSocket;
  struct sockaddr_in serverAddr;
  struct sockaddr_storage incomingStorage;
  socklen_t addr_size;

  struct addrinfo hints, *res;
  int BACK_LOG = 10;


  // servSocket = socket(PF_INET, SOCK_STREAM, 0);
  // serverAddr.sin_family = AF_INET;
  // serverAddr.port = htons(8080);
  // inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr));
  // memset(serverAddr.sin_zero, 0, 8);

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  getaddrinfo("127.0.0.1", "3636", &hints, &res);

  servSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  bind(servSocket, res->ai_addr, res->ai_addrlen);

  listen(servSocket, BACK_LOG);

  addr_size = sizeof incomingStorage;
  while((newSocket = accept(servSocket, (struct sockaddr *)&incomingStorage, &addr_size)) > 0){
      printf("%s", "New Connection\n");
      char buffer[1024];
      while(1){
          while(!hasNewLine(buffer, 1024)){
              int received = recv(newSocket, buffer, sizeof buffer, 0);
              if (received < 0){
                  //error
                  continue;
              }
          }
          char *result = (char *)malloc(strlen(buffer)+1);
          strcpy(result,buffer);
          printf("%s", result);
          free(result);
          memset(&buffer[0], 0, sizeof(buffer));
      }
      close(newSocket);
  }

  // if (newSocket > 0){
  //     printf("%s", "New Connection\n");
  //     char buffer[1024];
  //     char holder[10000];
  //     const char n = '\n';
  //     while(!hasNewLine(holder, 10000)){
  //         int received = recv(newSocket, buffer, sizeof buffer, 0);
  //         strcat(holder, buffer);
  //
  //     }
  //     printf("%s", holder);
  // }


  return 0;

}
