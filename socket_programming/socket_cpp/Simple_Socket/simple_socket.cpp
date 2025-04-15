#include "simple_socket.hpp"

HDE::simpleSocket::simpleSocket(int copyDomain, int copyServce, int copyProtocol, int copyPort, u_long interface) :
    Domain(copyDomain),
    Servce(copyServce),
    Protocol(copyProtocol),
    Port(copyPort) {                    
    // Define address struct
    this->addrass.sin_family = this->Domain;
    this->addrass.sin_port = htons(this->Port);
    this->addrass.sin_addr.s_addr = htonl(interface);
    
    // Establish socket
    this->Sock = socket(this->Domain, this->Servce, this->Protocol);
    this->test_connection(this->Sock);
}

void HDE::simpleSocket::initialize_connection(void) {
    this->connection = this->connect_to_network(this->Sock, this->addrass);
    this->test_connection(this->connection);
}

void HDE::simpleSocket::test_connection(int copyItemTest) {
    if (copyItemTest < 0) {
        perror("Failed to connect ....");
        exit(EXIT_FAILURE);
    }
}