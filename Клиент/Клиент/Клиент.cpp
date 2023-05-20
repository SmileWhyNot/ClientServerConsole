#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable: 4996)

int main() {
    // тут как и в старых лабах
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(12345);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) { // проверяет, была ли успешно создан сокет клиента. 
        std::cerr << "Failed to create client socket.\n";
        WSACleanup();
        return 1;
    }
    // соединяют клиентский сокет с серверным сокетом, используя заданный адрес и порт сервера.
    if (connect(clientSocket, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server.\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Проверка выбора Выхода из программы
    bool flag = false;
    // Здесь начинается работа клиента
    while (!flag) {
        // Отображаем меню
        std::cout << "Select an action:" << std::endl;
        std::cout << "1. Add new vector" << std::endl;
        std::cout << "2. Change vector's element" << std::endl;
        std::cout << "3. Delete vector's element" << std::endl;
        std::cout << "4. Sort vector's elements" << std::endl;
        std::cout << "5. Add/Subtract 2 vectors with same indexes" << std::endl;
        std::cout << "6. Multiply/Divide a vector by a scalar" << std::endl;
        std::cout << "7. Find Min/Max vector's element" << std::endl;
        std::cout << "8. Show vector" << std::endl;
        std::cout << "9. Exit" << std::endl;

        // Ждем ввода пользователя
        int choice;
        std::cin >> choice;
        
        // Выбираем действие на основе ввода пользователя
        switch (choice) {
        case 1: {
            std::string r1 = "1";
            send(clientSocket, r1.c_str(), r1.size(), 0); // отправляем выбранный пункт меню

            int count, element;
            std::cout << "Enter number of vector's elements: "; // нужно понять сколько в новом векторе будет чисел
            std::cin >> count;
            std::string request;
            for (int i = 0; i < count; i++)
            {
                std::cout << "Enter vector's element: "; // вводим числа и разделяем с помощью #
                std::cin >> element;
                request += std::to_string(element) + "#";
            }


            // Отправляем запрос на сервер
            if (send(clientSocket, request.c_str(), request.size(), 0) < 0) {
                std::cerr << "Error: failed to send data" << std::endl;
                return 1;
            }

            // Ждем ответа от сервера
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0) {
                std::cerr << "Error: failed to receive data" << std::endl;
                return 1;
            }

            // Проверяем, что сервер ответил "OK"
            if (strncmp(buffer, "OK", 2) != 0) {
                std::cerr << "Error: server returned an error" << std::endl;
                return 1;
            }
            else {
                std::cout << "New vector added successfully" << std::endl;
            }
            break;
        }
        case 2: {
            std::string r2 = "2";
            send(clientSocket, r2.c_str(), r2.size(), 0);
            
            int number, index, value;
            std::cout << "Enter vector's number: "; // номер вектора в коллекции
            std::cin >> number;
            std::cout << "Enter vector's index to change: "; // индекс элемента в векторе
            std::cin >> index;
            std::cout << "Enter value: "; // значение
            std::cin >> value;

            // Формируем запрос к серверу
            std::string request = std::to_string(number) + "#" + std::to_string(index) + "#" + std::to_string(value) + "#";

            // Отправляем запрос на сервер
            if (send(clientSocket, request.c_str(), request.size(), 0) < 0) {
                std::cerr << "Error: failed to send data" << std::endl;
                return 1;
            }

            // Ждем ответа от сервера
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0) {
                std::cerr << "Error: failed to receive data" << std::endl;
                return 1;
            }

            // Проверяем, что сервер ответил "OK"
            if (strncmp(buffer, "OK", 2) != 0) {
                std::cerr << "Error: server returned an error" << std::endl;
                return 1;
            }
            else {
                std::cout << "Element changed successfully" << std::endl;
            }
            break;
        }
        case 3: {
            std::string r3 = "3";
            send(clientSocket, r3.c_str(), r3.size(), 0);
            
            int number, index;
            std::cout << "Enter vector's number: ";
            std::cin >> number;
            std::cout << "Enter vector's element index: ";
            std::cin >> index;

            // Формируем запрос к серверу
            std::string request = std::to_string(number) + "#" + std::to_string(index) + "#";

            // Отправляем запрос на сервер
            if (send(clientSocket, request.c_str(), request.size(), 0) < 0) {
                std::cerr << "Error: failed to send data" << std::endl;
                return 1;
            }

            // Ждем ответа от сервера
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0) {
                std::cerr << "Error: failed to receive data" << std::endl;
                return 1;
            }

            // Проверяем, что сервер ответил "OK"
            if (strncmp(buffer, "OK", 2) != 0) {
                std::cerr << "Error: server returned an error" << std::endl;
                return 1;
            }
            else {
                std::cout << "Element deleted" << std::endl;
            }
            break;
        }
        case 4: {
            std::string r4 = "4";
            send(clientSocket, r4.c_str(), r4.size(), 0);
            
            int number;
            std::cout << "Enter vector's number: ";
            std::cin >> number;

            // Формируем запрос к серверу
            std::string request = std::to_string(number) + "#";

            // Отправляем запрос на сервер
            if (send(clientSocket, request.c_str(), request.size(), 0) < 0) {
                std::cerr << "Error: failed to send data" << std::endl;
                return 1;
            }

            // Ждем ответа от сервера
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0) {
                std::cerr << "Error: failed to receive data" << std::endl;
                return 1;
            }

            // Проверяем, что сервер ответил "OK"
            if (strncmp(buffer, "OK", 2) != 0) {
                std::cerr << "Error: server returned an error" << std::endl;
                return 1;
            }
            else {
                std::cout << "Sorted successfully" << std::endl;
            }
            break;
        }
        case 5: {
            std::string r5 = "5";
            send(clientSocket, r5.c_str(), r5.size(), 0);
            
            int  vector1, vector2;
            char operation;
            std::cout << "Enter operation(+ or -): ";
            std::cin >> operation;
            std::cout << "Enter vector's-1 number: ";
            std::cin >> vector1;
            std::cout << "Enter vector's-2 number: ";
            std::cin >> vector2;

            // Формируем запрос к серверу
            std::string request = std::to_string(operation) + "#" + std::to_string(vector1) + "#" + std::to_string(vector2) + "#";

            // Отправляем запрос на сервер
            if (send(clientSocket, request.c_str(), request.size(), 0) < 0) {
                std::cerr << "Error: failed to send data" << std::endl;
                return 1;
            }

            char response[1024];
            memset(response, 0, sizeof(response));
            int response_size = recv(clientSocket, response, sizeof(response), 0);
            response[response_size] = '\0';

            std::cout << response << std::endl;
            break;
        }
        case 6: {
            std::string r6 = "6";
            send(clientSocket, r6.c_str(), r6.size(), 0);
            
            int vector1, scalar;
            char operation;
            std::cout << "Enter operation(* or /): ";
            std::cin >> operation;
            std::cout << "Enter vector's-1 number: ";
            std::cin >> vector1;
            std::cout << "Enter scalar: ";
            std::cin >> scalar;

            // Формируем запрос к серверу
            std::string request = std::to_string(operation) + "#" + std::to_string(vector1) + "#" + std::to_string(scalar) + "#";

            // Отправляем запрос на сервер
            if (send(clientSocket, request.c_str(), request.size(), 0) < 0) {
                std::cerr << "Error: failed to send data" << std::endl;
                return 1;
            }

            char response[1024];
            memset(response, 0, sizeof(response));
            int response_size = recv(clientSocket, response, sizeof(response), 0);
            response[response_size] = '\0';

            std::cout << response << std::endl;
            break;
        }
        case 7: {
            std::string r7 = "7";
            send(clientSocket, r7.c_str(), r7.size(), 0);
            
            int vector1;
            char operation; // ввести +, чтобы найти максимум, ввести -, чтобы найти минимум
            std::cout << "Enter operation +(find max) or -(find min): ";
            std::cin >> operation;
            std::cout << "Enter vector's-1 number: "; // в каком векторе искать максимум или минимум
            std::cin >> vector1;

            // Формируем запрос к серверу
            std::string request = std::to_string(operation) + "#" + std::to_string(vector1) + "#";

            // Отправляем запрос на сервер
            if (send(clientSocket, request.c_str(), request.size(), 0) < 0) {
                std::cerr << "Error: failed to send data" << std::endl;
                return 1;
            }

            // Ждем ответа от сервера
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0) {
                std::cerr << "Error: failed to receive data" << std::endl;
                return 1;
            }

            std::cout << buffer << std::endl; // выводим найденный элемент
            break;
        }
        case 8: {
            std::string r8 = "8";
            send(clientSocket, r8.c_str(), r8.size(), 0);
            // Запрашиваем новые размеры матрицы
            int vector1;
            std::cout << "Enter vector's-1 number: ";
            std::cin >> vector1;

            // Формируем запрос к серверу
            std::string request = std::to_string(vector1) + "#";

            // Отправляем запрос на сервер
            if (send(clientSocket, request.c_str(), request.size(), 0) < 0) {
                std::cerr << "Error: failed to send data" << std::endl;
                return 1;
            }

            char response[1024];
            memset(response, 0, sizeof(response));
            int response_size = recv(clientSocket, response, sizeof(response), 0);
            response[response_size] = '\0';

            std::cout << response << std::endl;
            break;
        }
        case 9: {
            std::string r8 = "9";
            send(clientSocket, r8.c_str(), r8.size(), 0);
            closesocket(clientSocket);
            flag = true;
            break;
        }
        default: {
            std::cout << "Invalid choice.\n";
            closesocket(clientSocket);
            break;
        }
        }
    }
}