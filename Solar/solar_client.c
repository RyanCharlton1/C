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

#include <glad/include/glad/glad.h>
#include <GLFW/glfw3.h>

#include "Solar/planet.h"

#define LOCALHOST "127.0.0.1"
#define SSPORT 1234

#define TESTMSG "this is a test\n"

void print_error(){
    printf("%d:%s\n", errno, strerror(errno));
}

void server_connection_thread(){
    int server_socket;
    struct sockaddr_in address;
    struct hostent *hostptr;

    //create address struct for server socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SSPORT);
    //create 
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

    write(server_socket, TESTMSG, sizeof(TESTMSG));

    close(server_socket);
    pthread_detach(pthread_self());
}

int main(int argc, char **argv){
    
    return 0;
}