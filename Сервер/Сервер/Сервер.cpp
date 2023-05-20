#include <iostream>
#include <vector>
#include <functional>
#include <winsock2.h>
#include <string>
#include <cstring>
#include <thread>
#include <cstring>
#include <algorithm>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib") // основная библиотека
#pragma warning(disable: 4996) // отключаем ненужные ошибки (устаревшая версия и тп)

using namespace std;


// Структура для хранения вектора
struct Vector {
    vector<int> data;

    // добавляем в конец
    void addElement(int value) {
        data.push_back(value);
    }
    // сортируем по возрастанию
    void sort() {
        std::sort(data.begin(), data.end());
    }
    // получаем элемент вектора по индексу
    int getElement(int index) {
        if (index >= data.size()) {
            return -1; // индекс находится за пределами вектора
        }
        return data[index];
    }
    // устанавливаем элемент
    void setElement(int index, int value) {
        if (index >= data.size()) {
            return; // индекс находится за пределами вектора
        }
        data[index] = value;
    }

    int getSize() {
        return data.size();
    }
    // удаляем элемент
    void deleteElement(int index) {
        if (index >= data.size()) {
            return; // индекс находится за пределами вектора
        }
        data.erase(data.begin() + index);
    }
    // возвращаем весь вектор в виде массива
    int* getArray() {
        // создаем новый массив в куче
        int* arr = new int[data.size()];
        // копируем элементы вектора в созданный массив
        for (int i = 0; i < data.size(); i++) {
            arr[i] = data[i];
        }
        return arr;
    }
};


// Структура для хранения нескольких векторов
struct VectorCollection {
    // Грубо говоря массив векторов, тк их может быть много
    vector<Vector> vectors;

    // метод для добавления нового вектора в коллекцию
    void addVector(Vector vector) {
        vectors.push_back(vector);
    }

    // метод для поиска вектора по индексу в коллекции
    Vector& getVector(int index) {
        return vectors[index];
    }

    // метод для получения количества векторов в коллекции
    int getSize() {
        return vectors.size();
    }

    // метод для поэлементного сложения векторов с одинаковыми границами индексов
    Vector addVectors(int index1, int index2) {
        Vector result;
        if (index1 >= vectors.size() || index2 >= vectors.size() ||
            vectors[index1].getSize() != vectors[index2].getSize()) {
            return result; // векторы с такими индексами отсутствуют или их размеры различны
        }
        for (int i = 0; i < vectors[index1].getSize(); i++) {
            result.addElement(vectors[index1].getElement(i) + vectors[index2].getElement(i));
        }
        return result;
    }

    // метод для поэлементного вычитания векторов с одинаковыми границами индексов
    Vector subtractVectors(int index1, int index2) {
        Vector result;
        if (index1 >= vectors.size() || index2 >= vectors.size() ||
            vectors[index1].getSize() != vectors[index2].getSize()) {
            return result; // векторы с такими индексами отсутствуют или их размеры различны
        }
        for (int i = 0; i < vectors[index1].getSize(); i++) {
            result.addElement(vectors[index1].getElement(i) - vectors[index2].getElement(i));
        }
        return result;
    }

    // метод для умножения всех элементов вектора на скаляр
    Vector multiplyVectorByScalar(int index, int scalar) {
        Vector result;
        if (index >= vectors.size()) {
            return result; // вектор с таким индексом отсутствует
        }
        for (int i = 0; i < vectors[index].getSize(); i++) {
            result.addElement(vectors[index].getElement(i) * scalar);
        }
        return result;
    }

    // метод для деления всех элементов вектора на скаляр
    Vector divideVectorByScalar(int index, int scalar) {
        Vector result;
        if (index >= vectors.size()) {
            return result; // вектор с таким индексом отсутствует
        }
        for (int i = 0; i < vectors[index].getSize(); i++) {
            result.addElement(vectors[index].getElement(i) / scalar);
        }
        return result;
    }

