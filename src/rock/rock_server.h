#ifndef __SRC_ROCK_SERVER_H__
#define __SRC_ROCK_SERVER_H__

#include "rock_stream.h"
#include "../tcp_server.h"

namespace jujimeizuo {

class RockServer : public TcpServer {
public:
    typedef std::shared_ptr<RockServer> ptr;
    RockServer(const std::string& type = "rock"
               ,jujimeizuo::IOManager* worker = jujimeizuo::IOManager::GetThis()
               ,jujimeizuo::IOManager* io_worker = jujimeizuo::IOManager::GetThis()
               ,jujimeizuo::IOManager* accept_worker = jujimeizuo::IOManager::GetThis());

protected:
    virtual void handleClient(Socket::ptr client) override;
};

}

#endif