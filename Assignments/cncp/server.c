#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void error_exit(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}
// Define options
char options[][3][150] = {
    {"a) Respiration", "b) Photosynthesis", "c) Digestion"},
    {"a) Carbon dioxide", "b) Oxygen", "c) Nitrogen"},
    {"a) 5", "b) 6", "c) 7"},
    {"a) 1942", "b) 1947", "c) 1950"},
    {"a) Mahatma Gandhi", "b) B.R. Ambedkar", "c) Jawaharlal Nehru"},
    {"a) Go", "b) Gd", "c) Au"},
    {"a) Venus", "b) Mercury", "c) Mars"},
    {"a) Mars", "b) Venus", "c) Jupiter"},
    {"a) Large Area Network", "b) Local Area Network", "c) Light Access Network"},
    {"a) Star topology", "b) Bus topology", "c) Ring topologyyst"},
};

char correct_options[10] = {'b', 'c', 'c', 'b', 'b', 'c', 'b', 'a', 'b', 'a'};

int main() {
    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Bind the socket to a port
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Binding failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("Listening failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Quiz Server is listening on port 8080...\n");
    
    FILE *questions_file = fopen("questions.txt", "r");
    if (!questions_file) {
        printf("Unable to open questions.txt\n");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Accept a client connection
        struct sockaddr_in client_address;
        socklen_t client_addr_size = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_addr_size);
        if (client_socket == -1) {
            perror("Acceptance failed");
            continue;
        }

        printf("Client connected.\n");

        // Signal the client to start
        char start_message[] = "start";
        send(client_socket, start_message, sizeof(start_message), 0);

        char user_options[11];

        for (int i = 0; i < 10; i++) {
            char question[1024];
            fgets(question, sizeof(question), questions_file);

            // Send the question and wait for confirmation
            send(client_socket, question, strlen(question), 0);

            char confirmation[20];
            recv(client_socket, confirmation, sizeof(confirmation), 0);
            if (strcmp(confirmation, "received") != 0) {
                perror("Confirmation not received");
                break;
            }

            for (int j = 0; j < 3; j++) {
                send(client_socket, options[i][j], sizeof(options[i][j]), 0);
                   
                    char confirmationOpt[12];
                    recv(client_socket, confirmation, sizeof(confirmation), 0);
                    if (strcmp(confirmation, "receivedOpt") != 0) {
                        perror("Confirmation of option received is not received");
                        break;
                }

            }

            char choice[2];
            recv(client_socket, choice, sizeof(choice), 0);
            user_options[i] = choice[0];
        }

        // Calculate the score
        int score = 0;
        for (int i = 0; i < 10; i++) {
            if (user_options[i] == correct_options[i]) {
                score++;
            }
        }

// Send the user's score
        send(client_socket, &score, sizeof(score), 0);

        // Close the client socket
        close(client_socket);
    }

    // Close the server socket (this part will not be reached in this example)
    close(server_socket);

    return 0;
}