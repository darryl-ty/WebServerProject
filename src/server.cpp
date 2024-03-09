#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <filesystem>


int createSocket(int portNum);
std::string receiveInformation(int connection);

void sendFileInformation(int connection, std::string &fileToSend);

int main(){
    const int portNum = 2468;

    int server = createSocket(portNum);
    std::string file = receiveInformation(server);
    sendFileInformation(server, file);

    closesocket(server);
    WSACleanup();
    return 0;
}

void sendFileInformation(int connection, std::string &fileToSend) {

}

std::string receiveInformation(int connection){
    char* dataBuffer;


    int fileName = recv(connection, dataBuffer, sizeof(dataBuffer), 0);
    if (fileName == SOCKET_ERROR){
        std::cerr << "Unable to bind socket. Following error: " << WSAGetLastError() << std::endl;
        closesocket(connection);
        WSACleanup();
        exit(1);
    }

    if (std::filesystem::exists(dataBuffer)){
        std::cout << "Client requested file exists! Sending information to client..." << std::endl;
    } else {
        std::cout << "The file " << std::string(dataBuffer) << " is not available. Notifying client..." << std::endl;
    }

    return std::string{dataBuffer};
}

int createSocket(int portNum) {
    WSAData wsaData;
    int wsaerr;
    WORD wVersion = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersion, &wsaData);

    if (wsaerr != 0) {
        std::cerr << "Windows Socket was unable to initialize." << std::endl;
        exit(1);
    } else {
        std::clog << "Windows Socket was successfully initialized with status: " << wsaData.szSystemStatus << std::endl;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET){
        std::cerr << "Unable to create socket. Following error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(1);
    } else {
        std::clog << "Successfully created server-side socket!" << std::endl;
    }

    SOCKADDR_IN serverAddress {AF_INET,
                               htons(portNum),
                               INADDR_ANY};
    int serverBinding = bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
    if (serverBinding == -1){
        std::cerr << "Unable to bind socket. Following error: " << strerror(serverBinding) << std::endl;
        WSACleanup();
        exit(1);
    } else {
        std::clog << "Server was successfully bound to port " << portNum << "! Listening for connections" << std::endl;
    }

    listen(serverSocket, 1);
    int clientConnection = accept(serverSocket, nullptr, nullptr);
    if (clientConnection == -1){
        std::cerr << "Unable to accept client connection. Following error: " << strerror(clientConnection) << std::endl;
        WSACleanup();
        exit(1);
    } else {
        std::clog << "Server established connection to client on " << portNum << "! Waiting for information..." << std::endl;
        return clientConnection;
    }

}
