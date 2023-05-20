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

#pragma comment(lib, "Ws2_32.lib") // �������� ����������
#pragma warning(disable: 4996) // ��������� �������� ������ (���������� ������ � ��)

using namespace std;


// ��������� ��� �������� �������
struct Vector {
    vector<int> data;

    // ��������� � �����
    void addElement(int value) {
        data.push_back(value);
    }
    // ��������� �� �����������
    void sort() {
        std::sort(data.begin(), data.end());
    }
    // �������� ������� ������� �� �������
    int getElement(int index) {
        if (index >= data.size()) {
            return -1; // ������ ��������� �� ��������� �������
        }
        return data[index];
    }
    // ������������� �������
    void setElement(int index, int value) {
        if (index >= data.size()) {
            return; // ������ ��������� �� ��������� �������
        }
        data[index] = value;
    }

    int getSize() {
        return data.size();
    }
    // ������� �������
    void deleteElement(int index) {
        if (index >= data.size()) {
            return; // ������ ��������� �� ��������� �������
        }
        data.erase(data.begin() + index);
    }
    // ���������� ���� ������ � ���� �������
    int* getArray() {
        // ������� ����� ������ � ����
        int* arr = new int[data.size()];
        // �������� �������� ������� � ��������� ������
        for (int i = 0; i < data.size(); i++) {
            arr[i] = data[i];
        }
        return arr;
    }
};


// ��������� ��� �������� ���������� ��������
struct VectorCollection {
    // ����� ������ ������ ��������, �� �� ����� ���� �����
    vector<Vector> vectors;

    // ����� ��� ���������� ������ ������� � ���������
    void addVector(Vector vector) {
        vectors.push_back(vector);
    }

    // ����� ��� ������ ������� �� ������� � ���������
    Vector& getVector(int index) {
        return vectors[index];
    }

    // ����� ��� ��������� ���������� �������� � ���������
    int getSize() {
        return vectors.size();
    }

    // ����� ��� ������������� �������� �������� � ����������� ��������� ��������
    Vector addVectors(int index1, int index2) {
        Vector result;
        if (index1 >= vectors.size() || index2 >= vectors.size() ||
            vectors[index1].getSize() != vectors[index2].getSize()) {
            return result; // ������� � ������ ��������� ����������� ��� �� ������� ��������
        }
        for (int i = 0; i < vectors[index1].getSize(); i++) {
            result.addElement(vectors[index1].getElement(i) + vectors[index2].getElement(i));
        }
        return result;
    }

    // ����� ��� ������������� ��������� �������� � ����������� ��������� ��������
    Vector subtractVectors(int index1, int index2) {
        Vector result;
        if (index1 >= vectors.size() || index2 >= vectors.size() ||
            vectors[index1].getSize() != vectors[index2].getSize()) {
            return result; // ������� � ������ ��������� ����������� ��� �� ������� ��������
        }
        for (int i = 0; i < vectors[index1].getSize(); i++) {
            result.addElement(vectors[index1].getElement(i) - vectors[index2].getElement(i));
        }
        return result;
    }

    // ����� ��� ��������� ���� ��������� ������� �� ������
    Vector multiplyVectorByScalar(int index, int scalar) {
        Vector result;
        if (index >= vectors.size()) {
            return result; // ������ � ����� �������� �����������
        }
        for (int i = 0; i < vectors[index].getSize(); i++) {
            result.addElement(vectors[index].getElement(i) * scalar);
        }
        return result;
    }

    // ����� ��� ������� ���� ��������� ������� �� ������
    Vector divideVectorByScalar(int index, int scalar) {
        Vector result;
        if (index >= vectors.size()) {
            return result; // ������ � ����� �������� �����������
        }
        for (int i = 0; i < vectors[index].getSize(); i++) {
            result.addElement(vectors[index].getElement(i) / scalar);
        }
        return result;
    }

    // ����� ��� ������ ������������� �������� �������
    int getMaxElement(int index) {
        if (index >= vectors.size()) {
            return -1; // ������ � ����� �������� �����������
        }
        int maxElement = INT_MIN; // ������������� ����� �����������
        Vector vector = vectors[index]; // �������� ���������� ������ �� ������
        for (int i = 0; i < vector.getSize(); i++) {
            int currentElement = vector.getElement(i);
            if (currentElement > maxElement) { // ���������� � ���� ����������
                maxElement = currentElement;
            }
        }
        if (maxElement == INT_MIN) {
            return -1; // ������ ������
        }
        return maxElement;
    }

    // ����� ��� ������ ������������ �������� ������� // ���������� ������ �������������
    int getMinElement(int index) {
        if (index >= vectors.size()) {
            return -1; // ������ � ����� �������� �����������
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
            return -1; // ������ ������
        }
        return minElement;
    }
};


