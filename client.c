#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <netdb.h>

#define MESSAGE_BUFFER 100
#define PORT 8989
#define SA struct sockaddr
#define TYPE_SIZE 30
#define BUFSIZE 4096

void *clientChoice(void *args);
void getCountOfAnimals(int serverSocket);
void addNewAnimal(int serverSocket);
void deleteAnimal(int serverSocket);

int main() {
    int choice;
    pthread_t tid;

    while (1) {
        printf("\n\n\n");
        printf(".:: MENU ::.\n");
        printf("1. Show count of given animal. \n");
        printf("2. Add new animal.  \n");
        printf("3. Delete animal. \n");
        printf("4. EXIT. \n");
        
        printf("Enter choice: ");
        scanf("%d",&choice);
      
        if(choice < 1 || choice > 4) {
            perror("Wrong choice!!!");
            return 0;
        }

        if (choice == 4) {
            return 0;
        }
        
        pthread_create(&tid, NULL, clientChoice, &choice);
        pthread_join(tid, NULL);
    }

    return 0;
}

void *clientChoice(void *args) {
    int serverSocket;
    ssize_t bytesSend;
    size_t bytesRead;
    char buffer[BUFSIZE];
    int clientRequest = *((int*)args);
    char message[MESSAGE_BUFFER];
    int countOfAnimal;
 
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
 
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
 
    int connectionStatus = connect(serverSocket, (struct sockaddr*)&server_address, sizeof(server_address));
 
    if (connectionStatus < 0) {
        puts("Error\n");
        return 0;
    }
 
    printf("Connection established\n");
    printf("Choice: %d\n", clientRequest);

    bytesSend = send(serverSocket, &clientRequest, sizeof(clientRequest), 0);
    printf("Bytes send: %zu\n", bytesSend);

    switch (clientRequest) {
        case 1: {
            printf("User choice is: %d\n", clientRequest);
            
            getCountOfAnimals(serverSocket);
            
            break;
        }
        case 2: {
            printf("User choice is: %d\n", clientRequest);

            addNewAnimal(serverSocket);

            break;
        }
        case 3: {
            printf("User choice is: %d\n", clientRequest);

            deleteAnimal(serverSocket);

            break;
        }
    }

    close(serverSocket);
    pthread_exit(NULL);
 
    return 0;
}

void deleteAnimal(int serverSocket) {
    ssize_t bytesSend;
    size_t bytesRead;
    char buffer[BUFSIZE];
    char message[MESSAGE_BUFFER];
    char typeOfAnimal[TYPE_SIZE];
    int countOfAnimal = 0;;
    
    bzero(buffer, sizeof(buffer));
    bytesRead = read(serverSocket, &message, sizeof(buffer));
    printf("\n");
    printf("%s", message);

    bzero(buffer, sizeof(buffer));

    scanf("%s", typeOfAnimal);
    printf("Animal type to send: %s\n", typeOfAnimal);

    bytesSend = send(serverSocket, &typeOfAnimal, sizeof(typeOfAnimal), 0);
    printf("Bytes send: %zu\n", bytesSend);

    bzero(buffer, sizeof(buffer));
 
    bytesRead = read(serverSocket, &message, sizeof(buffer));
    printf("\n");
    printf("%s", message);

    bzero(buffer, sizeof(buffer));

    scanf("%d", &countOfAnimal);
    printf("Animal count: %d\n", countOfAnimal);

    bytesSend = send(serverSocket, &countOfAnimal, sizeof(countOfAnimal), 0);
    printf("Bytes send: %zu\n", bytesSend);

    bzero(buffer, sizeof(buffer));
 
    bytesRead = read(serverSocket, &message, sizeof(buffer));
    printf("Bytes read: %zu\n", bytesRead);
	printf("%s\n", message);
}

void addNewAnimal(int serverSocket) {
    ssize_t bytesSend;
    size_t bytesRead;
    char buffer[BUFSIZE];
    char message[MESSAGE_BUFFER];
    char typeOfAnimal[TYPE_SIZE];
    int countOfAnimal = 0;
    
    bzero(buffer, sizeof(buffer));
    bytesRead = read(serverSocket, &message, sizeof(buffer));
    printf("\n");
    printf("%s", message);

    bzero(buffer, sizeof(buffer));

    scanf("%s", typeOfAnimal);
    printf("Animal type to send: %s\n", typeOfAnimal);

    bytesSend = send(serverSocket, &typeOfAnimal, sizeof(typeOfAnimal), 0);
    printf("Bytes send: %zu\n", bytesSend);

    bzero(buffer, sizeof(buffer));
 
    bytesRead = read(serverSocket, &message, sizeof(buffer));
    printf("\n");
    printf("%s", message);

    bzero(buffer, sizeof(buffer));

    scanf("%d", &countOfAnimal);
    printf("Animal count: %d\n", countOfAnimal);

    bytesSend = send(serverSocket, &countOfAnimal, sizeof(countOfAnimal), 0);
    printf("Bytes send: %zu\n", bytesSend);

    bzero(buffer, sizeof(buffer));
 
    bytesRead = read(serverSocket, &message, sizeof(buffer));
    printf("Bytes read: %zu\n", bytesRead);
	printf("%s\n", message);
}

void getCountOfAnimals(int serverSocket) {
    ssize_t bytesSend;
    size_t bytesRead;
    char buffer[BUFSIZE];
    char message[MESSAGE_BUFFER];
    char typeOfAnimal[TYPE_SIZE];
    int countOfAnimal;

    bzero(buffer, sizeof(buffer));
    bytesRead = read(serverSocket, &message, sizeof(buffer));
    printf("\n");
    printf("%s", message);

    bzero(buffer, sizeof(buffer));

    scanf("%s", typeOfAnimal);
    printf("Animal type to send: %s\n", typeOfAnimal);

    bytesSend = send(serverSocket, &typeOfAnimal, sizeof(typeOfAnimal), 0);
    printf("Bytes send: %zu\n", bytesSend);

    bzero(buffer, sizeof(buffer));

    bytesRead = read(serverSocket, &countOfAnimal, sizeof(buffer));
    printf("Bytes read: %zu\n", bytesRead);
	printf("Count of %s is %d\n", typeOfAnimal, countOfAnimal);
}