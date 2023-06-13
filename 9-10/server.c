#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define TOTAL_OBSERVERS 1
#define TOTAL_CLIENTS 2
#define ISLAND_SIZE 10
#define SWAPS 1000

int next_section_index = 0;
int sections[ISLAND_SIZE];
struct sockaddr_in clients[TOTAL_CLIENTS];
socklen_t client_sizes[TOTAL_CLIENTS];
struct sockaddr_in observers[TOTAL_OBSERVERS];
socklen_t observer_sizes[TOTAL_OBSERVERS];
int server_socket;
char observer_message[256];

void send_message_to_observers(char* message) {
    for (int i = 0; i < TOTAL_OBSERVERS; i++) {
        if (sendto(server_socket, message, strlen(message), 0, (struct sockaddr*)&observers[i], observer_sizes[i]) == -1) {
            perror("Failed to send message to observer");
            exit(1);
        }
    }
    sleep(3);
}

void check_observers() {
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(server_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    int active = 0;

    while (active < TOTAL_OBSERVERS) {
        char* message = "check";
        for (int i = 0; i < TOTAL_OBSERVERS; i++) {
            if (sendto(server_socket, message, strlen(message), 0, (struct sockaddr*)&observers[i], observer_sizes[i]) == -1) {
                perror("Failed to send message to observer");
                exit(1);
            }
        }

        for (int i = 0; i < TOTAL_OBSERVERS; i++) {
            char message[256];
            struct sockaddr_in client_addr;
            socklen_t addr_size = sizeof(client_addr);

            if (recvfrom(server_socket, message, sizeof(message), 0, (struct sockaddr*) &client_addr, &addr_size) >= 0) {
                active++;
            }
            else {
                continue;
            }

            observers[i] = client_addr;
            observer_sizes[i] = addr_size;
        }

        if (active != TOTAL_OBSERVERS) {
            printf("Waiting for all observers to come back online!\n");
        }
    }

    tv.tv_sec = 0;
    tv.tv_usec = 0;
    setsockopt(server_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

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
        sprintf(observer_message, "Client %d is searching section %d\n", i + 1, sections[next_section_index - 1]);
        send_message_to_observers(observer_message);
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
        sprintf(observer_message, "Client %d is stopping it's work\n", i + 1);
        send_message_to_observers(observer_message);
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
    int num_clients = 0;
    int num_observers = 0;

    while ((num_clients < TOTAL_CLIENTS) || (num_observers < TOTAL_OBSERVERS)) {
        char message[256];
        struct sockaddr_in client_addr;
        socklen_t addr_size = sizeof(client_addr);

        int bytes_read = recvfrom(server_socket, message, sizeof(message), 0, (struct sockaddr*)&client_addr, &addr_size);
        if (bytes_read == -1) {
            perror("Failed to receive client information");
            exit(1);
        }

        message[bytes_read] = '\0';

        if (strcmp(message, "client") == 0) {
            clients[num_clients] = client_addr;
            client_sizes[num_clients] = addr_size;

            printf("Client %d located\n", num_clients + 1);

            num_clients++;
        }
        
        if (strcmp(message, "observer") == 0) {
            observers[num_observers] = client_addr;
            observer_sizes[num_observers] = addr_size;

            printf("Observer %d located\n", num_observers + 1);

            num_observers++;
        }
    }

    int found_treasure = 0;
    do {
        // Проверка наблюдателей
        check_observers();

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
                        sprintf(observer_message, "Client %d found the treasure!\n", j + 1);
                        send_message_to_observers(observer_message);
                    }
                }
                found_treasure = 1;
            }
            else {
                for (int j = 0; j < TOTAL_CLIENTS; j++) {
                    if ((clients[j].sin_port == client_addr.sin_port) && (clients[j].sin_addr.s_addr == client_addr.sin_addr.s_addr)) {
                        sprintf(observer_message, "Client %d did not find the treasure\n", j + 1);
                        send_message_to_observers(observer_message);
                    }
                }
            }
        }
    } while (!found_treasure);

    // Отправка сообщения о завершении работы
    sleep(3);
    send_termination_message_to_clients();
    sprintf(observer_message, "Stopping work\n");
    send_message_to_observers(observer_message);

    // Завершение работы сервера
    printf("Stopping work\n");
    close(server_socket);

    return 0;
}
