#include <stdio.h>
#include <string.h>
#include <WinSock2.h>
#include <stdint.h>
#include "helper.h"
// set port number constant
#define PORT_NUMBER "14000"

int main(void)
{
    // winsock initialization
    WSADATA wsadata;
    int error = WSAStartup(MAKEWORD(2,2),&wsadata);
    if (error)
    {
        printf("WSA startup failed. error no: %d\n", error);
        return 1;
    }
    // socket initialization
    SOCKET sock;
    // creates socket, specifying IPV4 address family, TCP
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // error checking socket creation
    if (sock == INVALID_SOCKET)
    {
        printf("could not create socket.\n");
        return 1;
    }

    // retrieve local host name
    char hostname[100];
    int host_error = gethostname(hostname,sizeof(hostname));
    if (host_error != 0)
    {
        printf("could not retrieve host name. error no: %d\n", host_error);
        return 1;
    }
    // get ip address from host name
    struct hostent* host_info = gethostbyname(hostname);
    // long ipv4 located within hostent struct (fourth in list on my machine)
    long ip_address = (uintptr_t)host_info->h_addr_list[3];
    
    // converts port number to network byte order (big endian)
    u_short port_number = htons((uintptr_t)*PORT_NUMBER);

    // holds server's address + port number (specific to ipv4)
    struct sockaddr_in server_info, client_info;
    // filling in the server info
    server_info.sin_family = AF_INET;
    server_info.sin_port = port_number;
    server_info.sin_addr.s_addr = ip_address;

    // bind socket to server
    int bind_error = bind(sock, (const struct sockaddr*)&server_info, sizeof(server_info));
    if (bind_error != 0)
    {
        printf("could not bind socket. error no: %d\n", bind_error);
        return 1;
    }

    // wait for connections (second parameter is max connection queue length)
    int listen_error = listen(sock,SOMAXCONN);
    if (listen_error != 0)
    {
        printf("could not listen for connections. error no: %d\n", listen_error);
        return 1;
    }
    printf("waiting for connection...\n");

    // create new active socket for sending/recieving messages
    int client_size = (int)sizeof(client_info);
    SOCKET active_socket = accept(sock, (struct sockaddr*)&client_info, &client_size);
    if (active_socket == INVALID_SOCKET)
    {
        printf("active socket could not be created. error no: %d\n", WSAGetLastError());
        return 1;
    }
    printf("connected!\n");
   
    char user_message[30];
    do
    {
        // receive message from user 
        int receive_error = recv(active_socket, user_message, sizeof(user_message), 0);
        if (receive_error == -1)
        {
            printf("could not receive user message. error no: %d\n", WSAGetLastError());
            return 1;
        }
        // reverse and send string to client
        char reverse_message[30];
        strreverse(user_message, reverse_message);
        int send_error = send(active_socket, reverse_message,(int)strlen(reverse_message)+1,0);
        if (send_error == -1)
        {
            printf("could not send message to user. error no: %d\n", send_error);
            return 1;
        }
    } while (strcmp(user_message,"end"));
    
    printf("ending communication with client...\n");

    closesocket(active_socket);
    closesocket(sock);
    WSACleanup();
}