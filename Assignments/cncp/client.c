#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void error_exit(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main() {
    // Create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        error_exit("Socket creation failed");
    }

    // Define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Use the server's IP address here

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        error_exit("Connection failed");
    }

    printf("Connected to Quiz Server.\n\n");

    char choice[2];
    char buff[1024];
        recv(client_socket,buff,sizeof(buff),0);
        printf("Starting Quizz...\n");
        
    for (int i = 1; i <= 10; i++) {
        char question[1024];
        char option1[1024];

        recv(client_socket, question, sizeof(question), 0);

        send(client_socket, "received", sizeof("received"), 0);

        printf("\n%s\n",question);

        for (int j = 0; j < 3; j++) {
            char option[150];
            recv(client_socket, option, sizeof(option), 0);
            printf("\n%s\n", option);
            send(client_socket,"receivedOpt",sizeof("receivedOpt"),0);
        }

        printf("Your choice (a/b/c): ");
        scanf("%s", choice);

        send(client_socket, choice, sizeof(choice), 0);

    }
    
    // Receive the integer from the server
    int score;
    if (recv(client_socket, &score, sizeof(score), 0) < 0) {
        error_exit("Receive integer failed");
    }
    
    printf("\nQuiz Over....\n");
    printf("\nYour Score: %d out of 10\n", score);
        
        char res;
        printf("Do you wnant to store your record ? (y/n)");
        scanf("%s",&res);
        if (res=='y'||res=='Y')
        {
             FILE *user_file = fopen("user.txt", "a");
            if (user_file) {
                char name[50], prn[20]; // Increase buffer size if needed
                printf("Enter your name: ");
                scanf("%s", name);
                printf("Enter your PRN: ");
                scanf("%s", prn);
                fprintf(user_file, "Name: %s\n", name);
                fprintf(user_file, "PRN: %s\n", prn);
                fprintf(user_file, "Score: %d\n", score);
                fclose(user_file);
            } else {
                printf("Unable to open user.txt\n");
            }

        }
    // Close the client socket
    close(client_socket);
   
    return 0;
}
