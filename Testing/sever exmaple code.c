/*
 *  NetworkServer.c
 *  ProgrammingPortfolio Skeleton
 *
 */

/* You will need to include these header files to be able to implement the TCP/IP functions */
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

/* You will also need to add #include for your graph library header files */
#include "linked_list.h"
#include "graph.h"
#include "dijkstra.h"



#define BUFFERSIZE 512

void print_client(int stream, const char* text);

void ReadLine(int stream, char *buffer);

void Connection(int clientConnection, Graph *graph);

int main(int argc, const char * argv[])
{
	int serverSocket = -1;
	int clientConnection;
	int port = -1;
	int quit = 0;
	int alen;
	Graph *graph;
	struct sockaddr_in server;
	struct sockaddr_in client;
	

	printf("Programming Portfolio 2021 Implementation\n");
	printf("=========================================\n\n");
	
	/* Insert your code here to create, and the TCP/IP socket for the Network Server
	 *
	 * Then once a connection has been accepted implement protocol described in the coursework description.
	 */
	
	if ((port = atoi(argv[1])) < 1){
		fprintf(stderr, "error : invalid port\n");
		exit(1);
	}

	serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);


	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	if(bind(serverSocket, (struct sockaddr*)&server, sizeof(server)) < 0){
		fprintf(stderr, "error : failed to bind\n");
		exit(1);
	}

	if (listen(serverSocket, 15) < 0){
		fprintf(stderr, "error : failed to listen\n");
	}

	graph = init_graph();
	while (!quit){
		alen = sizeof(client);
		clientConnection = accept(serverSocket, (struct sockaddr*)&client, &alen);
		Connection(clientConnection, graph);
		close(clientConnection);
	}

	free_graph(graph);
	return 0;
}

void Connection(int clientConnection, Graph *graph ){
	Edge **edges;
	Path* table;
	int scrNet, destNet;
	float weight;
	int *vert, vertCount;
	int clientQuit = 0;
	int size, i;
	char buffer[512], *token;

	print_client(clientConnection, "+OK 2020 Programming Portfolio Route Server\r\n");
	clientQuit = 0;
	while (!clientQuit){
		ReadLine(clientConnection, buffer);
		token = strtok(buffer, " ");
		if (!strcmp(buffer, "QUIT")){
			print_client(clientConnection, "+OK\r\n");
			clientQuit = 1;
			break;
		} else if(!strcmp(token, "NET-ADD")){
			token = strtok(NULL, "\r\n");
			scrNet = atoi(token);
			if (!find_vertex(graph, scrNet)){
				add_vertex(graph, scrNet);
				print_client(clientConnection, "+OK\r\n");
			} else{
				print_client(clientConnection, "-ERR Network already exist\r\n");
			}
		} else if(!strcmp(token, "NET-DELETE")){
			token = strtok(NULL, "\r\n");
			scrNet = atoi(token);
			if (find_vertex(graph, scrNet)){
				remove_vertex(graph, scrNet);
				print_client(clientConnection, "+OK\r\n");
			}else{
				print_client(clientConnection, "-ERR Network not found\r\n");
			}
		} else if(!strcmp(token, "NET-LIST")){
			vert = get_vertices(graph, &vertCount);
			sprintf(buffer, "+OK %d\n", vertCount);
			write(clientConnection, buffer, sizeof(buffer));
			for (i = 0; i < vertCount; i++){
				sprintf(buffer, "%d\n", vertCount);
				write(clientConnection, buffer, sizeof(buffer));
			}
			print_client(clientConnection, "\r\n");
		} else if(!strcmp(token, "ROUTE-ADD")){
			scrNet = atoi(strtok(NULL, " "));
			destNet = atoi(strtok(NULL, " "));
			weight = atof(strtok(NULL, "\r\n"));
			if (!find_vertex(graph, scrNet)){
				print_client(clientConnection, "-ERR Source network not found\r\n");
			} else if (!find_vertex(graph, destNet)){
				print_client(clientConnection, "-ERR Destination network not found\r\n");
			} else{
				if (get_edge(graph, scrNet, destNet)){
					get_edge(graph, scrNet, destNet)->weight = weight;
					get_edge(graph, destNet, scrNet)->weight = weight;
				}else
					add_edge_undirected(graph, scrNet, destNet, weight);
				
					print_client(clientConnection, "+OK\r\n");
				}
		} else if(!strcmp(token, "ROUTE-DELETE")){
			scrNet = atoi(strtok(NULL, " "));
			destNet = atoi(strtok(NULL, "\r\n"));
			
			if(get_edge(graph, scrNet, destNet)){
				remove_edge(graph, scrNet, destNet);
				remove_edge(graph, destNet, scrNet);
				print_client(clientConnection, "+OK\r\n");
			} else{
				print_client(clientConnection, "-ERR Network(s) not found \r\n");
			}
		} else if(!strcmp(token, "ROUTE-SHOW")){
			scrNet = atoi(strtok(NULL, "\r\n"));
			if (find_vertex(graph, scrNet)){
				edges = get_edges(graph, find_vertex(graph, scrNet), &size);
				sprintf(buffer, "+OK %d\r\n", size);
				write(clientConnection, buffer,strlen(buffer) * sizeof(char));
				for (i = 0; i < size; i++){
					sprintf(buffer, "%d\r\n", edges[i]->vertex->id);
					write(clientConnection, buffer,strlen(buffer) * sizeof(char));
				}
			} else{
				print_client(clientConnection, "-ERR Network not found\r\n");
			}
		} else if(!strcmp(token, "ROUTE-HOP")){
			scrNet = atoi(strtok(NULL, " "));
			destNet = atoi(strtok(NULL, "\r\n"));
			if (find_vertex(graph, scrNet) && find_vertex(graph, destNet) && scrNet != destNet){
				table = dijkstra(graph, scrNet, &size);
				sprintf(buffer, "+OK %d\r\n", table[destNet].next_hop);
				write(clientConnection, buffer,strlen(buffer) * sizeof(char));
				free(table);
			} else {
				print_client(clientConnection, "-ERR Network(s) not found or identical\r\n");
			}
		} else if(!strcmp(token, "ROUTE-TABLE")){
			scrNet = atoi(strtok(NULL, "\r\n"));
			if (find_vertex(graph, scrNet)){
				table = dijkstra(graph, scrNet, &size);
				sprintf(buffer, "+OK %d\r\n", size -2);
				write(clientConnection, buffer,strlen(buffer) * sizeof(char));
				
				for (i = 1; i <= size -1; i++){
					if (i != scrNet){
						sprintf(buffer, "%d -> %d, next-hop %d, weight %f\r\n", scrNet, i, table[i].next_hop, table[i].weight);
						write(clientConnection, buffer,strlen(buffer) * sizeof(char));
					}
				}
				free(table);
			}else{
				print_client(clientConnection, "-ERR Network not found\r\n");
			}
		} else{
			print_client(clientConnection, "-ERR Not Implemented\r\n");
		}			
	}
}

