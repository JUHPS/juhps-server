#include "rock_server.h"
#include "../log.h"
#include "../module.h"

namespace jujimeizuo {

static jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_NAME("system");

RockServer::RockServer(const std::string& type
                       ,jujimeizuo::IOManager* worker
                       ,jujimeizuo::IOManager* io_worker
                       ,jujimeizuo::IOManager* accept_worker)
    :TcpServer(worker, io_worker, accept_worker) {
    m_type = type;
}

void RockServer::handleClient(Socket::ptr client) {
    JUJIMEIZUO_LOG_DEBUG(g_logger) << "handleClient " << *client;
    jujimeizuo::RockSession::ptr session(new jujimeizuo::RockSession(client));
    session->setWorker(m_worker);
    ModuleMgr::GetInstance()->foreach(Module::ROCK,
            [session](Module::ptr m) {
        m->onConnect(session);
    });
    session->setDisconnectCb(
        [](AsyncSocketStream::ptr stream) {
             ModuleMgr::GetInstance()->foreach(Module::ROCK,
                    [stream](Module::ptr m) {
                m->onDisconnect(stream);
            });
        }
    );
    session->setRequestHandler(
        [](jujimeizuo::RockRequest::ptr req
           ,jujimeizuo::RockResponse::ptr rsp
           ,jujimeizuo::RockStream::ptr conn)->bool {
            //JUJIMEIZUO_LOG_INFO(g_logger) << "handleReq " << req->toString()
            //                         << " body=" << req->getBody();
            bool rt = false;
            ModuleMgr::GetInstance()->foreach(Module::ROCK,
                    [&rt, req, rsp, conn](Module::ptr m) {
                if(rt) {
                    return;
                }
                rt = m->handleRequest(req, rsp, conn);
            });
            return rt;
        }
    ); 
    session->setNotifyHandler(
        [](jujimeizuo::RockNotify::ptr nty
           ,jujimeizuo::RockStream::ptr conn)->bool {
            JUJIMEIZUO_LOG_INFO(g_logger) << "handleNty " << nty->toString()
                                     << " body=" << nty->getBody();
            bool rt = false;
            ModuleMgr::GetInstance()->foreach(Module::ROCK,
                    [&rt, nty, conn](Module::ptr m) {
                if(rt) {
                    return;
                }
                rt = m->handleNotify(nty, conn);
            });
            return rt;
        }
    );
    session->start();
}

}