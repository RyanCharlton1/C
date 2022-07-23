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

#include <glad/include/glad/gl.h>
#include <GLFW/glfw3.h>

#include "Solar/planet.h"
#include "Solar/solar.h"

int QUIT = FALSE;

Planet client_planets[MAXCLIENTS];
Planet server_planets[SERVERPLANETS];
int player_index;

void print_client_planets(){
    for(int i = 0; i < MAXCLIENTS; i++)
        printf("client_planets[%d]:%s\n", i, client_planets[i].name);
}

void print_server_planets(){
    for(int i = 0; i < SERVERPLANETS; i++)
        printf("server_planets[%d]:%s\n", i, server_planets[i].name);
}

void print_error(){
    printf("%d:%s\n", errno, strerror(errno));
}

void server_connection_thread(char name[NAMEMAX]){
    int server_socket;
    struct sockaddr_in address;
    struct hostent *hostptr;

    //create address struct for server socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SSPORT);
    //create ip struct
    hostptr = gethostbyname(LOCALHOST);
    memcpy(&address.sin_addr, hostptr->h_addr, hostptr->h_length);

    server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(server_socket < 0){
        printf("create socket failed\n");
        print_error();
        return -1;
    }

    if(connect(server_socket, &address, sizeof(address)) < 0){
        printf("failed to connect\n");
        print_error();
        return -1;
    }

    //sends player name 
    write(server_socket, name, strlen(name) + 1);
    //receive map planets
    recv(server_socket, server_planets, sizeof(server_planets), 0);
    //receive client planets
    recv(server_socket, client_planets, sizeof(client_planets), 0);
    //receive index for which planet is players
    recv(server_socket, &player_index, sizeof(player_index), 0);
    //loop sending player planet and recieving other clients and server data
    int len;
    while(!QUIT){
        write(server_socket, &client_planets[player_index], sizeof(Planet));
        
        len = recv(server_socket, &client_planets, sizeof(client_planets), 0);
        if(len < 0){
            print_error();
            break;
        }
        if(len == 0){
            printf("server closed connection\n");
            break;
        }
        
        len = recv(server_socket, &server_planets, sizeof(server_planets), 0);
        if(len < 0){
            print_error();
            break;
        }
        if(len == 0){
            printf("server closed connection\n");
            break;
        }

        printf("\033[H\033[J");
        print_client_planets();
        print_server_planets();
    }

    
    close(server_socket);  
    pthread_detach(pthread_self());
}
//args: "palyer name"
int main(int argc, char **argv){

    //thread will update planet data
    memset(client_planets, 0, sizeof(client_planets));
    pthread_t server_connection;
    pthread_create(&server_connection, NULL, server_connection_thread, argv[1]);
    
    

    //QUIT = TRUE;
    pthread_join(server_connection, NULL);
    return 0;
}
