#include "src/jujimeizuo.h"

static jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_ROOT();

void test_tcp_server() {
    int ret;

    auto addr = jujimeizuo::Address::LookupAnyIPAddress("0.0.0.0:12345");
    JUJIMEIZUO_ASSERT(addr);

    auto socket = jujimeizuo::Socket::CreateTCPSocket();
    JUJIMEIZUO_ASSERT(socket);

    ret = socket->bind(addr);
    JUJIMEIZUO_ASSERT(ret);
    
    JUJIMEIZUO_LOG_INFO(g_logger) << "bind success";

    ret = socket->listen();
    JUJIMEIZUO_ASSERT(ret);

    JUJIMEIZUO_LOG_INFO(g_logger) << socket->toString() ;
    JUJIMEIZUO_LOG_INFO(g_logger) << "listening...";

    while(1) {
        auto client = socket->accept();
        JUJIMEIZUO_ASSERT(client);
        JUJIMEIZUO_LOG_INFO(g_logger) << "new client: " << client->toString();
        client->send("hello world", strlen("hello world"));
        client->close();
    }
}

int main(int argc, char *argv[]) {
    jujimeizuo::EnvMgr::GetInstance()->init(argc, argv);
    jujimeizuo::Config::LoadFromConfDir(jujimeizuo::EnvMgr::GetInstance()->getConfigPath());

    jujimeizuo::IOManager iom(2);
    iom.schedule(&test_tcp_server);

    return 0;
}