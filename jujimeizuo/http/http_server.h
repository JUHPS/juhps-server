/**
 * @file http_server.h
 * @brief HTTP服务器封装
 * @author jujimeizuo.yin
 * @email 564628276@qq.com
 * @date 2019-06-09
 * @copyright Copyright (c) 2019年 jujimeizuo.yin All rights reserved (www.jujimeizuo.top)
 */

#ifndef __JUJIMEIZUO_HTTP_HTTP_SERVER_H__
#define __JUJIMEIZUO_HTTP_HTTP_SERVER_H__

#include "../tcp_server.h"
#include "http_session.h"
#include "servlet.h"

namespace jujimeizuo {
namespace http {

/**
 * @brief HTTP服务器类
 */
class HttpServer : public TcpServer {
public:
    /// 智能指针类型
    typedef std::shared_ptr<HttpServer> ptr;

    /**
     * @brief 构造函数
     * @param[in] keepalive 是否长连接
     * @param[in] worker 工作调度器
     * @param[in] accept_worker 接收连接调度器
     */
    HttpServer(bool keepalive = false
               ,jujimeizuo::IOManager* worker = jujimeizuo::IOManager::GetThis()
               ,jujimeizuo::IOManager* io_worker = jujimeizuo::IOManager::GetThis()
               ,jujimeizuo::IOManager* accept_worker = jujimeizuo::IOManager::GetThis());

    /**
     * @brief 获取ServletDispatch
     */
    ServletDispatch::ptr getServletDispatch() const { return m_dispatch;}

    /**
     * @brief 设置ServletDispatch
     */
    void setServletDispatch(ServletDispatch::ptr v) { m_dispatch = v;}

    virtual void setName(const std::string& v) override;
protected:
    virtual void handleClient(Socket::ptr client) override;
private:
    /// 是否支持长连接
    bool m_isKeepalive;
    /// Servlet分发器
    ServletDispatch::ptr m_dispatch;
};

}
}

#endif
