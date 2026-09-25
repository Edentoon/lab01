#include <stdio.h>
#include <string.h>
#include <WinSock2.h>

int main(int argc, char *argv[])
{
    // checks that correct arguments are provided
    if (argc != 3)
    {
        printf("arguments expected: <ip-address> <port-number>\n");
        return 1;
    }

    // winsock initialization
    WSADATA wsadata;
    int error = WSAStartup(MAKEWORD(2,2),&wsadata);
    if (error)
    {
        printf("WSA startup failed. error no: %d", error);
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

    // converts user inputted ip address from string to network endianness
    long ip_address = inet_addr(argv[1]);
    if (sizeof(ip_address) != 4)
    {
        printf("incorrect ip address/format. please try again\n");
        return 1;
    }
    // converts port number to network byte order (big endian)
    u_short port_number = htons((uintptr_t)*argv[2]);

    // holds server's address + port number (specific to ipv4)
    struct sockaddr_in server;
    // filling in the server info
    server.sin_family = AF_INET;
    server.sin_port = port_number;
    server.sin_addr.s_addr = ip_address;

    // connect to server
    int connect_error = connect(sock, (const struct sockaddr*)&server, sizeof(server));
    if (connect_error != 0)
    {
        printf("could not connect to server. error no: %d\n", WSAGetLastError());
        return 1;
    }
    printf("connected!\n");
    char user_message[30];
    char reverse_message[30];
    
    do
    {
        // get message from user
        printf("enter your message! (max 30 char)\n");
        scanf("%s", user_message);

        // send message to server (plus one to include null terminator)
        int send_error = send(sock, user_message, (int)strlen(user_message)+1, 0);
        if (send_error == -1)
        {
            printf("could not send message. error: %d", WSAGetLastError);
            return 1;
        }
        // recieve reversed string
        int receive_error = recv(sock, reverse_message, (int)sizeof(reverse_message), 0);
        if (receive_error == -1)
        {
            printf("could not recieve message. error no: %d", WSAGetLastError());
        }
        printf("server response: %s\n", reverse_message);
    } while (strcmp(user_message,"end"));
    
    printf("ending connection with server...\n");

    closesocket(sock);
    WSACleanup();
    


   

    



}