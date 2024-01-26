#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Soket oluşturma
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // Soket seçeneklerini ayarlama
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Soketi bağlama
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Dinleme
    listen(server_fd, 3);

    // Bağlantıyı kabul etme
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    while (true) {
        memset(buffer, 0, 1024);
        read(new_socket, buffer, 1024);
        std::cout << "Mesaj alındı: " << buffer << std::endl;
        
        // Belirli bir kelimeyi kontrol etme
        if (std::string(buffer) == "dur") {
            std::cout << "Durduruldu" << std::endl;
            break;
        }
    }

    // Soketi kapatma
    close(new_socket);
    close(server_fd);
    return 0;
}
