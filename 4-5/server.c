#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

#define TOTAL_CLIENTS 3
#define ISLAND_SIZE 20
#define SWAPS 1000

int next_section_index = 0;
int sections[ISLAND_SIZE];
struct sockaddr_in clients[TOTAL_CLIENTS];
socklen_t client_sizes[TOTAL_CLIENTS];
int server_socket;

void initialize_sections() {
    for (int i = 0; i < ISLAND_SIZE; i++) {
        sections[i] = ISLAND_SIZE - i;
    }

    for (int i = 0; i < SWAPS; i++) {
        int j = rand() % ISLAND_SIZE;
        int k = rand() % ISLAND_SIZE;

        int temp = sections[j];
        sections[j] = sections[k];
        sections[k] = temp;
    }
}

int send_instructions_to_clients() {
    int instructions_sent = 0;

    for (int i = 0; i < TOTAL_CLIENTS; i++) {
        if (next_section_index == ISLAND_SIZE) {
            break;
        }

        char instructions[256];
        sprintf(instructions, "search section %d", sections[next_section_index]);
        next_section_index++;

        if (sendto(server_socket, instructions, strlen(instructions), 0, (struct sockaddr*)&clients[i], client_sizes[i]) == -1) {
            perror("Failed to send instructions");
            exit(1);
        }

        instructions_sent++;
        printf("Instructions \"%s\" sent to client %d\n", instructions, i + 1);
    }

    return instructions_sent;
}

void send_termination_message_to_clients() {
    for (int i = 0; i < TOTAL_CLIENTS; i++) {
        const char* message = "stop work";
        if (sendto(server_socket, message, strlen(message), 0, (struct sockaddr*)&clients[i], client_sizes[i]) == -1) {
            perror("Failed to send termination message");
            exit(1);
        }
        printf("Instructions \"%s\" sent to client %d\n", message, i + 1);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Server port not specified\n");
        exit(1);
    }

    int server_port = atoi(argv[1]);
    struct sockaddr_in server_addr;

    srand(time(NULL));
    initialize_sections();

    // Создание сокета
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("Failed to create socket");
        exit(1);
    }

    // Настройка адреса сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Привязка сокета к адресу сервера
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Failed to bind socket");
        exit(1);
    }

    printf("Waiting for clients...\n");

    // Получение адресов клиентов
    int client_ports[TOTAL_CLIENTS];
    for (int i = 0; i < TOTAL_CLIENTS; i++) {
        struct sockaddr_in client_addr;
        socklen_t addr_size = sizeof(client_addr);

        if (recvfrom(server_socket, NULL, 0, 0, (struct sockaddr*)&client_addr, &addr_size) == -1) {
            perror("Failed to receive client information");
            exit(1);
        }
        
        clients[i] = client_addr;
        client_sizes[i] = addr_size;

        printf("Client %d located\n", i + 1);
    }

    int found_treasure = 0;    
    do {
        // Отправка инструкций
        int instructions_sent = send_instructions_to_clients();

        for (int i = 0; i < instructions_sent; i++) {
            char message[256];
            struct sockaddr_in client_addr;
            socklen_t addr_size = sizeof(client_addr);
            int bytes_read = recvfrom(server_socket, message, sizeof(message), 0, (struct sockaddr*)&client_addr, &addr_size);
            
            if (bytes_read == -1) {
                perror("Failed to receive message");
                exit(1);
            }

            message[bytes_read] = '\0';
            if (strcmp(message, "treasure found") == 0) {
                // Клад был найден
                for (int j = 0; j < TOTAL_CLIENTS; j++) {
                    if ((clients[j].sin_port == client_addr.sin_port) && (clients[j].sin_addr.s_addr == client_addr.sin_addr.s_addr)) {
                        printf("Treasure found by client %d\n", j + 1);
                    }
                }
                found_treasure = 1;
            }
        }
    } while (!found_treasure);

    // Отправка сообщения о завершении работы
    send_termination_message_to_clients();

    // Завершение работы сервера
    printf("Stopping work\n");
    close(server_socket);

    return 0;
}
