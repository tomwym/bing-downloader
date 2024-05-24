#include "NaiveSocketComponents.h"
#include "BasicTypes.h"
#include "SelfClosingSocket.h"

#include <iostream>
#include <format>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string.h>
#include <thread>

// resource: https://stackoverflow.com/questions/27014955/socket-connect-vs-bind

namespace Naive {

using std::unique_ptr;
using std::make_unique;

/* 
 - This class is only responsible for reading the contents into some stream.
 - This class is not responsible for opening or closing any sockets.  
 */
class NaiveSocketComponentsReader {
    socket_t _connection;
public:
    NaiveSocketComponentsReader() = delete;
    NaiveSocketComponentsReader(const socket_t& connection)
    : _connection{connection}
    {
    }

    [[nodiscard]] auto ReadToMessageT(const std::size_t chunk_size) const -> message_t {
        std::ostringstream ss;
        ReadAsChunks(chunk_size, ss);
        return message_t{ss.str()};
    }

    auto ReadToFile(const std::size_t chunk_size, std::filesystem::path& path) -> void {
        if (!std::filesystem::exists(path.parent_path()))
            std::filesystem::create_directory(path.parent_path());
        std::ofstream file{path, std::ios::trunc};
        ReadAsChunks(chunk_size, file);
    }

private:
    auto ReadAsChunks(const std::size_t chunk_size, std::ostream& ostream) const -> void {
        std::string buffer(chunk_size, 0);
        // std::string contents;
        ssize_t read_size {0};
        do {
            read_size = recv(_connection, (void*)(buffer.c_str()), chunk_size, MSG_DONTWAIT);
            if (read_size > 0) {
                // contents.append(buffer.begin(), buffer.begin()+read_size);
                ostream << std::string{buffer.begin(), buffer.begin() + read_size};
            }
        } while(read_size > 0);
    }
};

class NaiveSocketComponentsSender {
    socket_t _connection;
public:
    NaiveSocketComponentsSender() = delete;
    NaiveSocketComponentsSender(const socket_t& connection)
    : _connection{connection}
    {
    }

    auto SendAsChunks(const std::vector<char>& message, const std::size_t chunk_size) const -> void  {
        int total_sent {0};
        send(_connection, message.data(), message.size(), 0);
        return;
        while (total_sent < message.size()) {
            auto sent = send(_connection, message.data()+total_sent, chunk_size, 0);

            if (sent == -1)
                std::cout << "Error\n";
            total_sent += sent;
            // std::cout << "send " << num_sent << " bytes.\n";
        }
        // std::cout << "Done sending.\n";
    }
private:

};


/* Intent
 - This class should serve as a wrapper over the basic socket functions 
 */
class NaiveSocketComponents::impl {
    addr_t _address;
    port_t _port;
    sockaddr_in _address_in;
    unique_ptr<SelfClosingSocket> _socket;

public:
    impl() = delete;
    impl(const addr_t& address, const port_t& port)
    : _address{address}
    , _port{port}
    , _address_in{AF_INET, _port.Get(), _address.Get(), {'\0'}}
    , _socket{make_unique<SelfClosingSocket>(socket(AF_INET, SOCK_STREAM, 0))}
    {
        if (!_socket)
            return;

        const int enable = 1;
        int rest {setsockopt(*_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enable, sizeof(int))};
        if (rest < 0)
            std:: cout << "error/.\n";
    }

    impl(unique_ptr<SelfClosingSocket>&& socket)
    : _socket{std::move(socket)}
    {}

    [[nodiscard]] auto Bind() -> error_t {
        const int bind_result {bind(*_socket,
                               (struct sockaddr*)(&_address_in),
                               sizeof(_address_in))};
        if (bind_result < 0)
        {
            fprintf(stderr, "bind: %s\n", strerror(errno));
        }
        return SuccessOrFail(bind_result,
                             std::format("Call to bind at address {} SUCCESSFUL.\n", _address.address),
                             std::format("Call to bind at address {} has FAILED.\n", _address.address));
    }
    
    [[nodiscard]] auto Connect() -> error_t {
        const int connect_result {connect(*_socket,
                               (struct sockaddr*)(&_address_in),
                               sizeof(_address_in))};
        if (connect_result < 0)
        {
            fprintf(stderr, "bind: %s\n", strerror(errno));
        }
        return SuccessOrFail(connect_result,
                             std::format("Call to Connect at address {} SUCCESSFUL.\n", _address.address),
                             std::format("Call to Connect at address {} has FAILED.\n", _address.address));
    }

    [[nodiscard]] auto Listen(const std::size_t& max_connections) -> error_t {
        const int listen_result {listen(*_socket, max_connections)};
        return SuccessOrFail(listen_result,
                             std::format("Call to listen SUCCESSFUL.\n"),
                             std::format("Call to listen has FAILED.\n"));
    }

    [[nodiscard]] auto Accept() -> std::unique_ptr<SelfClosingSocket> {
        int address_in_length{sizeof(sockaddr_in)};
        const socket_t new_socket = accept(*_socket,
                                           reinterpret_cast<sockaddr*>(&_address_in),
                                           reinterpret_cast<socklen_t*>(&address_in_length));
        if (new_socket < 0) {
            std::cerr << std::format("Call to accept has FAILED.\n");
            return nullptr;
        }
        else {
            std::cout << std::format("Call to accept SUCCSESFUL.\n");
            return std::make_unique<SelfClosingSocket>(new_socket);
        }
    }

    [[nodiscard]] auto Read() const -> message_t {
        NaiveSocketComponentsReader reader{*_socket};
        message_t message {reader.ReadToMessageT(10000)};
        return message;
    }

    auto Write(const message_t& message) const -> void {
        NaiveSocketComponentsSender send{*_socket};
        send.SendAsChunks({message.begin(), message.end()}, 1000);
    }

    auto Write(const std::vector<char>& message) const -> void {
        NaiveSocketComponentsSender send{*_socket};
        send.SendAsChunks(message, 1000);
    }

private:
    [[nodiscard]] auto SuccessOrFail(const int result, const std::string& success, const std::string& failed) -> error_t {
        if (result < 0) {
            std::cerr << failed;
            return error_t::FAILED;
        }
        else {
            std::cout << success;
            return error_t::SUCCESS;
        }
    }
};

NaiveSocketComponents::NaiveSocketComponents(const addr_t& address, const port_t& port)
: pImpl{std::make_unique<impl>(address, port)}
{
}

NaiveSocketComponents::NaiveSocketComponents(unique_ptr<SelfClosingSocket>&& socket)
: pImpl{std::make_unique<impl>(std::move(socket))}
{
}

NaiveSocketComponents::~NaiveSocketComponents() = default;

auto NaiveSocketComponents::Success() const -> error_t {
    return {};
}

auto NaiveSocketComponents::Bind() const -> error_t {
    return pImpl->Bind();
}

auto NaiveSocketComponents::Connect() const -> error_t {
    return pImpl->Connect();
}

auto NaiveSocketComponents::Listen(const std::size_t max_connections) const -> error_t {
    return pImpl->Listen(max_connections);
}

auto NaiveSocketComponents::Accept() -> std::unique_ptr<SelfClosingSocket> {
    return pImpl->Accept();
}

auto NaiveSocketComponents::Read() const -> message_t {
    return pImpl->Read();
}

auto NaiveSocketComponents::Write(const message_t& message) const -> void {
    return pImpl->Write(message);
}

auto NaiveSocketComponents::Write(const std::vector<char>& message) const -> void {
    return pImpl->Write(message);
}

}
