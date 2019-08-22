#Cocos2dx-Lua C++ socket 客户端

## 模型

阻塞，每个`SocketClient`直接创建一个线程阻塞调用`recv`。

相关消息通知存入队列，由`update`时发送到`Lua`。

## 使用方法

将 cpp 加入 lua_bindings 工程，使用`lua_cocos2dx_socket_manual.hpp`的`register_all_cocos2dx_socket_manual(L)`注册模块。

下面有一份简易的 Lua 代码：

```lua
function test:testSocket()
    local client
    client = sock.Cocos2dSocket:new(function(type, data, ext)
        if type == sock.EventType.kState then
            self:handleState(client, data, ext)
        else
            self:handleData(client, data)
        end
    end)
    client:retain()
    client:ConnectIP("127.0.0.1", 8080, sock.SOCK_STREAM)
end

function test:handleState(client, state, ext)
    if state == sock.State.kConnected then
        print("连接成功")
        client:Send("hallo")
    elseif state == sock.State.kClosed then
        print("断开连接")
        client:release()
    end
end

function test:handleData(client, data)
    print("received: " .. data)
end
```

## 注意

`Windows`平台还未调试

