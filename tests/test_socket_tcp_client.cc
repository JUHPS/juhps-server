#include "src/jujimeizuo.h"

static jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_ROOT();

void test_tcp_client() {
    int ret;

    auto socket = jujimeizuo::Socket::CreateTCPSocket();
    JUJIMEIZUO_ASSERT(socket);

    auto addr = jujimeizuo::Address::LookupAnyIPAddress("0.0.0.0:12345");
    JUJIMEIZUO_ASSERT(addr);

    ret = socket->connect(addr);
    JUJIMEIZUO_ASSERT(ret);

    JUJIMEIZUO_LOG_INFO(g_logger) << "connect success, peer address: " << socket->getRemoteAddress()->toString();

    std::string buffer;
    buffer.resize(1024);
    socket->recv(&buffer[0], buffer.size());
    JUJIMEIZUO_LOG_INFO(g_logger) << "recv: " << buffer;
    socket->close();

    return;
}

int main(int argc, char *argv[]) {
    jujimeizuo::EnvMgr::GetInstance()->init(argc, argv);
    jujimeizuo::Config::LoadFromConfDir(jujimeizuo::EnvMgr::GetInstance()->getConfigPath());

    jujimeizuo::IOManager iom;
    iom.schedule(&test_tcp_client);

    return 0;
}