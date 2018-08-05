#include "socket.hpp"
#include "ssocket.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdint.h>

namespace socket_raii {
	ServerSocket::ServerSocket(uint16_t port, int backlog) {
		int status {};

		sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == -1) {
			throw SocketException{};
		}

		struct sockaddr_in addr {};
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = INADDR_ANY;
		status = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
		if (status == -1) {
			throw SocketException{};
		}

		status = listen(sock, backlog);
		if (status == -1) {
			throw SocketException{};
		}

		own = true;
	}

  ServerSocket::~ServerSocket() {
    if (own) {
      shutdown(sock, SHUT_RDWR);
      close(sock);
    }
  }

	ServerSocket::ServerSocket(ServerSocket&& rxssocket): sock {rxssocket.sock}, own {true} {
		rxssocket.own = false;
	}

	ServerSocket& ServerSocket::operator=(ServerSocket&& rxssocket) {
		if (own) {
      shutdown(sock, SHUT_RDWR);
      close(sock);
    }

		sock = rxssocket.sock;
		own = true;
		rxssocket.own = false;

		return *this;
	}

  int ServerSocket::getFileDescriptor() {
    return sock;
  }


	Socket ServerSocket::accept() {
    int csock {::accept(sock, nullptr, nullptr)};
    if (csock == -1) {
      throw SocketException{};
    }

		return {std::move(csock)};
	}
}
