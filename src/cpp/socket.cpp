// Copyright @2Dou@github.com

#include "./socket.hpp"

#ifdef __win
bool Socket::inited = false;
#endif

#include <iostream>

SocketHandler::SocketHandler(int fd) {
    this->fd = fd;
}

SocketHandler::~SocketHandler() {
    std::cout << "close" << std::endl;
    if (this->fd != -1) {
        close(this->fd);
    }
}

SocketHandler::operator int() const {
    return this->fd;
}

Socket::Socket(int domain, int sock_type, int protocol) {
#ifdef __win
    if (!inited) {
        inited = true;
        this->init_windows();
    }
#endif
    int fd = socket(domain, sock_type, protocol);
    this->fd = std::shared_ptr<SocketHandler>(new SocketHandler(fd));
}

#ifdef __win
void Socket::init_windows() {
    // Todo: WSAStartup
}
#endif

Socket::~Socket() {
}

Socket& Socket::operator=(const Socket &other) {
    this->fd = other.fd;
}

Socket::operator int() const {
    return *this->fd;
}

bool Socket::operator==(const Socket &other) const {
    return *this->fd == *other.fd;
}
bool Socket::operator==(const int &other) const {
    return *this->fd == other;
}
