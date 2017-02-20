#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

pthread_mutex_t lock;

bool hasNewLine(char buffer[10000], size_t size){
    for (int i = 0; i < size; i++){
        if (buffer[i] == '\n'){
            return true;
        }
    }
    return false;
}

void* listen_for_incoming(void* in_socket){
    while(1){
        sleep(5);
        pthread_mutex_lock(&lock);
        printf("%s", "\nEntered listen thread\n>> ");
        fflush(stdout);
        pthread_mutex_unlock(&lock);
    }
    int socket = *((int *) in_socket);
    int BACK_LOG = 10;
    socklen_t addr_size;
    int newSocket;
    struct sockaddr_storage incomingStorage;
    listen(socket, BACK_LOG);

    addr_size = sizeof incomingStorage;
    while((newSocket = accept(socket, (struct sockaddr *)&incomingStorage, &addr_size)) > 0){

        pthread_mutex_lock(&lock);
        printf("%s", "New Connection\n");
        pthread_mutex_unlock(&lock);

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

            pthread_mutex_lock(&lock);
            printf("%s", result);
            pthread_mutex_unlock(&lock);

            free(result);
            memset(&buffer[0], 0, sizeof(buffer));
        }
        close(newSocket);
    }
    return NULL;
}

int main(int argc, char *argv[]){
  int clientSocket;

  struct addrinfo hints, *res;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  getaddrinfo("127.0.0.1", "3636", &hints, &res);

  clientSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (!clientSocket){
      perror("Socket creation error");
      exit(-1);
  }


  int status;
  status = connect(clientSocket, res->ai_addr, res->ai_addrlen);
  if (status){
      perror("Connection not established");
      exit(-1);
  } else {
      printf("%s", "Connection successfully established\n\n");
  }

  pthread_t listen_thread;
  pthread_create(&listen_thread, NULL, listen_for_incoming, &clientSocket);

  char testBuffer[1024];
  while (1){
      printf(">> ");
      fgets(testBuffer,sizeof(testBuffer), stdin);
      send(clientSocket,testBuffer,sizeof(testBuffer),0);
      memset(&testBuffer[0], 0, sizeof(testBuffer));
  }

  pthread_join(listen_thread, NULL);
  close(clientSocket);


  return 0;

}
