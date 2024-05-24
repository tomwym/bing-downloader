#include "SelfClosingSocket.h"

#include <iostream>
#include <thread>

#include <unistd.h> // close
#include <sys/socket.h>

namespace Naive {

SelfClosingSocket::SelfClosingSocket(const socket_t& socket)
: _socket{socket}
{}

SelfClosingSocket::operator socket_t() const { return _socket; }

SelfClosingSocket::~SelfClosingSocket() {
    // if (shutdown(_socket, SHUT_RDWR) < 0) {
    //     std::cout << "SHUTDOWN ERROR.\n";
    // }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    close(_socket);
}

}