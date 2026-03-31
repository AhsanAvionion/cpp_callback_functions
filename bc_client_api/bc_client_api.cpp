#include "bc_client_api.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <atomic>
#include <mutex>
#include <iostream>

struct bc_client_api::Impl {
    int sockfd;
    std::thread recvThread;
    std::atomic<bool> running;

    CallbackTypeA callbackA;
    CallbackTypeB callbackB;

    std::mutex cbMutex;
};

static void recv_loop(bc_client_api::Impl* client)
{
    char buffer[1024];

    while (client->running)
    {
        int n = recv(client->sockfd, buffer, sizeof(buffer), 0);

        if (n <= 0) {
            usleep(1000);
            continue;
        }

        unsigned char firstByte = buffer[0];
        // Combine first two bytes into an unsigned short (big-endian)
        unsigned short header = (static_cast<unsigned short>(buffer[0]) << 8) | static_cast<unsigned short>(buffer[1]);

        std::lock_guard<std::mutex> lock(client->cbMutex);

        if (firstByte == 0xAA && client->callbackA)
        {
            client->callbackA(std::string(buffer, n));
        }
        else if (firstByte == 0xBB && client->callbackB)
        {
            client->callbackB(std::string(buffer, n));
        }


        else if (header == 0xAAAA && client->callbackA)
        {
            client->callbackA(std::string(buffer, n));
        }
    }
}

// ======================
// Class implementation
// ======================

bc_client_api::bc_client_api()
{
    impl = new Impl();
    impl->sockfd = -1;
    impl->running = false;
}

bc_client_api::~bc_client_api()
{
    disconnect();
    delete impl;
}

bool bc_client_api::connect(const std::string& host, int port)
{
    impl->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (impl->sockfd < 0) return false;

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &server.sin_addr) <= 0)
        return false;

    if (::connect(impl->sockfd, (sockaddr*)&server, sizeof(server)) < 0)
        return false;

    impl->running = true;
    impl->recvThread = std::thread(recv_loop, impl);

    return true;
}

void bc_client_api::disconnect()
{
    if (!impl->running) return;

    impl->running = false;

    if (impl->recvThread.joinable())
        impl->recvThread.join();

    if (impl->sockfd >= 0)
    {
        close(impl->sockfd);
        impl->sockfd = -1;
    }
}

bool bc_client_api::send(const std::string& data)
{
    if (impl->sockfd < 0) return false;

    return ::send(impl->sockfd, data.c_str(), data.size(), 0) >= 0;

    //char buffer[1024]="AHSAN";
    //int n=10;
    //impl->callbackA(std::string(buffer, n));


    return false;
}

void bc_client_api::setCallbackTypeA(CallbackTypeA cb)
{
    std::lock_guard<std::mutex> lock(impl->cbMutex);
    impl->callbackA = cb;
}

void bc_client_api::setCallbackTypeB(CallbackTypeB cb)
{
    std::lock_guard<std::mutex> lock(impl->cbMutex);
    impl->callbackB = cb;
}
