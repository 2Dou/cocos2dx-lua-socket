// Copyright @2Dou@github.com

#include <thread>

#include "./socket_client.hpp"

#define RECV_BUFF_SIZE  4096

int SocketClient::parseHost(const std::string &domain, int sock_type
        , std::string &ip) {
    struct addrinfo *resolved = nullptr;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    hints.ai_socktype = sock_type;
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;

    int rc = getaddrinfo(domain.c_str(), nullptr, &hints, &resolved);
    if (rc != 0) {
        return -1;
    }

    int addr_family = resolved->ai_family;

    ip.clear();
    switch (addr_family) {
        case AF_INET: {
            char buf[INET_ADDRSTRLEN] = {0};
            struct sockaddr_in *in = (struct sockaddr_in *)resolved->ai_addr;
            const char* ip_address = inet_ntop(resolved->ai_family
                , &in->sin_addr, buf, sizeof(buf));
            if (ip_address) {
                ip = ip_address;
            }
            break;
        }

        case AF_INET6: {
            char buf[INET6_ADDRSTRLEN] = {0};
            struct sockaddr_in6 *in = (struct sockaddr_in6 *)resolved->ai_addr;
            const char* ip_address = inet_ntop(resolved->ai_family
                , &in->sin6_addr, buf, sizeof(buf));
            if (ip_address) {
                ip = ip_address;
            }
            break;
        }

        default:
            break;
    }
    freeaddrinfo(resolved);

    if (ip.empty()) {
        return -1;
    }

    return addr_family;
}

int SocketClient::parseIP(std::string ip, int sock_type) {
    struct addrinfo *resolved = nullptr;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = sock_type;
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;

    int rc = getaddrinfo(ip.c_str(), nullptr, &hints, &resolved);
    if (rc != 0) {
        return -1;
    }

    int af = resolved->ai_family;
    freeaddrinfo(resolved);

    return af;
}

SocketClient::SocketClient(ISocketObserver *observer)
    : fd(nullptr)
    , thread_exit_flag(nullptr)
    , state(ISocketObserver::State::kInit)
    , observer(observer) {
}

SocketClient::~SocketClient() {
}


void SocketClient::ConnectIP(std::string ip, int port
    , int sock_type /*= SOCK_STREAM*/) {

    auto fn = [=](){
        int af = SocketClient::parseIP(ip, sock_type);
        if (af == -1) {
            this->setStateAndNotify(ISocketObserver::State::kClosed, "failed to parse ip");
        }

        this->connect(af, ip, port, sock_type);
    };
    // 新开线程进行连接
    std::thread t{fn};
    t.detach();
}

void SocketClient::ConnectHost(std::string domain, int port
    , int sock_type /*= SOCK_STREAM*/) {
    auto fn = [=](){
        std::string ip;
        int af = SocketClient::parseHost(domain, sock_type, ip);
        if (af == -1) {
            this->setStateAndNotify(ISocketObserver::State::kClosed, "failed to parse ip");
        }

        this->connect(af, ip, port, sock_type);
    };
    // 新开线程进行连接
    std::thread t{fn};
    t.detach();
}

int SocketClient::Send(unsigned char *buff, int len) {
    int sent = 0;
    while (sent < len) {
        int n = ::send(*this->fd, buff + sent, len - sent, 0);
        if (n <= 0) {
            return 0;
        }

        sent += n;
    }

    return 0;
}

int SocketClient::createSocket(int af, int sock_type, int protocol) {
    this->fd = new Socket(af, sock_type, 0);
    return *this->fd;
}

void SocketClient::releaseSocket() {
    if (this->fd) {
        delete this->fd;
        this->fd = nullptr;
    }
}

