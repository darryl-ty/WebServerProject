
#include "server.h"

int main(){
    WSAData wsaData;
    int wsaerr;
    WORD wVersion = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersion, &wsaData);

    if (wsaerr != 0) {
        std::cout << "Windows Socket was unable to initialize." << std::endl;
        return 0;
    } else {
        std::cout << "Windows Socket was successfully initialized with status: " << wsaData.szSystemStatus << std::endl;
    }
    return 0;
}
