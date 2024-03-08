#include <winsock2.h>
#include "client.h"


int main(){
    WSAData wsaData{};
    int wsaerr;
    WORD wVersion = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersion, &wsaData);

    if (wsaerr != 0) {
        std::cout << "Windows Socket was unable to initialize." << std::endl;
        return 0;
    } else {
        std::cout << "Windows Socket was successfully initialized with status: " << wsaData.szSystemStatus << std::endl;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET){
        std::cout << "Unable to create socket. Following error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    } else {
        std::cout << "Successfully created client-side socket!" << std::endl;
    }

    SOCKADDR_IN serverAddr{AF_INET,
                           htons(2468),
                           static_cast<u_char>(inet_addr("127.0.0.1"))};



    return 0;
}
