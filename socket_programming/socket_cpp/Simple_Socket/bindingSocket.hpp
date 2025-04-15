#ifndef _SIMPLE_SOCKET_BINDING_SOCKET_HPP_
#define _SIMPLE_SOCKET_BINDING_SOCKET_HPP_

#include <stdio.h>
#include "simple_socket.hpp"

namespace HDE {
	class bindingSocket : public simpleSocket {
		private:
			
		public:
			bindingSocket(int copyDomain, int copyServce, int copyProtocol, int copyPort, u_long interface) : 
			simpleSocket(copyDomain, copyServce, copyProtocol, copyPort, interface) {}
			int connect_to_network(int copySock, struct sockaddr_in copyAddrass);
	};
}

#endif
