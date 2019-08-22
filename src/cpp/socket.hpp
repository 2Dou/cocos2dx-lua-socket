// Copyright @2Dou@github.com


#ifndef _GITHUB_COM_2DOU_SOCKET_CLIENT_SRC_SOCKET_HPP_
#define _GITHUB_COM_2DOU_SOCKET_CLIENT_SRC_SOCKET_HPP_

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#   include <windows.h>
#   ifndef(__win)
#       define  __win
#   endif
#elif !defined(__unix)
#   define  __unix
#endif

#ifdef __unix
#   include <unistd.h>
#   include <sys/time.h>
#   include <sys/wait.h>
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <fcntl.h>
#   include <arpa/inet.h>
#   include <netdb.h>
#endif

#include <memory>

class SocketHandler {
 protected:
    explicit SocketHandler(int fd);

 public:
    ~SocketHandler();

    operator int() const;  // 重载类型转换

    friend class Socket;

 private:
    int fd;
};

class Socket {
 public:
    Socket(int domain, int sock_type, int protocol = 0);
    virtual ~Socket();

    Socket& operator=(const Socket &other);

    operator int() const;  // 重载类型转换

    bool operator==(const int &other) const;
    bool operator==(const Socket &other) const;

 private:
    std::shared_ptr<SocketHandler> fd;

#ifdef __win
    static bool inited;
    static void init_windows();
#endif
};

#endif  // _GITHUB_COM_2DOU_SOCKET_CLIENT_SRC_SOCKET_HPP_
