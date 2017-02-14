include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(){
  int servSocket, newSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
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
  hints.ai_flags = AI_PASSIVE;

  getaddressinfo(NULL, "3636", &hints, &res);

  servSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  bind(servSocket, res->ai_addr, res->ai_addrlen);

  listen(servSocket, BACK_LOG);
  
}
