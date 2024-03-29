#include "http_server.h"
#include "../log.h"
#include "../env.h"
//#include "servlets/config_servlet.h"
//#include "servlets/status_servlet.h"

namespace jujimeizuo {
namespace http {

static jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_NAME("system");

HttpServer::HttpServer(bool keepalive
               ,jujimeizuo::IOManager* worker
               ,jujimeizuo::IOManager* io_worker
               ,jujimeizuo::IOManager* accept_worker)
    :TcpServer(io_worker, accept_worker)
    ,m_isKeepalive(keepalive) {
    m_dispatch.reset(new ServletDispatch);

    m_type = "http";
    //m_dispatch->addServlet("/_/status", Servlet::ptr(new StatusServlet));
    //m_dispatch->addServlet("/_/config", Servlet::ptr(new ConfigServlet));
}

void HttpServer::setName(const std::string& v) {
    TcpServer::setName(v);
    m_dispatch->setDefault(std::make_shared<NotFoundServlet>(v));
}

void HttpServer::handleClient(Socket::ptr client) {
    JUJIMEIZUO_LOG_DEBUG(g_logger) << "handleClient " << *client;
    HttpSession::ptr session(new HttpSession(client));
    do {
        auto req = session->recvRequest();
        if(!req) {
            JUJIMEIZUO_LOG_DEBUG(g_logger) << "recv http request fail, errno="
                << errno << " errstr=" << strerror(errno)
                << " cliet:" << *client << " keep_alive=" << m_isKeepalive;
            break;
        }

        HttpResponse::ptr rsp(new HttpResponse(req->getVersion()
                            ,req->isClose() || !m_isKeepalive));
        rsp->setHeader("Server", getName());
        m_dispatch->handle(req, rsp, session);
        session->sendResponse(rsp);

        if(!m_isKeepalive || req->isClose()) {
            break;
        }
    } while(true);
    session->close();
}

ServerManager::ServerManager() {
    m_server = jujimeizuo::http::HttpServer::ptr(new jujimeizuo::http::HttpServer(true));
    std::string port = jujimeizuo::EnvMgr::GetInstance()->getPort();
    std::string address = std::string("0.0.0.0:") + (port.empty() ? "8080" : port);
    m_addr = jujimeizuo::Address::LookupAnyIPAddress(address);
    while (!m_server->bind(m_addr)) {
        sleep(2);
    }
    m_sd = m_server->getServletDispatch();
}

HttpServer::ptr& ServerManager::getServer() {
    return m_server;
}

Address::ptr& ServerManager::getAddress() {
    return m_addr;
}

ServletDispatch::ptr& ServerManager::getSd() {
    return m_sd;
}

}
}