void print_client(int client, const char* text){
	char buffer[BUFFERSIZE];
	sprintf(buffer, text);
	write(client, buffer,strlen(buffer) * sizeof(char));
}

void ReadLine(int stream, char *buffer){
	char line[BUFFERSIZE + 1];
	char *p = buffer;
	int size, i, lineEnd = 0;

	do {
		size = read(stream, line, BUFFERSIZE);

		for (i = 0; i < size; i++){
			*p = line[i];
			/*if end of line */
			if (p != buffer && *p == 10 && *(p-1) == 13){
				*(p-1) = 0;
				lineEnd = 1;
				break;
			}
			p++;
		}
		*p = 0;
	}while(lineEnd == 0 && size > 0);
}

/**
	Handle multiple socket connections with select and fd_set on Linux
*/
 
#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
 
#define TRUE   1
#define FALSE  0
#define PORT 8888

int main(int argc , char *argv[])
{
    int opt = TRUE;
    int master_socket , addrlen , new_socket , client_socket[30] , max_clients = 30 , activity, i , valread , sd;
	int max_sd;
    struct sockaddr_in address;
     
    char buffer[1025];  //data buffer of 1K
     
    //set of socket descriptors
    fd_set readfds;
     
    //a message
    char *message = "ECHO Daemon v1.0 \r\n";
 
    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++) 
    {
        client_socket[i] = 0;
    }
     
    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
 
    //set master socket to allow multiple connections , this is just a good habit, it will work without this
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
 
    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
     
    //bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
	printf("Listener on port %d \n", PORT);
	
    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
     
    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");
    
	while(TRUE) 
    {
        //clear the socket set
        FD_ZERO(&readfds);
 
        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;
		
        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++) 
        {
            //socket descriptor
			sd = client_socket[i];
            
			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &readfds);
            
            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
				max_sd = sd;
        }
 
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
   
        if ((activity < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }
         
        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(master_socket, &readfds)) 
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
			
            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
       
            //send new connection greeting message
            if( send(new_socket, message, strlen(message), 0) != strlen(message) ) 
            {
                perror("send");
            }
             
            puts("Welcome message sent successfully");
             
            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++) 
            {
                //if position is empty
				if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);
					
					break;
                }
            }
        }
         
        //else its some IO operation on some other socket :)
        for (i = 0; i < max_clients; i++) 
        {
            sd = client_socket[i];
             
            if (FD_ISSET( sd , &readfds)) 
            {
                //Check if it was for closing , and also read the incoming message
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                     
                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[i] = 0;
                }
                 
                //Echo back the message that came in
                else
                {
                    //set the string terminating NULL byte on the end of the data read
                    buffer[valread] = '\0';
                    send(sd , buffer , strlen(buffer) , 0 );
                }
            }
        }
    }
     
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>

