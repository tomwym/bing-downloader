#pragma once

#include <memory>
#include <optional>
#include <vector>

namespace Naive {

struct port_t;
struct addr_t;
enum class error_t;
class SelfClosingSocket;
using message_t = std::string;
using socket_t = int;


class NaiveSocketComponents {
public:
    NaiveSocketComponents() = delete;
    NaiveSocketComponents(const addr_t&, const port_t&);
    NaiveSocketComponents(std::unique_ptr<SelfClosingSocket>&& socket);
    ~NaiveSocketComponents();

    [[nodiscard]] auto Success() const -> error_t;
    [[nodiscard]] auto Bind() const -> error_t;
    [[nodiscard]] auto Connect() const -> error_t;
    [[nodiscard]] auto Listen(const std::size_t max_connections) const -> error_t;
    [[nodiscard]] auto Accept() -> std::unique_ptr<SelfClosingSocket>;
    [[nodiscard]] auto Read() const -> message_t;
                  auto Write(const message_t& message) const -> void;
                  auto Write(const std::vector<char>& message) const -> void;

private:
    class impl;
    std::unique_ptr<impl> pImpl;
};

}