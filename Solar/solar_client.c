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

#include "Solar/models/model.h"
#include "Solar/planet.h"
#include "Solar/solar.h"


#define WIDTH 800
#define HEIGHT 600


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

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        QUIT = TRUE;
        glfwSetWindowShouldClose(window, TRUE);
    }
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

    GLFWwindow *window;
    int loglen = 0;
    if(!glfwInit()){
        QUIT = FALSE;
        pthread_join(server_connection, NULL);
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Solar Client", NULL, NULL);
    if(!window){
        glfwTerminate();
        QUIT = FALSE;
        pthread_join(server_connection, NULL);
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, key_callback);

    int vert, frag;
    int prog;
    vert = glCreateShader(GL_VERTEX_SHADER);
    frag = glCreateShader(GL_FRAGMENT_SHADER);
    //open file
    FILE *vertfile = fopen("shaders/planet.vert", "r");
    FILE *fragfile = fopen("shaders/planet.frag", "r");
    //find file lengths
    fseek(vertfile, 0, SEEK_END);
    int vertlen = ftell(vertfile);
    rewind(vertfile);

    fseek(fragfile, 0, SEEK_END);
    int fraglen = ftell(fragfile);
    rewind(fragfile);
    //read files
    char *vertsrc = malloc(vertlen + 1);
    char *fragscr = malloc(fraglen + 1);

    vertsrc[vertlen] = '\0';
    fragscr[fraglen] = '\0';

    fread(vertsrc, vertlen, 1, vertfile);
    fread(fragscr, fraglen, 1, fragfile);
    //set shader source
    glShaderSource(vert, 1, &vertsrc, NULL);
    glShaderSource(frag, 1, &fragscr, NULL);

    glCompileShader(vert);
    glCompileShader(frag);
    //print shader compile info
    int vertsuccess = 0;
    int fragsuccess = 0;

    glGetShaderiv(vert, GL_COMPILE_STATUS, &vertsuccess);
    glGetShaderiv(frag, GL_COMPILE_STATUS, &fragsuccess);

    if(vertsuccess == FALSE){
        glGetShaderiv(vert, GL_INFO_LOG_LENGTH, &loglen);
        char *infolog = malloc(loglen + 1);
        infolog[loglen] = '\0';
        glGetShaderInfoLog(vert, loglen, &loglen, infolog);
        printf("%s\n", infolog);
        free(infolog);
        QUIT = TRUE;
        //clear up shader
        glDeleteShader(vert);
    }

    if(fragsuccess == FALSE){
        glGetShaderiv(frag, GL_INFO_LOG_LENGTH, &loglen);
        char *infolog = malloc(loglen + 1);
        infolog[loglen] = '\0';
        glGetShaderInfoLog(frag, loglen, &loglen, infolog);
        printf("%s\n", infolog);
        free(infolog);
        QUIT = TRUE;
        //clean up shaders
        glDeleteShader(vert);
        glDeleteShader(frag);
    }

    //create program
    prog = glCreateProgram();
    //attach shaders
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    //link program
    glLinkProgram(prog);
    //print link info
    int linked = FALSE;
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    if(linked == FALSE){
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &loglen);
        char *infolog = malloc(loglen + 1);
        infolog[loglen] = '\0';
        glGetProgramInfoLog(prog, loglen, &loglen, infolog);
        printf("%s\n", infolog);
        free(infolog);
        QUIT = TRUE;
        //clean up shaders and program
        glDeleteShader(vert);
        glDeleteShader(frag);
        glDeleteProgram(prog);
    }

    //detatch post link to avoid memleaks
    glDetachShader(prog, vert);
    glDetachShader(prog, frag);

    unsigned int 
    Vec3 planet_model = load_obj("Model/sphere.obj", )

    

    while(!QUIT){
        glClear(GL_COLOR_BUFFER_BIT);



        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glfwSetWindowShouldClose(window, TRUE);
    
    //QUIT = TRUE;
    pthread_join(server_connection, NULL);
    
    glfwTerminate();
    free(vertsrc);
    free(fragscr);
    return 0;
}
