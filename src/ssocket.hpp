#ifndef __SSOCKET__
#define __SSOCKET__

#include "socket.hpp"
#include <stdint.h>

namespace socket_raii {
  class Socket;

	class ServerSocket {
  private:
		int sock;
		bool own {false};

	public:
    ServerSocket(uint16_t port, int backlog);
    virtual ~ServerSocket();

		ServerSocket(const ServerSocket& ssoket) = delete;
		ServerSocket& operator=(const ServerSocket& ssoket) = delete;

		ServerSocket(ServerSocket&& rssocket);
		ServerSocket& operator=(ServerSocket&& rssocket);

    int getFileDescriptor();

		Socket accept();
  };
}

#endif
