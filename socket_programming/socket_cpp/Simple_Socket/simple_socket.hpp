#ifndef _SIMPLE_SOCKET_HPP_
#define _SIMPLE_SOCKET_HPP_

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

namespace hde {
	class simpleSocket {
		private:
			int connection{0}, Domain{0}, Servce{0}, Protocol{0}, Port{0}, Sock{0};
			struct sockaddr_in addrass;
			public:
			simpleSocket(int copyDomain, int copyServce, int copyProtocol, int copyPort, u_long interface);
			
			virtual int connect_to_network(int copySock, struct sockaddr_in copyAddrass) = 0;
			void test_connection(int);
			struct sockaddr_in getAddress();
			int getSock();
			int getConnection();
	};

}

#endif
