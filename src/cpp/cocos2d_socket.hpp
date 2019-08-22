// Copyright @2Dou@github.com


#ifndef _GITHUB_COM_2DOU_SOCKET_CLIENT_SRC_COCOS2D_SOCKET_HPP_
#define _GITHUB_COM_2DOU_SOCKET_CLIENT_SRC_COCOS2D_SOCKET_HPP_

#include <mutex>
#include <queue>
#include <string>
#include <memory>

#include "cocos2d.h"
#include "CCLuaEngine.h"

#include "./socket_observer.hpp"
#include "./socket_client.hpp"

class Cocos2dSocket: public cocos2d::Ref
    , public SocketClient, public ISocketObserver {
 public:
    // 事件类型
    enum EventType {
        kState = 0,     // 状态改变
        kData,          // 接收数据
    };

    /* 构造函数需要传入一个lua function
    * eg:
    * function(event, data, ext)
    *     if event == sock.EventType.kState then
    *         -- 状态改变，此时data表示新状态
    *         -- kInit、kConnecting这两个状态不会回调
    *         -- kConnected、kClosed分别表示连接成功、连接失败/连接丢失
    *         self:handleState(data, ext)
    *     elseif event == sock.EventType.kData then
    *         -- 收到数据，此时data表示数据
    *     end
    * end
    */
    explicit Cocos2dSocket(cocos2d::LUA_FUNCTION event_cb);
    ~Cocos2dSocket();

    void update(float delta);  // update时调用回调发送队列数据

 protected:
    void NotifyState(ISocketObserver::State state, const std::string &msg);
    void NotifyData(const char* buff, int length);

 private:
    // 事件数据
    typedef struct _EventData {
        ISocketObserver::State  state;  // 状态
        std::string             ext;
    }EventData;

    typedef struct _EVENT {
        EventType type;
        EventData data;
    }Event;

    // 事件回调函数
    cocos2d::LUA_FUNCTION event_cb;
    std::mutex      mux_pool;
    std::mutex      mux_queue;
    std::queue<std::shared_ptr<Event>>  event_pool;
    std::queue<std::shared_ptr<Event>>  event_queue;  // 事件队列

    std::shared_ptr<Event> borrowEvent();
    void returnEvent(std::shared_ptr<Event>);

    void pushEvent(std::shared_ptr<Event> event);
};


#endif  // _GITHUB_COM_2DOU_SOCKET_CLIENT_SRC_COCOS2D_SOCKET_HPP_
