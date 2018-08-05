#ifndef __SOCKET__
#define __SOCKET__

#include "ssocket.hpp"
#include <string>
#include <stdint.h>

namespace socket_raii {
  class SocketException : std::exception {};

  class Socket {
    friend class ServerSocket;

  private:
    int sock;
    bool own {false};

    Socket(int&& rsock);

  public:
    Socket(const std::string& ipaddr, uint16_t port);
    virtual ~Socket();

    Socket(const Socket& socket) = delete;
    Socket& operator=(const Socket& socket) = delete;

    Socket(Socket&& rsocket);
    Socket& operator=(Socket&& rsocket);

    int getFileDescriptor();

    size_t send(const std::string& in);
    size_t send(const char *in);
    size_t send(const char *in, size_t size);
    size_t recv(std::string& out, size_t size);
    size_t recv(char *out, size_t size);
  };
}

#endif
