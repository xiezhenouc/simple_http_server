/*
 * simple http server practice
 */
#include<netinet/in.h>    
#include<stdio.h>    
#include<stdlib.h>    
#include<sys/socket.h>    
#include<sys/stat.h>    
#include<sys/types.h>    
#include<unistd.h>    

#define SERVER_PORT 8899
#define BUFFSIZE 1024

void core_response(int new_socket)
{
     write(new_socket, "HTTP/1.1 200 OK\n", 16);
     write(new_socket, "Content-length: 46\n", 19);
     write(new_socket, "Content-Type: text/html\n\n", 25);
     write(new_socket, "<html><body><H1>Hello world</H1></body></html>",46);
}

void core_listen_loop(int create_socket, struct sockaddr_in address)
{
    int new_socket;
    int bufsize = BUFFSIZE;
    char *buffer = malloc(bufsize);
    socklen_t addrlen;

    while(1)
    {
        if(listen(create_socket, 10) < 0)
        {
            perror("Server listen error!\n");
            exit(1);
        }   
        if ((new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen)) < 0) 
        {    
            perror("Server accept error!\n");    
            exit(1);    
        }    
        else
        {
            printf("Client is connected ...\n");
        }
        
        recv(new_socket, buffer, bufsize, 0);
        printf("Request is following : \n %s \n", buffer);

        core_response(new_socket);

        close(new_socket);
    }   
}

int main()
{
    int create_socket;
    struct sockaddr_in address;

    if((create_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Created socket failed!\n");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT);

    if(bind(create_socket, (struct sockaddr *) &address, sizeof(address)) < 0)
    {
        perror("Bind failed!\n");
        exit(1);
    }
    
    core_listen_loop(create_socket, address);

    close(create_socket);
    return 0;
}
