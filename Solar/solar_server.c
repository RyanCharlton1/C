#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <time.h>
#include <poll.h>
#include <pthread.h>

#include "Maths/vec4.h"
#include "Solar/planet.h"

#define BUFFERSIZE 512
#define SSPORT 1234
#define MAXCLIENTS 8

#define SEC 1000
#define MIN 60 * SEC

#define TRUE 1
#define FALSE 0

int QUIT = 0;

void print_error(){
    printf("%d:%s\n", errno, strerror(errno));
}

typedef struct{
    int socket_id;
    Planet *planets;
    //client num will be used to index arrays
    int client_num;
} client_thread_arg;

void client_thread(client_thread_arg *args){
    char buffer[32];
    printf("reading socket %d\n", args->socket_id);



    do
    {
        //read all data until ewouldblock
        //any other error will close socket
        int len  = recv(args->socket_id, buffer, sizeof(buffer), 0);
        int dummy = 10;
        if(len < 0){
            if(errno != EWOULDBLOCK)
                printf("recv failed\n");

            break;
        }
        //connection closed by client
        if(len == 0){
            printf("closed socket %d\n", args->socket_id);
            break;
        }
        printf("socket %d : %s\n", args->socket_id, buffer);
    } while(!QUIT);
    
    close(args->socket_id);
    //clean up all connections accociated data
    free(args); 
    pthread_detach(pthread_self());
}


//master socket thread to establish new connections
void master_socket_thread(){
    int master_socket, new_socket;
    struct sockaddr_in address;
    int client_count = 0;
    pthread_t *client_threads = NULL;
    int *retvals = NULL;

    //master socket to listen for incoming connections
    if((master_socket = socket(PF_INET, SOCK_STREAM, 0)) == 0){
        printf("master socket create failed\n");
        print_error();
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections(reuseable)
    int opt = TRUE;
    if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0){
        printf("master socket option failed\n");
        print_error();
        close(master_socket);
        exit(EXIT_FAILURE);
    }
    
    if(ioctl(master_socket, FIONBIO, (char*)&opt, sizeof(opt)) < 0){
        printf("master socket could not be set to nonblocking\n");
        print_error();
        close(master_socket);
        exit(EXIT_FAILURE);
    }

    //create address struct for master socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SSPORT);

    //bind master socket to address struct
    if(bind(master_socket, (struct sockaddr*)&address, sizeof(address)) < 0){
        printf("master socket bind failed\n");
        print_error();
        close(master_socket);
        exit(EXIT_FAILURE);
    }

    printf("master socket created on port %d\n", SSPORT);

    //sets socket to listen for new connections
    if(listen(master_socket, MAXCLIENTS) < 0){
        printf("master socket listen failed");
        print_error();
        close(master_socket);
        exit(EXIT_FAILURE);
    }

    int address_size = sizeof(address);
    printf("waiting for connections...\n");

    //loop listening and accepting connections until quit
    struct sockaddr *client_info;
    while(!QUIT){
        //client_info = malloc(sizeof(struct sockaddr));
        new_socket = accept(master_socket, NULL, NULL);

        if(new_socket < 0){
            //if fails with ewouldblock all connections have been accepted
            //otherwise stop server
            if(errno != EWOULDBLOCK){
                printf("accept failed\n");
                QUIT = TRUE;
            }
            //print_error();
            continue;
        }

        printf("new connection %d\n", new_socket);

        client_thread_arg *args = malloc(sizeof(client_thread_arg));
        args->client_num = client_count;
        args->socket_id = new_socket;

        client_count++;
        client_threads = realloc(client_threads, client_count * sizeof(pthread_t));
        retvals = realloc(retvals, client_count * sizeof(int));
        retvals[client_count] = pthread_create(&client_threads[client_count - 1], NULL, client_thread, args);
    }

    for(int i = 0; i < client_count; i++)
        pthread_join(client_threads[i], NULL);

    pthread_detach(pthread_self());
    close(master_socket);
    free(retvals);
    free(client_threads);
}

int main(int argc, char** argv){
    pthread_t listener = NULL;
    int error_code = pthread_create(&listener, NULL, master_socket_thread, NULL);
    while(!QUIT){
        if(getchar() == 'q')
            QUIT = TRUE;
    }

    ////wait for listener thread to close 
    pthread_join(listener, NULL);

    return 0;
}