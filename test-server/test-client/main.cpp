#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <unistd.h>
#include "BasicTypes.h"
#include "NaiveSocketComponents.h"
#include "pure-functions/FileIO.h"

#include <chrono>
#include <thread>


const std::string message {"Hello from client"};


int main() {
    Naive::addr_t address{"0.0.0.0"};
    Naive::port_t port{8080};
    Naive::NaiveSocketComponents client(address, port);

    if (client.Connect() == Naive::error_t::FAILED)
        return 0;

    client.Write(message);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    const std::string buffer{client.Read()};
    WriteToFile("res/new.jpg", buffer);
    return 0;
}