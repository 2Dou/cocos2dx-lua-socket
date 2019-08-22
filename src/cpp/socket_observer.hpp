// Copyright @2Dou@github.com


#ifndef _GITHUB_COM_2DOU_SOCKET_CLIENT_SRC_SOCKET_OBSERVER_HPP_
#define _GITHUB_COM_2DOU_SOCKET_CLIENT_SRC_SOCKET_OBSERVER_HPP_

class ISocketObserver {
 public:
    enum State {
        kInit = 0,
        kConnecting,
        kConnected,
        kClosed,
     };

    virtual ~ISocketObserver() {};

    virtual void NotifyState(State state, const std::string &msg) = 0;
    virtual void NotifyData(const char* buff, int length) = 0;
};

#endif  // _GITHUB_COM_2DOU_SOCKET_CLIENT_SRC_SOCKET_OBSERVER_HPP_