// ������� ��� ��������� ������� �� �������
void processRequest(SOCKET clientSocket, VectorCollection& vCollection) {
    while (true) {
        char buffer[1024];
        recv(clientSocket, buffer, sizeof(buffer), 0); // recv ������ ���������� ����������� ����


        // ������ ������� �� �������
        int requestType = buffer[0] - '0'; // ��������, ���� ������ ������ � ������ - '3', �� buffer[0] - '0' ������ ������������� �������� 3.
        switch (requestType) {

        case 1: { // ������ �� ������������ ������ �������
            char buf[100];
            int size = recv(clientSocket, buf, sizeof(buf), 0);
            Vector v; // ����� ������
            std::string number;
            for (int i = 0; i < size; i++)
            {
                if (buf[i] == '#') {
                    int element = std::stoi(number);
                    v.addElement(element); // ���� ��������� # => ��������� ����� � ������
                    number = "";
                }
                else {
                    number += buf[i]; // ���� �� ��������� �����������, �������� ����� 
                }
            }
            if (!number.empty()) {
                int element = std::stoi(number);
                v.addElement(element);
            }
            vCollection.addVector(v); // ��������� ������ � ��������� �������� 
            send(clientSocket, "OK", 2, 0); // ���������� ������������� ��������� �������
            break;

        }
        case 2: { // ������ �� ��������� �������
            char buf[100];
            recv(clientSocket, buf, sizeof(buf), 0);
          

            int vectorNumber = 0; // ����� �������
            int j = 0;
            while (buf[j] != '#') { // ���� �� ��������� �����������, �������� �����
                vectorNumber = vectorNumber * 10 + buf[j] - '0';
                j++;
            }

            int IndexToChange = 0; // ������ �������� �������, ������� ������
            int i = j+1;
            while (buf[i] != '#') {
                IndexToChange = IndexToChange * 10 + buf[i] - '0';
                i++;
            }

            int Value = 0; // ��������, ������� ����� �������� ������ �������
            int z = i + 1;
            while (buf[z] != '#') {
                Value = Value * 10 + buf[z] - '0';
                z++;
            }
            // ������������� ����� �������� � ������ �����
            vCollection.getVector(vectorNumber).setElement(IndexToChange, Value);
            send(clientSocket, "OK", 2, 0); // ���������� �������������
            break;
        }
        case 3: { // ������ �� �������� �������� ������� !!! ������� � ������� ���������
            // ����������� �������� ���������, ��� ���������
            char buf[100];
            recv(clientSocket, buf, sizeof(buf), 0);

            int vectorNumber = 0; // ����� �������
            int j = 0;
            while (buf[j] != '#') {
                vectorNumber = vectorNumber * 10 + buf[j] - '0';
                j++;
            }

            int IndexToDelete = 0; // ������ �������� �������, ������� �������
            int i = j + 1;
            while (buf[i] != '#') {
                IndexToDelete = IndexToDelete * 10 + buf[i] - '0';
                i++;
            }
            // ������� �������
            vCollection.getVector(vectorNumber).deleteElement(IndexToDelete);
            send(clientSocket, "OK", 2, 0); // ���������� ������������� ��������� �������
            break;
        }
        case 4: { // ������ �� ���������� ��������� �������
            char buf[100];
            recv(clientSocket, buf, sizeof(buf), 0);


            int vectorNumber = 0; // ����� �������
            int j = 0;
            while (buf[j] != '#') {
                vectorNumber = vectorNumber * 10 + buf[j] - '0';
                j++;
            }
            // ��������� � ������ ������� ��������
            vCollection.getVector(vectorNumber).sort();
            send(clientSocket, "OK", 2, 0); // ���������� ������������� ��������� �������
            break;

        }
        case 5: { // ������ �� ��������/��������� ��������
            char buf[100];
            recv(clientSocket, buf, sizeof(buf), 0);

            int operation_code = 0; // ���� ������������/����������� ������� �������
            int j = 0;
            while (buf[j] != '#') { // ������ + ��� - ��� �������� ��� �������
                operation_code = operation_code * 10 + buf[j] - '0';
                j++;
            }
            char operation = static_cast<char>(operation_code); // ����������� ��� ������� � ������

            int numVector1 = 0; // ����� 1�� �������
            int i = j + 1;
            while (buf[i] != '#') {
                numVector1 = numVector1 * 10 + buf[i] - '0';
                i++;
            }

            int numVector2 = 0; // ����� 2�� �������
            int z = i + 1;
            while (buf[z] != '#') {
                numVector1 = numVector1 * 10 + buf[z] - '0';
                z++;
            }
            Vector result; // ���� + ���������� �������, ���� - ��������
            if (operation == '+') {
                result = vCollection.addVectors(numVector1, numVector2);
            }
            else if (operation == '-') {
                result = vCollection.subtractVectors(numVector1, numVector2);
            }
            
            if (result.getSize() > 0) { // ���� ��� ������� � ���������� ����������� ���������
                int* arr = result.getArray(); // �������� ��������� �� ������ ����� �����

                int arrSize = result.getSize(); // �������� ������ ������� � ������

                // ���������� � ������ ��� ������ (���������)
                std::stringstream ss;
                for (int i = 0; i < arrSize; i++) {
                    ss << arr[i] << " ";
                }

                std::string resultString = ss.str();

                send(clientSocket, resultString.c_str(), resultString.size(), 0); // ���������� ��������� �������
                break;
            }
            else { // ���� ������ ���������� ���������, �������� {1,2,3} + {1,2} => ���� ����
                // � ���������� Different indexes
                std::string resultString = "Different indexes";
                send(clientSocket, resultString.c_str(), resultString.size(), 0); // ���������� ��������� �������
                break;
            }

            
        }
        case 6: { // ������ �� ���������/������� ������� �� ������
            char buf[100];
            recv(clientSocket, buf, sizeof(buf), 0);

            int operation_code = 0; // ���� ������������/����������� ������� �������
            int j = 0;
            while (buf[j] != '#') {
                operation_code = operation_code * 10 + buf[j] - '0';
                j++;
            }
            char operation = static_cast<char>(operation_code);

            int numVector1 = 0; // ����� 1�� �������
            int i = j + 1;
            while (buf[i] != '#') {
                numVector1 = numVector1 * 10 + buf[i] - '0';
                i++;
            }

            int Scalar = 0; // ������
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


            int* arr = result.getArray(); // �������� ��������� �� ������ ����� �����

            int arrSize = result.getSize(); // �������� ������ ������� � ������
            // ���������� � ������ ��� ������ (���������)
            std::stringstream ss;
            for (int i = 0; i < arrSize; i++) {
                ss << arr[i] << " ";
            }

            std::string resultString = ss.str();

            send(clientSocket, resultString.c_str(), resultString.size(), 0); // ���������� ��������� �������
            break;
        }
        case 7: { // ����� ������������/����������� ������� �������
            char buf[100];
            recv(clientSocket, buf, sizeof(buf), 0);

            int operation_code = 0; // ���� ������������/����������� ������� �������
            int j = 0;
            while (buf[j] != '#') {
                operation_code = operation_code * 10 + buf[j] - '0';
                j++;
            }
            char operation = static_cast<char>(operation_code);

            int numVector1 = 0; // ����� 1�� �������
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

            char resultStr[20]; // �������� ����� ��� �������� ������
            sprintf(resultStr, "%d", result); // ���������� �������� � �����

            send(clientSocket, resultStr , sizeof(resultStr), 0); // ���������� ��������� �������
            break;
        }
        case 8: {
            char buf[100];
            recv(clientSocket, buf, sizeof(buf), 0);

            int numVector1 = 0; // ����� 1�� �������
            int i = 0;
            while (buf[i] != '#') {
                numVector1 = numVector1 * 10 + buf[i] - '0';
                i++;
            }

            int* arr = vCollection.getVector(numVector1).getArray(); // �������� ��������� �� ������ ����� �����

            int arrSize = vCollection.getVector(numVector1).getSize(); // �������� ������ ������� � ������
            // ���������� � ������ ��� ������ (���������)
            std::stringstream ss;
            for (int i = 0; i < arrSize; i++) {
                ss << arr[i] << " ";
            }

            std::string result = ss.str();

            send(clientSocket, result.c_str(), result.size(), 0); // ���������� ��������� �������
            break;
        }
        case 9: {
            closesocket(clientSocket); // ��������� �����
            break;
        }
        default: {
            break;
        }
        }
    }
    
}

