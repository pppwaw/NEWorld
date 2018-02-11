#include <iostream>
#include <rpc/client.h>
#include <rpc/server.h>
#include "engine/maintenance/nwdebug.h"

#define FUNCTION_RENAME(oldname, alias)\
    template<typename... Args>\
    auto alias(Args&&... args)->decltype(fun(std::forward<Args>(args)...))\
    {return oldname(std::forward<Args>(args)...);}

class RPC{
public:
    void enableClient(const std::string& ip, uint16_t addr) {
        Assert(mClient == nullptr); // Do not initialize the client twice
        mClient = std::make_unique<rpc::client>(ip, addr);
        infostream << "Client connection initialized";
    }
    void enableServer(uint16_t addr) {
        Assert(mServer == nullptr); // Do not initialize the server twice
        mServer = std::make_unique<rpc::server>(addr);
        debugstream << "Server RPC initialized";
    }
    rpc::client& getClient() {
        Assert(mClient != nullptr);
        return *mClient;
    }
    rpc::server& getServer() {
        Assert(mServer != nullptr);
        return *mServer;
    }

    // TODO: avoid RPC in singleplayer mode.
    FUNCTION_RENAME(mClient->call, call);
    FUNCTION_RENAME(mClient->async_call, async_call);
    FUNCTION_RENAME(mServer->bind, bind);
private:
    std::unique_ptr<rpc::client> mClient;
    std::unique_ptr<rpc::server> mServer;
};
