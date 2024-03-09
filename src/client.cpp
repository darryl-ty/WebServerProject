#include <winsock2.h>
#include <fstream>
#include <filesystem>
#include "client.h"

SOCKET createSocket(const char* serverAddr, int portNum);

char* sendFileName(SOCKET client);
char* receiveFile(SOCKET client, char* fileName);

void createNewFile(char *fileName, char* fileContents);

int main(){
    const char* serverAddr = "127.0.0.1";
    const int portNum = 2468;

    SOCKET client = createSocket(serverAddr, portNum);
    char* fileName = sendFileName(client);
    char* fileContents = receiveFile(client, fileName);
    createNewFile(fileName, fileContents);


    closesocket(client);
    WSACleanup();

    return 0;
}

void createNewFile(char* fileName, char* fileContents) {
    std::string newFileBaseName;
    std::string newFileExtension;
    std::string newFileName;
    bool EXTENSION_FLAG = false;

    for (size_t i = 0; i < strlen(fileName); i++){
        if (fileName[i] == '.'){
               EXTENSION_FLAG = true;
        } else if (!EXTENSION_FLAG) {
            newFileBaseName += fileName[i];
        } else {
            newFileExtension += fileName[i];
        }
    }

    newFileName.append(newFileBaseName + "_clt." + newFileExtension);
    std::ofstream newFile(std::filesystem::current_path().append(newFileName));

    newFile << fileContents;
}

char* receiveFile(SOCKET client, char* fileName){
    const int bufferSize = 2048;
    static char dataBuffer[bufferSize];
    int dataSize;

    int result = recv(client, dataBuffer, dataSize, 0);
    if (result == SOCKET_ERROR){
        std::cerr << "The file " << std::string(fileName) << " is not available." << std::endl;
        WSACleanup();
        exit(1);
    } else {
        std::clog << "User received requested file from server!" << std::endl;
        return dataBuffer;
    }

}

char* sendFileName(SOCKET client){
    const int bufferSize = 64;
    static char fileName[bufferSize];

    std::cout << "Please enter the name of the file you'd like to get:" << std::endl;
    std::cin >> fileName;

    int result = send(client, fileName, sizeof(fileName), 0);
    if (result == SOCKET_ERROR){
        std::cerr << "Unable to send information to server. Following error: " << WSAGetLastError() << std::endl;
        exit(1);
    } else {
        std::clog << "User requested " << fileName << " from server. Waiting for response..." << std::endl;
    }

    return fileName;


}

SOCKET createSocket(const char* serverAddr, int portNum){
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

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET){
        std::cerr << "Unable to create socket. Following error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(1);
    } else {
        std::clog << "Successfully created client-side socket!" << std::endl;
    }

    SOCKADDR_IN server;
    server.sin_family = AF_INET;
    server.sin_port = htons(portNum);
    server.sin_addr.s_addr = inet_addr(serverAddr);

    int result = connect(clientSocket, (SOCKADDR*)&server, sizeof(server));
    if (result == SOCKET_ERROR){
        std::cerr << "Unable to connect to server. Following error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(1);
    } else {
        std::clog << "Connection to server established!" << std::endl;
    }

    return clientSocket;
}
