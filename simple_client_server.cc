#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

void handler(int clientSocket) {
    char buffer[BUFFER_SIZE] = {0};

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);

        if (recv(clientSocket, buffer, BUFFER_SIZE, 0) < 0) {
            return;
        }
        std::cout << "Получено сообщение от клиента: " << buffer << std::endl;

        const char *confirmation = "Сообщение успешно получено!";
        if (send(clientSocket, confirmation, strlen(confirmation), 0) < 0) {
            return;
        }
    }

    close(clientSocket);
}

void startServer() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        return;
    }

    while (true) {
        if (listen(serverSocket, 5) < 0) {
            return;
        }
        std::cout << "Сервер запущен и слушает порт: " << PORT << std::endl;

        while (true) {
            if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen)) < 0) {
                continue;
            }
            std::thread(handler, clientSocket).detach();
        }
    }

    close(serverSocket);
}

void startClient() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE] = {0};

    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        return;
    }

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        return;
    }

    while (true) {
        std::cout << "Введите сообщение: ";
        char message[BUFFER_SIZE];
        std::cin >> message;

        if (send(clientSocket, message, strlen(message), 0) < 0) {
            return;
        }

        if (recv(clientSocket, buffer, BUFFER_SIZE, 0) < 0) {
            return;
        }
        std::cout << "Ответ сервера: " << buffer << std::endl;
    }

    close(clientSocket);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <server|client>" << std::endl;
        return -1;
    }

    std::string mode = argv[1];
    if (mode == "server") {
        startServer();
    } else if (mode == "client") {
        startClient();
    } else {
        std::cerr << "Неизвестный аргумент!" << std::endl;
        return -1;
    }

    return 0;
}