#define SERVER_PORT  12345

#define TRUE             1
#define FALSE            0

main (int argc, char *argv[])
{
  int    len, rc, on = 1;
  int    listen_sd = -1, new_sd = -1;
  int    desc_ready, end_server = FALSE, compress_array = FALSE;
  int    close_conn;
  char   buffer[80];
  struct sockaddr_in6   addr;
  int    timeout;
  struct pollfd fds[200];
  int    nfds = 1, current_size = 0, i, j;

  /*************************************************************/
  /* Create an AF_INET6 stream socket to receive incoming      */
  /* connections on                                            */
  /*************************************************************/
  listen_sd = socket(AF_INET6, SOCK_STREAM, 0);
  if (listen_sd < 0)
  {
    perror("socket() failed");
    exit(-1);
  }

  /*************************************************************/
  /* Allow socket descriptor to be reuseable                   */
  /*************************************************************/
  rc = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR,
                  (char *)&on, sizeof(on));
  if (rc < 0)
  {
    perror("setsockopt() failed");
    close(listen_sd);
    exit(-1);
  }

  /*************************************************************/
  /* Set socket to be nonblocking. All of the sockets for      */
  /* the incoming connections will also be nonblocking since   */
  /* they will inherit that state from the listening socket.   */
  /*************************************************************/
  rc = ioctl(listen_sd, FIONBIO, (char *)&on);
  if (rc < 0)
  {
    perror("ioctl() failed");
    close(listen_sd);
    exit(-1);
  }

  /*************************************************************/
  /* Bind the socket                                           */
  /*************************************************************/
  memset(&addr, 0, sizeof(addr));
  addr.sin6_family      = AF_INET6;
  memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
  addr.sin6_port        = htons(SERVER_PORT);
  rc = bind(listen_sd,
            (struct sockaddr *)&addr, sizeof(addr));
  if (rc < 0)
  {
    perror("bind() failed");
    close(listen_sd);
    exit(-1);
  }

  /*************************************************************/
  /* Set the listen back log                                   */
  /*************************************************************/
  rc = listen(listen_sd, 32);
  if (rc < 0)
  {
    perror("listen() failed");
    close(listen_sd);
    exit(-1);
  }

  /*************************************************************/
  /* Initialize the pollfd structure                           */
  /*************************************************************/
  memset(fds, 0 , sizeof(fds));

  /*************************************************************/
  /* Set up the initial listening socket                        */
  /*************************************************************/
  fds[0].fd = listen_sd;
  fds[0].events = POLLIN;
  /*************************************************************/
  /* Initialize the timeout to 3 minutes. If no                */
  /* activity after 3 minutes this program will end.           */
  /* timeout value is based on milliseconds.                   */
  /*************************************************************/
  timeout = (3 * 60 * 1000);

  /*************************************************************/
  /* Loop waiting for incoming connects or for incoming data   */
  /* on any of the connected sockets.                          */
  /*************************************************************/
  do
  {
    /***********************************************************/
    /* Call poll() and wait 3 minutes for it to complete.      */
    /***********************************************************/
    printf("Waiting on poll()...\n");
    rc = poll(fds, nfds, timeout);

    /***********************************************************/
    /* Check to see if the poll call failed.                   */
    /***********************************************************/
    if (rc < 0)
    {
      perror("  poll() failed");
      break;
    }

    /***********************************************************/
    /* Check to see if the 3 minute time out expired.          */
    /***********************************************************/
    if (rc == 0)
    {
      printf("  poll() timed out.  End program.\n");
      break;
    }


    /***********************************************************/
    /* One or more descriptors are readable.  Need to          */
    /* determine which ones they are.                          */
    /***********************************************************/
    current_size = nfds;
    for (i = 0; i < current_size; i++)
    {
      /*********************************************************/
      /* Loop through to find the descriptors that returned    */
      /* POLLIN and determine whether it's the listening       */
      /* or the active connection.                             */
      /*********************************************************/
      if(fds[i].revents == 0)
        continue;

      /*********************************************************/
      /* If revents is not POLLIN, it's an unexpected result,  */
      /* log and end the server.                               */
      /*********************************************************/
      if(fds[i].revents != POLLIN)
      {
        printf("  Error! revents = %d\n", fds[i].revents);
        end_server = TRUE;
        break;

      }
      if (fds[i].fd == listen_sd)
      {
        /*******************************************************/
        /* Listening descriptor is readable.                   */
        /*******************************************************/
        printf("  Listening socket is readable\n");

        /*******************************************************/
        /* Accept all incoming connections that are            */
        /* queued up on the listening socket before we         */
        /* loop back and call poll again.                      */
        /*******************************************************/
        do
        {
          /*****************************************************/
          /* Accept each incoming connection. If               */
          /* accept fails with EWOULDBLOCK, then we            */
          /* have accepted all of them. Any other              */
          /* failure on accept will cause us to end the        */
          /* server.                                           */
          /*****************************************************/
          new_sd = accept(listen_sd, NULL, NULL);
          if (new_sd < 0)
          {
            if (errno != EWOULDBLOCK)
            {
              perror("  accept() failed");
              end_server = TRUE;
            }
            break;
          }

          /*****************************************************/
          /* Add the new incoming connection to the            */
          /* pollfd structure                                  */
          /*****************************************************/
          printf("  New incoming connection - %d\n", new_sd);
          fds[nfds].fd = new_sd;
          fds[nfds].events = POLLIN;
          nfds++;

          /*****************************************************/
          /* Loop back up and accept another incoming          */
          /* connection                                        */
          /*****************************************************/
        } while (new_sd != -1);
      }

      /*********************************************************/
      /* This is not the listening socket, therefore an        */
      /* existing connection must be readable                  */
      /*********************************************************/

      else
      {
        printf("  Descriptor %d is readable\n", fds[i].fd);
        close_conn = FALSE;
        /*******************************************************/
        /* Receive all incoming data on this socket            */
        /* before we loop back and call poll again.            */
        /*******************************************************/

        do
        {
          /*****************************************************/
          /* Receive data on this connection until the         */
          /* recv fails with EWOULDBLOCK. If any other         */
          /* failure occurs, we will close the                 */
          /* connection.                                       */
          /*****************************************************/
          rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
          if (rc < 0)
          {
            if (errno != EWOULDBLOCK)
            {
              perror("  recv() failed");
              close_conn = TRUE;
            }
            break;
          }

          /*****************************************************/
          /* Check to see if the connection has been           */
          /* closed by the client                              */
          /*****************************************************/
          if (rc == 0)
          {
            printf("  Connection closed\n");
            close_conn = TRUE;
            break;
          }

          /*****************************************************/
          /* Data was received                                 */
          /*****************************************************/
          len = rc;
          printf("  %d bytes received\n", len);

          /*****************************************************/
          /* Echo the data back to the client                  */
          /*****************************************************/
          rc = send(fds[i].fd, buffer, len, 0);
          if (rc < 0)
          {
            perror("  send() failed");
            close_conn = TRUE;
            break;
          }

        } while(TRUE);

        /*******************************************************/
        /* If the close_conn flag was turned on, we need       */
        /* to clean up this active connection. This            */
        /* clean up process includes removing the              */
        /* descriptor.                                         */
        /*******************************************************/
        if (close_conn)
        {
          close(fds[i].fd);
          fds[i].fd = -1;
          compress_array = TRUE;
        }


      }  /* End of existing connection is readable             */
    } /* End of loop through pollable descriptors              */

    /***********************************************************/
    /* If the compress_array flag was turned on, we need       */
    /* to squeeze together the array and decrement the number  */
    /* of file descriptors. We do not need to move back the    */
    /* events and revents fields because the events will always*/
    /* be POLLIN in this case, and revents is output.          */
    /***********************************************************/
    if (compress_array)
    {
      compress_array = FALSE;
      for (i = 0; i < nfds; i++)
      {
        if (fds[i].fd == -1)
        {
          for(j = i; j < nfds-1; j++)
          {
            fds[j].fd = fds[j+1].fd;
          }
          i--;
          nfds--;
        }
      }
    }

  } while (end_server == FALSE); /* End of serving running.    */

  /*************************************************************/
  /* Clean up all of the sockets that are open                 */
  /*************************************************************/
  for (i = 0; i < nfds; i++)
  {
    if(fds[i].fd >= 0)
      close(fds[i].fd);
  }
}