int main() {
    // ������� 4 ������� �����������
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
    // ��������� ������� � ��������� ��������
    vCollection.addVector(v1);
    vCollection.addVector(v2);
    vCollection.addVector(v3);
    vCollection.addVector(v4);

    // ������������� Winsock
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(wVersionRequested, &wsaData) != 0) {
        cerr << "Failed to initialize Winsock.\n";
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET; // ���������� TCP
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // ip ����� ���������� �����
    serverAddress.sin_port = htons(12345); // ��� ����

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0); // ������� ���������� ������

    // �������� �� ����������� ������
    if (::bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to bind server socket.\n";
        return 1;
    }
    // ������� ������� �� �������
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Failed to listen on server socket.\n";
        closesocket(serverSocket);
        return 1;
    }

    cout << "Server is listening on port " << 12345 << endl;

    while (true) {
        // �������� ������
        int clientSocket = accept(serverSocket, NULL, NULL); // ������� ����� �����������
        if (clientSocket == -1) {
            std::cerr << "Failed to accept client connection.\n";
            continue;
        }
        // ����� �����
        std::thread t(processRequest, clientSocket, std::ref(vCollection)); // ��������� ����� ����� ��� ������� ������� � ���������� clientThread
        t.detach(); // ������������ ��� ������������ ������ �� �������� ��������, 
        // ����� ������� ������� ��� ���������� ������������ ����� �����������.
    }

    // ��������� ��������� �����
    closesocket(serverSocket);

    return 0;
}