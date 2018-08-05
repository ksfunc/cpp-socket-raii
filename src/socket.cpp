#include "socket.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

namespace socket_raii {
  Socket::Socket(int&& rxsock) : sock {rxsock}, own {true} {}

	Socket::Socket(const std::string& ipaddr, uint16_t port) {
		sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock < 0) throw SocketException{};

		struct sockaddr_in addr {};
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ipaddr.c_str());

		if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
      throw SocketException{};
    }

		own = true;
	}

	Socket::Socket(Socket&& rxsocket) : sock {rxsocket.sock}, own {true} {
		rxsocket.own = false;
	}

	Socket& Socket::operator=(Socket&& rxsocket) {
		if (own) {
      shutdown(sock, SHUT_RDWR);
      close(sock);
    }

		sock = rxsocket.sock;
		own = true;
		rxsocket.own = false;

		return *this;
	}

	Socket::~Socket() {
		if (own) {
      shutdown(sock, SHUT_RDWR);
      close(sock);
    }
	}

  int Socket::getFileDescriptor() {
    return sock;
  }

	size_t Socket::send(const std::string& in) {
    ssize_t send_size {::send(sock, in.c_str(), in.length(), MSG_NOSIGNAL)};
		if (send_size == -1) {
      throw SocketException{};
    }
    return send_size;
	}

  size_t Socket::send(const char *in) {
    ssize_t send_size {::send(sock, in, sizeof(char) * strlen(in), MSG_NOSIGNAL)};
    if (send_size == -1) {
      throw SocketException{};
    }
    return send_size;
  }

  size_t Socket::send(const char *in, size_t size) {
    ssize_t send_size {::send(sock, in, sizeof(char) * size, MSG_NOSIGNAL)};
    if (send_size == -1) {
      throw SocketException{};
    }
    return send_size;
  }

	size_t Socket::recv(std::string& out, size_t size) {
    char *buf {new char[size]};

    ssize_t recv_size {::recv(sock, buf, sizeof(char) * size, 0)};
		if (recv_size == -1) {
      delete[] buf;
			throw SocketException{};
		}

    out = std::string{buf, (size_t)recv_size};
    delete[] buf;

    return recv_size;
	}

  size_t Socket::recv(char *out, size_t size) {
    ssize_t recv_size {::recv(sock, out, sizeof(char) * size, 0)};
		if (recv_size == -1) {
			throw SocketException{};
		}
    out[recv_size] = '\0';
    return recv_size;
  }
}