void SocketClient::connect(int af, std::string ip, int port, int sock_type) {
    if (this->createSocket(af, sock_type, 0) == -1) {
        this->releaseSocket();
        this->setStateAndNotify(ISocketObserver::State::kClosed
            , "failed to create socket");
        return;
    }

    this->state = ISocketObserver::State::kConnecting;

    struct sockaddr* addr;
    int sizeaddr = 0;
    if (af == AF_INET) {
        // IPv4
        sizeaddr = sizeof(struct sockaddr_in);
        auto svraddr = (struct sockaddr_in*)malloc(sizeaddr);
#if !defined(__linux__) && !defined(__win)
        svraddr->sin_len = sizeaddr;
#endif
        svraddr->sin_family = AF_INET;
        inet_pton(AF_INET, ip.c_str(), &(svraddr->sin_addr));
        svraddr->sin_port = htons(port);

        addr = (struct sockaddr*)svraddr;
    } else if (af == AF_INET6) {
        // IPv6
        sizeaddr = sizeof(struct sockaddr_in6);
        auto svraddr = (struct sockaddr_in6*)malloc(sizeaddr);

#if !defined(__linux__) && !defined(__win)
        svraddr->sin6_len = sizeaddr;
#endif
        svraddr->sin6_family = AF_INET6;
        inet_pton(AF_INET6, ip.c_str(), &(svraddr->sin6_addr));
        svraddr->sin6_port = htons(port);

        addr = (struct sockaddr*)svraddr;
    } else {
        this->releaseSocket();
        this->setStateAndNotify(ISocketObserver::State::kClosed
            , "unsupport address family");
        return;
    }

    // 5秒超时
    this->connectTimeout(addr, sizeaddr, 5);
    free(addr);

    if (this->state == ISocketObserver::State::kConnected) {
        // 连接成功
        this->loopRecv();
    }
}

// 超时连接
void SocketClient::connectTimeout(struct sockaddr *addr
    , socklen_t len, int sec) {
    int flags = fcntl(*this->fd, F_GETFL, 0);

    // 设置为非阻塞模式
    fcntl(*this->fd, F_SETFL, flags | O_NONBLOCK);

    // 进行连接
    int rc = ::connect(*this->fd, addr, len);

    if (rc < 0 && errno != EINPROGRESS) {
        // 连接失败
        this->releaseSocket();
        this->setStateAndNotify(ISocketObserver::State::kClosed
            , strerror(errno));
        return;
    }

    // 5s超时
    struct timeval  timeout;
    timeout.tv_sec = sec;
    timeout.tv_usec = 0;

    fd_set set;
    FD_ZERO(&set);
    FD_SET(*this->fd, &set);

    // 使用select来实现超时
    rc = select(*(this->fd)+1, NULL, &set, NULL, &timeout);

    // 重新设置为阻塞模式
    fcntl(*this->fd, F_SETFL, flags);

    if (rc < 0 || (!FD_ISSET(*this->fd, &set))) {
        // 连接失败
        this->releaseSocket();
        this->setStateAndNotify(ISocketObserver::State::kClosed
            , "timeout");
        return;
    }

    // 获取错误码
    int err;
    len = sizeof(err);
    getsockopt(*this->fd, SOL_SOCKET, SO_ERROR, &err, &len);

    if (err != 0) {
        // 连接失败
        this->releaseSocket();
        this->setStateAndNotify(ISocketObserver::State::kClosed
            , strerror(err));
        return;
    }

    // 连接成功
    this->setStateAndNotify(ISocketObserver::State::kConnected, "");
}

// 在子线程中循环接收数据
void SocketClient::loopRecv() {
    int fd = *this->fd;

    auto *this_thread_exit = new bool;
    *this_thread_exit = false;
    this->thread_exit_flag = this_thread_exit;

    std::shared_ptr<char> buff(new char[RECV_BUFF_SIZE]);

    for (; !(*this_thread_exit); ) {
        int rc = ::recv(fd, buff.get(), RECV_BUFF_SIZE, 0);

        if (rc <= 0) {
            // 这几种错误码，认为连接是正常的，继续接收
            if ((errno == EAGAIN
                || errno == EWOULDBLOCK
                || errno == EINTR)) {
                continue;  // 继续接收数据
            }

            if (!(*this_thread_exit)) {
                this->Close();
            }
            break;
        }

        if (*this->fd != fd) {
            break;
        }

        this->observer->NotifyData((const char*)buff.get(), rc);
    }

    if (this->thread_exit_flag == this_thread_exit) {
        this->thread_exit_flag = NULL;
    }
    delete this_thread_exit;
}


// 设置状态并发送通知
void SocketClient::setStateAndNotify(ISocketObserver::State state
    , const std::string &ext = "") {
    this->state = state;
    this->observer->NotifyState(state, ext);
}

void SocketClient::Close() {
    if (this->thread_exit_flag) {
        *this->thread_exit_flag = true, this->thread_exit_flag = nullptr;
    }

    if (this->state != ISocketObserver::State::kClosed) {
        delete this->fd;
        this->fd = nullptr;

        this->setStateAndNotify(ISocketObserver::State::kClosed, "close");
    }
}
