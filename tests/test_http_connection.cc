#include "src/jujimeizuo.h"
#include <iostream>

static jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_ROOT();

void test_pool() {
    jujimeizuo::http::HttpConnectionPool::ptr pool(new jujimeizuo::http::HttpConnectionPool(
        "www.midlane.top", "", 80, 10, 1000 * 30, 5));

    jujimeizuo::IOManager::GetThis()->addTimer(
        1000, [pool]() {
            auto r = pool->doGet("/", 300);
            std::cout << r->toString() << std::endl;
        },
        true);
}

void run() {
    jujimeizuo::Address::ptr addr = jujimeizuo::Address::LookupAnyIPAddress("www.midlane.top:80");
    if (!addr) {
        JUJIMEIZUO_LOG_INFO(g_logger) << "get addr error";
        return;
    }

    jujimeizuo::Socket::ptr sock = jujimeizuo::Socket::CreateTCP(addr);
    bool rt                 = sock->connect(addr);
    if (!rt) {
        JUJIMEIZUO_LOG_INFO(g_logger) << "connect " << *addr << " failed";
        return;
    }

    jujimeizuo::http::HttpConnection::ptr conn(new jujimeizuo::http::HttpConnection(sock));
    jujimeizuo::http::HttpRequest::ptr req(new jujimeizuo::http::HttpRequest);
    req->setPath("/");
    req->setHeader("host", "www.midlane.top");
    // 小bug，如果设置了keep-alive，那么要在使用前先调用一次init
    req->setHeader("connection", "keep-alive");
    req->init();
    std::cout << "req:" << std::endl
              << *req << std::endl;

    conn->sendRequest(req);
    auto rsp = conn->recvResponse();

    if (!rsp) {
        JUJIMEIZUO_LOG_INFO(g_logger) << "recv response error";
        return;
    }
    std::cout << "rsp:" << std::endl
              << *rsp << std::endl;

    std::cout << "=========================" << std::endl;

    auto r = jujimeizuo::http::HttpConnection::DoGet("http://www.midlane.top/wiki/", 300);
    std::cout << "result=" << r->result
              << " error=" << r->error
              << " rsp=" << (r->response ? r->response->toString() : "")
              << std::endl;

    std::cout << "=========================" << std::endl;
    test_pool();
}

int main(int argc, char **argv) {
    jujimeizuo::IOManager iom(2);
    iom.schedule(run);
    return 0;
}
