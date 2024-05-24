#include <chrono>
#include <iostream>
#include <format>
#include <syncstream>

#include "SimpleFoxServer.h"
#include "NaiveSocketComponents.h"
#include "BasicTypes.h"
#include "SelfClosingSocket.h"
#include "pure-functions/FileIO.h"

SimpleFoxServer::SimpleFoxServer(const std::filesystem::path& fox_path)
: _server{Naive::NaiveSocketComponents{Naive::addr_t{"0.0.0.0"}, Naive::port_t{8080}}}
, _continue{true}
, _active_transfers{0}
, _response_content{ReadFromFile(fox_path)}
, _response_header{[&content = _response_content]() -> Header {
    Header head;
    head.AddContentType(CONTENT_TYPE::IMAGE_JPEG)
        .AddContentLength(content.size())
        .AddDate();
    return head;
}()}
, _queue_mutex{}
, _incoming_queue{}
, _worker_pool{_continue, std::bind(&SimpleFoxServer::Task, this), 8}
{
    if (_server.Bind() == Naive::error_t::FAILED)
        return;
    if (_server.Listen(2) == Naive::error_t::FAILED)
        return;

    _distributor_thread = std::thread{std::bind(&SimpleFoxServer::Distribute, this)};
}

SimpleFoxServer::~SimpleFoxServer() {
    _continue = false;
    _worker_pool.ShutDown();
    _distributor_thread.join();
}

auto SimpleFoxServer::MainThreadSpinner() -> void {
    while (true) {

    }
        // if (_active_transfers > 3)
        //     std::osyncstream{std::cout} << _active_transfers << '\n';
}

auto SimpleFoxServer::Distribute() -> void {
    while (_continue) {
        std::unique_ptr<Naive::NaiveSocketComponents> connection{std::make_unique<Naive::NaiveSocketComponents>(_server.Accept())};
        // Ignore the read, we don't care the request we will send the fox
        const std::string buffer{connection->Read()};
        if (connection)
        {
            std::lock_guard lock{_queue_mutex};
            _incoming_queue.emplace(std::move(connection));
            // std::cout << "Emplacing new connection.\n";
        }
    }
}

auto SimpleFoxServer::Task() -> void {
    std::unique_ptr<Naive::NaiveSocketComponents> connection;
    {
        std::lock_guard<std::mutex> lock{_queue_mutex};
        if (_incoming_queue.empty()) {
            return;
        }
        connection = std::move(_incoming_queue.front());
        ++_active_transfers;
        _incoming_queue.pop();
        // std::cout << "Connection popped.\n";
    }
    // std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    connection->Write(_response_header);
    connection->Write(_response_content);
    const unsigned int sleep_for_ms{50};
    // std::this_thread::sleep_for(std::chrono::milliseconds(sleep_for_ms));

    --_active_transfers;
    // std::osyncstream bout(std::cout);
    // bout << "Thread " << std::this_thread::get_id() << " slept for " << sleep_for_ms << '\n';
}