    // метод для поиска максимального элемента вектора
    int getMaxElement(int index) {
        if (index >= vectors.size()) {
            return -1; // вектор с таким индексом отсутствует
        }
        int maxElement = INT_MIN; // устанавливаем самое минимальное
        Vector vector = vectors[index]; // выбираем конкретный вектор по номеру
        for (int i = 0; i < vector.getSize(); i++) {
            int currentElement = vector.getElement(i);
            if (currentElement > maxElement) { // сравниваем и ищем наименьший
                maxElement = currentElement;
            }
        }
        if (maxElement == INT_MIN) {
            return -1; // вектор пустой
        }
        return maxElement;
    }

    // метод для поиска минимального элемента вектора // аналогично поиску максимального
    int getMinElement(int index) {
        if (index >= vectors.size()) {
            return -1; // вектор с таким индексом отсутствует
        }
        int minElement = INT_MAX;
        Vector vector = vectors[index];
        for (int i = 0; i < vector.getSize(); i++) {
            int currentElement = vector.getElement(i);
            if (currentElement < minElement) {
                minElement = currentElement;
            }
        }
        if (minElement == INT_MAX) {
            return -1; // вектор пустой
        }
        return minElement;
    }
};


// Функция для обработки запроса от клиента
void processRequest(SOCKET clientSocket, VectorCollection& vCollection) {
    while (true) {
        char buffer[1024];
        recv(clientSocket, buffer, sizeof(buffer), 0); // recv вернет количество прочитанных байт


        // Разбор запроса от клиента
        int requestType = buffer[0] - '0'; // Например, если первый символ в буфере - '3', то buffer[0] - '0' вернет целочисленное значение 3.
        switch (requestType) {

        case 1: { // Запрос на формирование нового вектора
            char buf[100];
            int size = recv(clientSocket, buf, sizeof(buf), 0);
            Vector v; // Новый вектор
            std::string number;
            for (int i = 0; i < size; i++)
            {
                if (buf[i] == '#') {
                    int element = std::stoi(number);
                    v.addElement(element); // если встретили # => добавляем число в вектор
                    number = "";
                }
                else {
                    number += buf[i]; // пока не встретили разделитель, получаем число 
                }
            }
            if (!number.empty()) {
                int element = std::stoi(number);
                v.addElement(element);
            }
            vCollection.addVector(v); // добавляем вектор в коллекцию векторов 
            send(clientSocket, "OK", 2, 0); // отправляем положительный результат клиенту
            break;

        }
        case 2: { // Запрос на изменение вектора
            char buf[100];
            recv(clientSocket, buf, sizeof(buf), 0);
          

            int vectorNumber = 0; // номер вектора
            int j = 0;
            while (buf[j] != '#') { // пока не встретили разделитель, получаем число
                vectorNumber = vectorNumber * 10 + buf[j] - '0';
                j++;
            }

            int IndexToChange = 0; // индекс элемента вектора, который меняем
            int i = j+1;
            while (buf[i] != '#') {
                IndexToChange = IndexToChange * 10 + buf[i] - '0';
                i++;
            }

            int Value = 0; // значение, которое нужно вставить вместо старого
            int z = i + 1;
            while (buf[z] != '#') {
                Value = Value * 10 + buf[z] - '0';
                z++;
            }
            // устанавливаем новое значение в нужное место
            vCollection.getVector(vectorNumber).setElement(IndexToChange, Value);
            send(clientSocket, "OK", 2, 0); // отправляем подтверждение
            break;
        }
        case 3: { // Запрос на удаление элемента вектора !!! Индексы в векторе изменятся
            // преобразуем входящие параметры, для изменения
            char buf[100];
            recv(clientSocket, buf, sizeof(buf), 0);

            int vectorNumber = 0; // номер вектора
            int j = 0;
            while (buf[j] != '#') {
                vectorNumber = vectorNumber * 10 + buf[j] - '0';
                j++;
            }

            int IndexToDelete = 0; // индекс элемента вектора, который удаляем
            int i = j + 1;
            while (buf[i] != '#') {
                IndexToDelete = IndexToDelete * 10 + buf[i] - '0';
                i++;
            }
            // удаляем элемент
            vCollection.getVector(vectorNumber).deleteElement(IndexToDelete);
            send(clientSocket, "OK", 2, 0); // отправляем положительный результат клиенту
            break;
        }
        case 4: { // Запрос на сортировку элементов вектора
            char buf[100];
            recv(clientSocket, buf, sizeof(buf), 0);


            int vectorNumber = 0; // номер вектора
            int j = 0;
            while (buf[j] != '#') {
                vectorNumber = vectorNumber * 10 + buf[j] - '0';
                j++;
            }
            // сортируем в нужном векторе элементы
            vCollection.getVector(vectorNumber).sort();
            send(clientSocket, "OK", 2, 0); // отправляем положительный результат клиенту
            break;

        }
        case 5: { // Запрос на сложение/вычитание векторов
            char buf[100];
            recv(clientSocket, buf, sizeof(buf), 0);

            int operation_code = 0; // ищем максимальный/минимальный элемент вектора
            int j = 0;
            while (buf[j] != '#') { // вместо + или - нам приходит код символа
                operation_code = operation_code * 10 + buf[j] - '0';
                j++;
            }
            char operation = static_cast<char>(operation_code); // преобразуем код символа в символ

            int numVector1 = 0; // номер 1го вектора
            int i = j + 1;
            while (buf[i] != '#') {
                numVector1 = numVector1 * 10 + buf[i] - '0';
                i++;
            }

            int numVector2 = 0; // номер 2го вектора
            int z = i + 1;
            while (buf[z] != '#') {
                numVector1 = numVector1 * 10 + buf[z] - '0';
                z++;
            }
            Vector result; // если + складываем вектора, если - отнимаем
            if (operation == '+') {
                result = vCollection.addVectors(numVector1, numVector2);
            }
            else if (operation == '-') {
                result = vCollection.subtractVectors(numVector1, numVector2);
            }
            
            if (result.getSize() > 0) { // если два вектора с одинаковым количеством элементов
                int* arr = result.getArray(); // получаем указатель на массив целых чисел

                int arrSize = result.getSize(); // получаем размер массива в байтах

                // записываем в строку наш массив (результат)
                std::stringstream ss;
                for (int i = 0; i < arrSize; i++) {
                    ss << arr[i] << " ";
                }

                std::string resultString = ss.str();

                send(clientSocket, resultString.c_str(), resultString.size(), 0); // отправляем результат клиенту
                break;
            }
            else { // если разное количество элементов, например {1,2,3} + {1,2} => идем сюда
                // и возвращаем Different indexes
                std::string resultString = "Different indexes";
                send(clientSocket, resultString.c_str(), resultString.size(), 0); // отправляем результат клиенту
                break;
            }

            
        }
        case 6: { // запрос на умножение/деление вектора на скаляр
            char buf[100];
            recv(clientSocket, buf, sizeof(buf), 0);

            int operation_code = 0; // ищем максимальный/минимальный элемент вектора
            int j = 0;
            while (buf[j] != '#') {
                operation_code = operation_code * 10 + buf[j] - '0';
                j++;
            }
            char operation = static_cast<char>(operation_code);

            int numVector1 = 0; // номер 1го вектора
            int i = j + 1;
            while (buf[i] != '#') {
                numVector1 = numVector1 * 10 + buf[i] - '0';
                i++;
            }

            int Scalar = 0; // скаляр
            int z = i + 1;
            while (buf[z] != '#') {
                Scalar = Scalar * 10 + buf[z] - '0';
                z++;
            }

            Vector result;
            if (operation == '*') {
                result = vCollection.multiplyVectorByScalar(numVector1,Scalar);
            }
            else if (operation == '/') {
                result = vCollection.divideVectorByScalar(numVector1, Scalar);
            }


            int* arr = result.getArray(); // получаем указатель на массив целых чисел

            int arrSize = result.getSize(); // получаем размер массива в байтах
            // записываем в строку наш массив (результат)
            std::stringstream ss;
            for (int i = 0; i < arrSize; i++) {
                ss << arr[i] << " ";
            }

            std::string resultString = ss.str();

            send(clientSocket, resultString.c_str(), resultString.size(), 0); // отправляем результат клиенту
            break;
        }
        case 7: { // найти максимальный/минимальный элемент вектора
            char buf[100];
            recv(clientSocket, buf, sizeof(buf), 0);

            int operation_code = 0; // ищем максимальный/минимальный элемент вектора
            int j = 0;
            while (buf[j] != '#') {
                operation_code = operation_code * 10 + buf[j] - '0';
                j++;
            }
            char operation = static_cast<char>(operation_code);

            int numVector1 = 0; // номер 1го вектора
            int i = j + 1;
            while (buf[i] != '#') {
                numVector1 = numVector1 * 10 + buf[i] - '0';
                i++;
            }

            int result = 0;
            if (operation_code == '+') {
                result = vCollection.getMaxElement(numVector1);
            }
            else if (operation_code == '-') {
                result = vCollection.getMinElement(numVector1);
            }

            char resultStr[20]; // выделяем буфер для хранения строки
            sprintf(resultStr, "%d", result); // записываем значение в буфер

            send(clientSocket, resultStr , sizeof(resultStr), 0); // отправляем результат клиенту
            break;
        }
        case 8: {
            char buf[100];
            recv(clientSocket, buf, sizeof(buf), 0);

            int numVector1 = 0; // номер 1го вектора
            int i = 0;
            while (buf[i] != '#') {
                numVector1 = numVector1 * 10 + buf[i] - '0';
                i++;
            }

            int* arr = vCollection.getVector(numVector1).getArray(); // получаем указатель на массив целых чисел

            int arrSize = vCollection.getVector(numVector1).getSize(); // получаем размер массива в байтах
            // записываем в строку наш массив (результат)
            std::stringstream ss;
            for (int i = 0; i < arrSize; i++) {
                ss << arr[i] << " ";
            }

            std::string result = ss.str();

            send(clientSocket, result.c_str(), result.size(), 0); // отправляем результат клиенту
            break;
        }
        case 9: {
            closesocket(clientSocket); // закрываем сокет
            break;
        }
        default: {
            break;
        }
        }
    }
    
}

