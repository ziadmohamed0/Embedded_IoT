#include "bindingSocket.hpp"


int HDE::bindingSocket::connect_to_network(int copySock, struct sockaddr_in copyAddrass) {
    return bind(this->Sock, (struct sockadd*) &copyAddrass, sizeof(this->addrass));
}