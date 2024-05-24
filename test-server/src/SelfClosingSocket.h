#pragma once

namespace Naive {

using socket_t = int;

class SelfClosingSocket {
    socket_t _socket;
public:
    SelfClosingSocket() = delete;
    SelfClosingSocket(const socket_t& socket);
    ~SelfClosingSocket();
    operator socket_t() const;
};

}
