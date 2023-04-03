#ifndef __SRC_HTTP_WS_SERVLET_H__
#define __SRC_HTTP_WS_SERVLET_H__

#include "ws_session.h"
#include "../thread.h"
#include "servlet.h"

namespace jujimeizuo {
namespace http {

class WSServlet : public Servlet {
public:
    typedef std::shared_ptr<WSServlet> ptr;
    WSServlet(const std::string& name)
        :Servlet(name) {
    }
    virtual ~WSServlet() {}

    virtual int32_t handle(jujimeizuo::http::HttpRequest::ptr request
                   , jujimeizuo::http::HttpResponse::ptr response
                   , jujimeizuo::http::HttpSession::ptr session) override {
        return 0;
    }

    virtual int32_t onConnect(jujimeizuo::http::HttpRequest::ptr header
                              ,jujimeizuo::http::WSSession::ptr session) = 0;
    virtual int32_t onClose(jujimeizuo::http::HttpRequest::ptr header
                             ,jujimeizuo::http::WSSession::ptr session) = 0;
    virtual int32_t handle(jujimeizuo::http::HttpRequest::ptr header
                           ,jujimeizuo::http::WSFrameMessage::ptr msg
                           ,jujimeizuo::http::WSSession::ptr session) = 0;
    const std::string& getName() const { return m_name;}
protected:
    std::string m_name;
};

class FunctionWSServlet : public WSServlet {
public:
    typedef std::shared_ptr<FunctionWSServlet> ptr;
    typedef std::function<int32_t (jujimeizuo::http::HttpRequest::ptr header
                              ,jujimeizuo::http::WSSession::ptr session)> on_connect_cb;
    typedef std::function<int32_t (jujimeizuo::http::HttpRequest::ptr header
                             ,jujimeizuo::http::WSSession::ptr session)> on_close_cb; 
    typedef std::function<int32_t (jujimeizuo::http::HttpRequest::ptr header
                           ,jujimeizuo::http::WSFrameMessage::ptr msg
                           ,jujimeizuo::http::WSSession::ptr session)> callback;

    FunctionWSServlet(callback cb
                      ,on_connect_cb connect_cb = nullptr
                      ,on_close_cb close_cb = nullptr);

    virtual int32_t onConnect(jujimeizuo::http::HttpRequest::ptr header
                              ,jujimeizuo::http::WSSession::ptr session) override;
    virtual int32_t onClose(jujimeizuo::http::HttpRequest::ptr header
                             ,jujimeizuo::http::WSSession::ptr session) override;
    virtual int32_t handle(jujimeizuo::http::HttpRequest::ptr header
                           ,jujimeizuo::http::WSFrameMessage::ptr msg
                           ,jujimeizuo::http::WSSession::ptr session) override;
protected:
    callback m_callback;
    on_connect_cb m_onConnect;
    on_close_cb m_onClose;
};

class WSServletDispatch : public ServletDispatch {
public:
    typedef std::shared_ptr<WSServletDispatch> ptr;
    typedef RWMutex RWMutexType;

    WSServletDispatch();
    void addServlet(const std::string& uri
                    ,FunctionWSServlet::callback cb
                    ,FunctionWSServlet::on_connect_cb connect_cb = nullptr
                    ,FunctionWSServlet::on_close_cb close_cb = nullptr);
    void addGlobServlet(const std::string& uri
                    ,FunctionWSServlet::callback cb
                    ,FunctionWSServlet::on_connect_cb connect_cb = nullptr
                    ,FunctionWSServlet::on_close_cb close_cb = nullptr);
    WSServlet::ptr getWSServlet(const std::string& uri);
};

}
}

#endif