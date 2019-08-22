// Copyright @2Dou@github.com

#include "./cocos2d_socket.hpp"

Cocos2dSocket::Cocos2dSocket(cocos2d::LUA_FUNCTION event_cb)
    : SocketClient(this)
    , event_cb(event_cb) {

    // 注册update函数，用于回调lua
    cocos2d::Director::getInstance()->getScheduler()
        ->schedule(schedule_selector(Cocos2dSocket::update), this, 0, false);
}

Cocos2dSocket::~Cocos2dSocket() {
    // 注销update
    cocos2d::Director::getInstance()->getScheduler()
        ->unscheduleAllForTarget(this);
}

void Cocos2dSocket::NotifyState(ISocketObserver::State state
    , const std::string &msg) {
    if (state == ISocketObserver::State::kClosed) {
        this->mux_queue.lock();
        while (this->event_queue.size()) {
            auto p = this->event_queue.front();
            this->event_queue.pop();
            this->returnEvent(p);
        }
        this->mux_queue.unlock();
    }

    auto p = this->borrowEvent();
    auto e = p.get();

    e->type = EventType::kState;
    e->data.state = state;

    e->data.ext.clear();
    if (!msg.empty()) {
        e->data.ext = msg;
    }

    this->pushEvent(p);
}

void Cocos2dSocket::NotifyData(const char* buff, int length) {
    auto p = this->borrowEvent();
    auto e = p.get();
    e->type = EventType::kData;
    e->data.ext.clear();
    e->data.ext.append(buff, length);

    this->pushEvent(p);
}

std::shared_ptr<Cocos2dSocket::Event> Cocos2dSocket::borrowEvent() {
    std::shared_ptr<Event> p = nullptr;

    this->mux_pool.lock();
    if (this->event_pool.size() > 0) {
        p = this->event_pool.front();
        this->event_pool.pop();
    } else {
        p = std::shared_ptr<Event>(new Event);
    }
    this->mux_pool.unlock();

    return p;
}
void Cocos2dSocket::returnEvent(std::shared_ptr<Event> p) {
    this->mux_pool.lock();
    this->event_pool.push(p);
    this->mux_pool.unlock();
}

// 新增事件
void Cocos2dSocket::pushEvent(std::shared_ptr<Event> event) {
    this->mux_queue.lock();
    this->event_queue.push(event);
    this->mux_queue.unlock();
}


// 在update中回调
void Cocos2dSocket::update(float dt) {
    this->mux_queue.lock();
    if (this->event_queue.empty()) {
        // 没有事件需要发送
        this->mux_queue.unlock();
        return;
    }

    auto p = this->event_queue.front();
    auto e = p.get();
    this->event_queue.pop();
    this->mux_queue.unlock();

    cocos2d::LuaEngine* engine =
        (cocos2d::LuaEngine*)cocos2d::ScriptEngineManager::getInstance()
        -> getScriptEngine();
    cocos2d::LuaStack* stack = engine->getLuaStack();

    int argc = 2;
    stack->pushInt(e->type);
    if (e->type == EventType::kData) {
        stack->pushString(e->data.ext.c_str()
            , static_cast<int>(e->data.ext.size()));
    } else /*  (e.type == CSocketClient::EVENT_TYPE::kState)  */ {
        stack->pushInt(e->data.state);

        if (!e->data.ext.empty()) {
            argc++;
            stack->pushString(e->data.ext.c_str());
        }
    }

    this->returnEvent(p);

    // 执行lua回调函数
    stack->executeFunctionByHandler(this->event_cb, argc);
}
