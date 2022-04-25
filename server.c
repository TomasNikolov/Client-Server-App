#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFSIZE 4096
#define PORT 8989
#define SOCKETERROR (-1)
#define SERVER_BACKLOG 100
#define TYPE_SIZE 30

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

typedef struct animalsData {
	char type[TYPE_SIZE];
}AnimalsData;

typedef struct animals {
	AnimalsData data;
	struct Animals *next;
}Animals;

void writeBinFile(Animals *head, char *fname);
Animals *readBinFile(char *fname);
void addNewAnimalsToFile(Animals *head, char *fname);
Animals *insertAnimals(Animals *head, AnimalsData animal);
Animals *insertAnimal();
Animals *createList();
void printAnimals(Animals *head);
void freeList(Animals *head);
int getCountOfAnimal(char animalType[]);
void *addNewAnimal(void* p_client_socket);
int check(int exp, const char *msg);
void *handleCountOfAnimals(void* p_client_socket);
void insertNewAnimal(char animalType[], int animalCount);
void *deleteAnimalThread(void* p_client_socket);
void deleteAnimal(char animalType[], int count);
Animals *deleteAnimalFromList(Animals *head, char animalType[], int count);

int main() {
	int server_socket, client_socket, addr_size;
	SA_IN server_addr, client_addr;

	check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Failed to create socket");
    
	server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
 
    
    check(bind(server_socket, (SA*)&server_addr, sizeof(server_addr)), "Bind Failed!");
 
    check(listen(server_socket, SERVER_BACKLOG), "Listen Failed!");
 
    while (1) {
		printf("Waiting for connections...\n");

        addr_size = sizeof(SA_IN);
        
        check(client_socket = accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size), "Accept Failed!");
        
		printf("Connected!\n");

        int choice = 0;
        recv(client_socket, &choice, sizeof(choice), 0);

        pthread_t t;
		int *pclient = malloc(sizeof(int));
		*pclient = client_socket;
        
        switch (choice) {
            case 1: {
                printf("User choice is: %d\n", choice);
                
		        pthread_create(&t, NULL, handleCountOfAnimals, pclient);
            
                break;
            }        
            case 2: {
                printf("User choice is: %d\n", choice);
		        
                pthread_create(&t, NULL, addNewAnimal, pclient);

                break;
            }
            case 3: {
                printf("User choice is: %d\n", choice);
		        
                pthread_create(&t, NULL, deleteAnimalThread, pclient);

                break;
            }
        }
    }
 
    return 0;
}

void *deleteAnimalThread(void* p_client_socket) {
	int client_socket = *((int*)p_client_socket);
	free(p_client_socket);
	char buffer[BUFSIZE];
	size_t bytesRead;
    ssize_t bytesSend;
    char animalType[TYPE_SIZE];
    int animalCount = 0;

    bzero(buffer, BUFSIZE);

    char message[] = "Enter type of animal to delete: ";

    bytesSend = send(client_socket, &message, sizeof(message), 0);

    printf("Bytes send: %zu\n", bytesSend);

    bytesRead = read(client_socket, &animalType, sizeof(buffer));

	bzero(buffer, BUFSIZE);

	printf("Bytes read: %zu\n", bytesRead);
	printf("Animal type: %s\n", animalType);

    bzero(buffer, BUFSIZE);

    char secondMessage[] = "Enter how many animals of this type to delete: ";

    bytesSend = send(client_socket, &secondMessage, sizeof(secondMessage), 0);

    printf("Bytes send: %zu\n", bytesSend);

    bzero(buffer, BUFSIZE);

    bytesRead = read(client_socket, &animalCount, sizeof(buffer));

	bzero(buffer, BUFSIZE);

	printf("Bytes read: %zu\n", bytesRead);
	printf("Animal count: %d\n", animalCount);
    printf("To delete: %d %ss\n", animalCount, animalType);

    deleteAnimal(animalType, animalCount);

    bzero(buffer, BUFSIZE);

    char lastMessage[] = "Animal successfull deleted";

    bytesSend = send(client_socket, &lastMessage, sizeof(lastMessage), 0);

    printf("Bytes send: %zu\n", bytesSend);

	close(client_socket);
	printf("Closing connection!\n");
	return NULL;
}

void *addNewAnimal(void* p_client_socket) {
	int client_socket = *((int*)p_client_socket);
	free(p_client_socket);
	char buffer[BUFSIZE];
	size_t bytesRead;
    ssize_t bytesSend;
    char animalType[TYPE_SIZE];
    int animalCount = 0;

    bzero(buffer, BUFSIZE);

    char message[] = "Enter type of animal: ";

    bytesSend = send(client_socket, &message, sizeof(message), 0);

    printf("Bytes send: %zu\n", bytesSend);

    bytesRead = read(client_socket, &animalType, sizeof(buffer));

	bzero(buffer, BUFSIZE);

	printf("Bytes read: %zu\n", bytesRead);
	printf("Animal type: %s\n", animalType);

    bzero(buffer, BUFSIZE);

    char secondMessage[] = "Enter how many animals of this type to insert: ";

    bytesSend = send(client_socket, &secondMessage, sizeof(secondMessage), 0);

    printf("Bytes send: %zu\n", bytesSend);

    bzero(buffer, BUFSIZE);

    bytesRead = read(client_socket, &animalCount, sizeof(buffer));

	bzero(buffer, BUFSIZE);

	printf("Bytes read: %zu\n", bytesRead);
	printf("Animal count: %d\n", animalCount);
    printf("To insert: %d %ss\n", animalCount, animalType);

    insertNewAnimal(animalType, animalCount);

    bzero(buffer, BUFSIZE);

    char lastMessage[] = "Animal successfull inserted";

    bytesSend = send(client_socket, &lastMessage, sizeof(lastMessage), 0);

    printf("Bytes send: %zu\n", bytesSend);

	close(client_socket);
	printf("Closing connection!\n");
	return NULL;
}

