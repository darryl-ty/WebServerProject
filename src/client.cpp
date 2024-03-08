#include <winsock2.h>
#include "client.h"

SOCKET createSocket(const char* serverAddr, int portNum);

int main(){
    const char* serverAddr = "127.0.0.1";
    int portNum = 2468;

    SOCKET client = createSocket(serverAddr, portNum);


    closesocket(client);
    WSACleanup();

    return 0;
}

SOCKET createSocket(const char* serverAddr, int portNum){
    WSAData wsaData;
    int wsaerr;
    WORD wVersion = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersion, &wsaData);

    if (wsaerr != 0) {
        std::cerr << "Windows Socket was unable to initialize." << std::endl;
        return 1;
    } else {
        std::clog << "Windows Socket was successfully initialized with status: " << wsaData.szSystemStatus << std::endl;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET){
        std::cerr << "Unable to create socket. Following error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    } else {
        std::clog << "Successfully created client-side socket!" << std::endl;
    }

    SOCKADDR_IN server{AF_INET,
                           htons(portNum),
                           static_cast<u_char>(inet_addr(serverAddr))};
    int result = connect(clientSocket, (SOCKADDR*)&server, sizeof(server));
    if (result == SOCKET_ERROR){
        std::cerr << "Unable to connect to server. Following error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    } else {
        std::clog << "Connection to server established!" << std::endl;
    }

    return clientSocket;
}
