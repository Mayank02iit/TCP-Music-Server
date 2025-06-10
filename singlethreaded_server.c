#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define PORT 8800
#define BUFFER_SIZE 4084

void send_song(int client_socket,char song_file[]) {
    FILE *song_file = fopen(song_file, "rb");
    if (!song_file) {
        perror("Error opening song file");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), song_file)) > 0) {
        if (send(client_socket, buffer, bytes_read, 0) == -1) {
            perror("Error sending song data");
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_read == -1) {
        perror("Error reading song file");
        exit(EXIT_FAILURE);
    }

    fclose(song_file);
}


int main(int argc, char *argv[]){
    int client_socket,server_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_length;
    char buffer[BUFFER_SIZE];
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(atoi(argv[1]));

    // Bind socket to address and port
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) 
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_socket, 5) == -1) 
    {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for a connection...\n");

    // Accept incoming connection
    client_length = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_length);
    if (client_socket == -1) 
    {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

    printf("Connection accepted\n");


    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received == -1) 
    {
        perror("Error receiving data");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received]='\0';

char song_file_location[1024] ;
char *songnames[]= {"dontgiveup","one gift shawty","Bad intentions"};
snprintf(song_file_location, sizeof(song_file_location), "%s%s.mp3",argv[2],songnames[(buffer[0]-'0')-1]);
//argv[2]=mp3/    I saved the songs in the subdirectory of the root directory in a file mp3

printf("song name requested is %s and the ip-address of the client is %s\n",songnames[(buffer[0]-'0')-1] ,inet_ntoa(client_address.sin_addr));
send_song(client_socket,song_file_location);
close(client_socket);
close(server_socket);

}
server1.c
Displaying server1.c.
