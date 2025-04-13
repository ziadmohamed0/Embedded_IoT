#include "simple_socket.hpp"

hde::simpleSocket(int copyDomain, int copyServce, int copyProtocol, int copyPort, u_long interface) :
 					Domain(copyDomain),
 					Servce(copyServce),
 					Protocol(copyProtocol),
 					Port(copyPort){
 					
 	// Define address struct
	addrass.sin_family = Domain;
	addrass.sin_port = htons(Port);
	addrass.sin_addr.s_addr =  htonl(interface);
	
	// Estublish sock
	Sock = sockt(Domain, Servce, Protocol);
	test_connection(Sock);
	
	// Estublish network connection
	connection = connect_to_network(Sock, addrass);
	test_connection(connection);
	// Binding
}

void hde::test_connection(int copyItemTest) {
	if(copyItemTest < 0) {
		perror("Faild to connect ....");
		exit(EXIT_FAILURE);
	}
}

struct sockaddr_in getAddress() {
	return addrass;
}

int getSock() {
	return Sock;
}

int getConnection() {
	return connection;
}