void *handleCountOfAnimals(void* p_client_socket) {
	int client_socket = *((int*)p_client_socket);
	free(p_client_socket);
	char buffer[BUFSIZE];
	size_t bytesRead;
    ssize_t bytesSend;
	char animalType[TYPE_SIZE];

    bzero(buffer, BUFSIZE);

    char message[] = "Enter type of animal: ";

    bytesSend = send(client_socket, &message, sizeof(message), 0);

    printf("Bytes send: %zu\n", bytesSend);

    bytesRead = read(client_socket, &animalType, sizeof(buffer));

	bzero(buffer, BUFSIZE);

	printf("Bytes read: %zu\n", bytesRead);
	printf("Animal type: %s\n", animalType);

    int countOfAnimal = getCountOfAnimal(animalType);

    bytesSend = send(client_socket, &countOfAnimal, sizeof(countOfAnimal), 0);
    printf("Bytes send: %zu\n", bytesSend);

	close(client_socket);
	printf("Closing connection!\n");
	return NULL;
}

int check(int exp, const char *msg) {
    if (exp == SOCKETERROR) {
        perror(msg);
        exit(1);
    }
    return exp;
}

void deleteAnimal(char animalType[], int count) {
    Animals *head = NULL;
    head = readBinFile("Animals.bin");
    head = deleteAnimalFromList(head, animalType, count);
    writeBinFile(head, "Animals.bin");
    freeList(head);
    head = NULL;
}

void insertNewAnimal(char animalType[], int animalCount) {
    for (int i = 0; i < animalCount; i++) {
        Animals *head = NULL;
        head = insertAnimal(animalType);
        addNewAnimalsToFile(head, "Animals.bin");
        printAnimals(head);

        freeList(head);
        head = NULL;
    }
}

int getCountOfAnimal(char animalType[]) {
    Animals *p;
    Animals *head;
    int counter = 0;

    head = readBinFile("Animals.bin");
    printAnimals(head);
    printf("\n\n");
    
    for(p = head; p != NULL; p = p->next) {
        if(strcmp(p->data.type, animalType) == 0) {
            counter++;
        }
    }
    
    freeList(head);
    head = NULL;

    return counter;
}

Animals *deleteAnimalFromList(Animals *head, char animalType[], int count) {
    Animals *current = head;
    Animals *previous = head;
    int flag = 0;
    int counter = 0;

    while(current != NULL) {
        if((strcmp(current->data.type, animalType) == 0) && count > counter) {
            if(current == head) {
                head = head->next;
                previous = head;
                flag = 1;
            } else {
                previous->next = current->next;
                free(current);
                current = previous;
                counter++;
            }
        }

        previous = current;
        if(flag == 0) {
            current = current->next;
        }
        flag = 0;
    }

    return head;
}

void writeBinFile(Animals *head, char *fname) {
    Animals *p;
    FILE *bout = fopen(fname,"wb");

    if(bout == NULL) {
        printf("Error");
        return;
    }

    for(p = head; p != NULL; p = p->next) {
        if(fwrite(&p->data, sizeof(AnimalsData), 1, bout) != 1) {
            printf("Error");
            fclose(bout);
            return;
        }
    }

	printf("Succes!\n");
    fclose(bout);
}

Animals *readBinFile(char *fname) {
    Animals *animal = NULL;
    AnimalsData animalData;
    FILE *bin = fopen(fname,"rb");

    if(bin == NULL) {
        printf("ERROR");
        return NULL;
    }

    while(1) {
        if(fread(&animalData, sizeof(animalData), 1, bin) != 1) {
            if(!feof(bin)) {
                printf("Error");
            }

            fclose(bin);
            return animal;
        }

        animal = insertAnimals(animal, animalData);
    }

    fclose(bin);
}

void addNewAnimalsToFile(Animals *head, char *fname) {
    Animals *p;
    FILE *bout = fopen(fname,"ab");

    if(bout == NULL) {
        printf("Error");
        return;
    }

    for(p = head; p != NULL; p = p->next) {
        if(fwrite(&p->data, sizeof(AnimalsData), 1, bout) != 1) {
            printf("Error");
            fclose(bout);
            return;
        }
    }

    fclose(bout);
}

Animals *insertAnimals(Animals *head, AnimalsData animal) {
    Animals *element = (Animals*)malloc(sizeof(Animals));

    if(element == NULL) {
        printf("Error");
        freeList(head);
        exit(1);
    }

    element->data = animal;
    if(head == NULL) {
        element->next = NULL;
    }
    else {
        element->next = head;
    }
    head = element;

    return head;
}

Animals *createList() {
    Animals *head = NULL;
    Animals *last = NULL;
    int i;

    for(i = 0; i < 11; i++) {
        char animalType[20];
        printf("Enter animal type: ");
        scanf("%s", animalType);

        if(head == NULL) {
            head = insertAnimal(animalType);
            last = head;
        }
        else {
            last->next = insertAnimal(animalType);
            last = last->next;
        }
    }

    return head;
}

Animals *insertAnimal(char animalType[]) {
    Animals *p = NULL;
    p = (Animals*)malloc(sizeof(Animals));

    strcpy(p->data.type, animalType);

    p->next = NULL;
    printf("\nAdd new animal : SUCCESSFUL\n\n");

    return p;
}

void printAnimals(Animals *head) {
    Animals *current = head;
    while(current != NULL) {
        printf("Animal: %s\n",current->data.type);
        current = current->next;
    }
}

void freeList(Animals *head) {
    Animals *current = head;
	while(head != NULL)
	{
		head = head->next;
		free(current);
		current = head;
	}
}
