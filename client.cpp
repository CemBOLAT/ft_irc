#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    struct sockaddr_in serv_addr;
    int sock = 0;
    char buffer[1024] = {0};

    // Soket oluşturma
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Sunucu adresini çözümleme
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // Sunucuya bağlanma
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (true) {
        std::string mesaj;
        std::cout << "Mesaj girin: ";
        std::getline(std::cin, mesaj);

        send(sock, mesaj.c_str(), mesaj.length(), 0);

        if (mesaj == "dur") {
            break;
        }
    }

    // Soketi kapatma
    close(sock);
    return 0;
}
