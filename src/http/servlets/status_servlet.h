#ifndef __SRC_HTTP_SERVLETS_STATUS_SERVLET_H__
#define __SRC_HTTP_SERVLETS_STATUS_SERVLET_H__

#include "../http/servlet.h"

namespace jujimeizuo {
namespace http {

class StatusServlet : public Servlet {
public:
    StatusServlet();
    virtual int32_t handle(jujimeizuo::http::HttpRequest::ptr request
                   , jujimeizuo::http::HttpResponse::ptr response
                   , jujimeizuo::http::HttpSession::ptr session) override;
};

}
}

#endif