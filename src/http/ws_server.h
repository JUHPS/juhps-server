#ifndef __SRC_HTTP_WS_SERVER_H__
#define __SRC_HTTP_WS_SERVER_H__

#include "../tcp_server.h"
#include "ws_session.h"
#include "ws_servlet.h"

namespace jujimeizuo {
namespace http {

class WSServer : public TcpServer {
public:
    typedef std::shared_ptr<WSServer> ptr;

    WSServer(jujimeizuo::IOManager* worker = jujimeizuo::IOManager::GetThis()
             , jujimeizuo::IOManager* io_worker = jujimeizuo::IOManager::GetThis()
             , jujimeizuo::IOManager* accept_worker = jujimeizuo::IOManager::GetThis());

    WSServletDispatch::ptr getWSServletDispatch() const { return m_dispatch;}
    void setWSServletDispatch(WSServletDispatch::ptr v) { m_dispatch = v;}
protected:
    virtual void handleClient(Socket::ptr client) override;
protected:
    WSServletDispatch::ptr m_dispatch;
};

}
}

#endif