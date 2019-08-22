-- Copyright @2Dou@github.com

if not sock then
    return
end

sock.EventType = {
    kState = 0,
    kData  = 1,
}

sock.State = {
    kInit       = 0,
    kConnecting = 1,
    kConnected  = 2,
    kClosed     = 3,
}

sock.SOCK_STREAM = 1
sock.SOCK_DGRAM  = 2
sock.SOCK_RAW    = 3

return sock
