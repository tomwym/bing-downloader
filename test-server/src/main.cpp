#include "NaiveSocketComponents.h"
#include "BasicTypes.h"
#include "SelfClosingSocket.h"
#include "HTTPHeaderParse.h"
#include "pure-functions/FileIO.h"
#include "NaiveHeader.cpp"

#include <iostream>
#include <thread>



auto main() -> int {
    auto content = ReadFromFile("res/fox.jpg");

    // std::cout << std::string{content.begin(), content.end()} << '\n';
    // WriteToFile("res/test.jpg", content);

    Naive::addr_t address{"0.0.0.0"};
    Naive::port_t port{8080};
    Naive::NaiveSocketComponents server(address, port);
    if (server.Bind() == Naive::error_t::FAILED)
        return 0;
    if (server.Listen(2) == Naive::error_t::FAILED)
        return 0;
    int number = 0;

    {
        Naive::NaiveSocketComponents connection{server.Accept()};
        const std::string buffer{connection.Read()};
        std::cout << buffer << '\n';
        // Naive::ReadHead(buffer);
        // std::string hello = "HTTP/1.1 200 OK\nContent-Type: image/jpeg\nContent-Length: \n\nHello world!";

        Header head;
        head.AddContentType(CONTENT_TYPE::IMAGE_JPEG)
            .AddContentLength(content.size())
            .AddDate();
        connection.Write(head);
        connection.Write(content);
    }

    return 0;
}
