#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char *end_word = "QUIT";
    
    // Sunucu soketi oluşturma
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Sunucu soketi yapılandırma
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("192.168.1.100"); // Sunucu IP adresi
    address.sin_port = htons(8080);

    // Soketi belirli bir porta bağlama
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Bağlantıları dinleme
    listen(server_fd, 3);

    // Gelen bağlantıyı kabul etme
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    // İstemciden gelen mesajları okuma
    while (true) {
        memset(buffer, 0, 1024);
        read(new_socket, buffer, 1024);
        std::cout << "Mesaj: " << buffer << std::endl;
        if (strcmp(buffer, end_word) == 0) {
            break;
        }
    }

    // Soketi kapatma
    close(new_socket);
    close(server_fd);

    return 0;
}