int main() {
    // создаем 4 вектора изначальных
    VectorCollection vCollection;
    Vector v1, v2, v3, v4;
    // 1
    v1.addElement(3);
    v1.addElement(1);
    v1.addElement(4);
    // 2
    v2.addElement(2);
    v2.addElement(5);
    v2.addElement(6);
    // 3
    v3.addElement(1);
    v3.addElement(2);
    // 4
    v4.addElement(3);
    v4.addElement(4);
    // добавляем вектора в коллекцию векторов
    vCollection.addVector(v1);
    vCollection.addVector(v2);
    vCollection.addVector(v3);
    vCollection.addVector(v4);

    // Инициализация Winsock
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(wVersionRequested, &wsaData) != 0) {
        cerr << "Failed to initialize Winsock.\n";
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET; // используем TCP
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // ip адрес локального хоста
    serverAddress.sin_port = htons(12345); // наш порт

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0); // создаем дискриптор сокета

    // проверка на подключение сокета
    if (::bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to bind server socket.\n";
        return 1;
    }
    // слушаем запросы от клиента
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Failed to listen on server socket.\n";
        closesocket(serverSocket);
        return 1;
    }

    cout << "Server is listening on port " << 12345 << endl;

    while (true) {
        // получаем запрос
        int clientSocket = accept(serverSocket, NULL, NULL); // слушаем новые подключения
        if (clientSocket == -1) {
            std::cerr << "Failed to accept client connection.\n";
            continue;
        }
        // новый поток
        std::thread t(processRequest, clientSocket, std::ref(vCollection)); // создается новый поток для каждого клиента и вызывается clientThread
        t.detach(); // используется для отсоединения потока от главного процесса, 
        // чтобы главный процесс мог продолжать прослушивать новые подключения.
    }

    // Закрываем серверный сокет
    closesocket(serverSocket);

    return 0;
}