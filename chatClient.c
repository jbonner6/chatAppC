#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

int main(){
  int clientSocket;
  socklen_t addr_size;
  size_t nBytes = 100;

  struct addrinfo hints, *res;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  getaddrinfo("127.0.0.1", "3635", &hints, &res);

  clientSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);


  int status;
  status = connect(clientSocket, res->ai_addr, res->ai_addrlen);
  printf("%d\n\n", status);
  if (status < 0){
      return 0;
  }
  char testBuffer[1024];
  while (1){
      fgets(testBuffer,sizeof(testBuffer), stdin);
    //   printf("%s",testBuffer);
    //   strcat(testBuffer, "\n");
      send(clientSocket,testBuffer,sizeof(testBuffer),0);
      memset(&testBuffer[0], 0, sizeof(testBuffer));
  }
  close(clientSocket);


  return 0;

}
