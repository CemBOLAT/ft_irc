#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    struct sockaddr_in serv_addr;
    int sock = 0;
    const char *end_word = "QUIT";
    
    // İstemci soketi oluşturma
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Sunucu adresini yapılandırma
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "192.168.1.100", &serv_addr.sin_addr); // Sunucu IP adresi

    // Sunucuya bağlanma
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // Kullanıcıdan gelen mesajları sunucuya gönderme
    while (true) {
        char buffer[1024] = {0};
        std::cout << "Mesajınızı yazın: ";
        std::cin.getline(buffer, 1024);
        send(sock, buffer, strlen(buffer), 0);

        if (strcmp(buffer, end_word) == 0) {
            break;
        }
    }

    // Soketi kapatma
    close(sock);

    return 0;
}
