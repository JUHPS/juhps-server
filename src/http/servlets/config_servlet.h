#ifndef __SRC_HTTP_SERVLETS_CONFIG_SERVLET_H__
#define __SRC_HTTP_SERVLETS_CONFIG_SERVLET_H__

#include "../servlet.h"

namespace jujimeizuo {
namespace http {

class ConfigServlet : public Servlet {
public:
    ConfigServlet();
    virtual int32_t handle(jujimeizuo::http::HttpRequest::ptr request
                   , jujimeizuo::http::HttpResponse::ptr response
                   , jujimeizuo::http::HttpSession::ptr session) override;
};

}
}

#endif