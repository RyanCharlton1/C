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
#define MAXCONNECTIONS 8
#define MAXCLIENTS 8

#define SEC 1000
#define MIN 60 * SEC

#define TRUE 1
#define FALSE 0

int QUIT = 0;

void print_error(){
    printf("%d:%s\n", errno, strerror(errno));
}

//master socket thread to establish new connections
void master_socket_thread(){
    int master_socket, new_socket, socket_count = 1;
    struct sockaddr_in address;
    struct pollfd socket_set[1 + MAXCLIENTS];
    char buffer[32];

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

    memset(socket_set, 0, sizeof(socket_set));
    //set master socket as socket 0 to listen on
    socket_set[0].fd = master_socket;
    socket_set[0].events = POLLIN;

    int address_size = sizeof(address);
    printf("waiting for connections...\n");

    //loop listening and accepting connections until quit
    while(!QUIT){
        //wait until 
        if(poll(socket_set, socket_count, 2 * MIN) <= 0){
            printf("master socket poll failed\n");
            print_error();
            exit(EXIT_FAILURE);
        }

        int current_socket_count = socket_count;
        for(int i = 0; i < current_socket_count; i++){
            
            //skip if no events
            if(socket_set[i].revents == 0)
                continue;

            //quit if event was not POLLIN
            if(socket_set[i].revents != POLLIN){
                printf("ERROR! revents %d\n", socket_set[i].revents);
                    QUIT = TRUE;
                    break;
            }

            if(socket_set[i].fd == master_socket){
                printf("reading master socket\n");

                //accept all incoming connections
                do{
                    new_socket = accept(master_socket, NULL, NULL);
                    if(new_socket < 0){
                        //if fails with ewouldblock all connections have been accepted
                        //otherwise stop server
                        if(errno != EWOULDBLOCK){
                            printf("accept failed\n");
                            QUIT = TRUE;
                        }
                        break;
                    }

                    //add new socket to socket_set
                    printf("new connection %d\n", new_socket);
                    socket_set[socket_count].fd = new_socket;
                    socket_set[socket_count].events = POLLIN;
                    socket_count++;

                } while(new_socket != -1);
            }
            //if not master socket read client socket
            else{
                printf("reading socket %d\n", socket_set[i].fd);
                int close_socket = FALSE;

                //do
                //{
                    //read all data until ewouldblock
                    //any other error will close socket
                    int len  = recv(socket_set[i].fd, buffer, sizeof(buffer), 0);
                    int dummy = 10;
                    if(len < 0){
                        if(errno != EWOULDBLOCK){
                            printf("recv failed\n");
                            close_socket = TRUE;
                        }
                        break;
                    }

                    //connection closed by client
                    if(len == 0){
                        printf("closed socket %d\n", socket_set[i].fd);
                        close_socket = TRUE;
                        break;
                    }

                    printf("socket %d : %s\n", socket_set[i].fd, buffer);
                //} while(TRUE);
                
                close(socket_set[i].fd);
                socket_set[i].fd = -1;
                for(int i = 0; i < socket_count; i++){
                    if(socket_set[i].fd == -1){
                        for(int j = i; j < socket_count - 1; j++)
                            socket_set[j].fd = socket_set[j + 1].fd;

                        i--;
                        socket_count--;
                    }
                }
            }

        }
    }

    for(int i = 0; i < socket_count; i++)
        if(socket_set[i].fd >= 0)
            close(socket_set[i].fd);
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
    //for(int i = 0;i < MAXCLIENTS; i++)
    //    if(client_socket[i])
    //        close(client_socket[i]);

    return 0;
}