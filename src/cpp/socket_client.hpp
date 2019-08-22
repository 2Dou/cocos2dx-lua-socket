// Copyright @2Dou@github.com


#ifndef _GITHUB_COM_2DOU_SOCKET_CLIENT_SRC_SOCKET_CLIENT_HPP_
#define _GITHUB_COM_2DOU_SOCKET_CLIENT_SRC_SOCKET_CLIENT_HPP_

#include <string>

#include "./socket.hpp"
#include "./socket_observer.hpp"

class SocketClient {
 public:
    explicit SocketClient(ISocketObserver *observer);
    virtual ~SocketClient();

    void ConnectIP(std::string ip, int port, int sock_type = SOCK_STREAM);
    void ConnectHost(std::string domain, int port, int sock_type = SOCK_STREAM);

    int Send(unsigned char *buf, int len);
    // int Recv(unsigned char *buf, int len);

    void Close();

 protected:
    Socket  *fd;

    bool    *thread_exit_flag;

    ISocketObserver::State   state;
    ISocketObserver  *observer;

    int createSocket(int af, int sock_type, int protocol);
    void releaseSocket();

    void connect(int af, std::string ip, int port, int sock_type);
    void connectTimeout(struct sockaddr *addr, socklen_t len, int sec);

    // 循环接收消息
    void loopRecv();

    // 设置状态并发送通知
    void setStateAndNotify(ISocketObserver::State state, const std::string &ext);

    // 根据域名获取网络类型、IP
    static int parseHost(const std::string &domain, int sock_type
        , std::string &ip);
    static int parseIP(std::string ip, int sock_type);
};

#endif  // _GITHUB_COM_2DOU_SOCKET_CLIENT_SRC_SOCKET_CLIENT_HPP_
