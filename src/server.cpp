#include <winsock2.h>
#include <fstream>
#include <iostream>

SOCKET createSocket(int portNum);

int main(){
    const int portNum = 2468;

    SOCKET server = createSocket(portNum);

    return 0;
}

SOCKET createSocket(int portNum) {
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

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET){
        std::cerr << "Unable to create socket. Following error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    } else {
        std::clog << "Successfully created client-side socket!" << std::endl;
    }
}
