#include "src/jujimeizuo.h"

static jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_ROOT();

/**
 * @brief 自定义TcpServer类，重载handleClient方法
 */
class MyTcpServer : public jujimeizuo::TcpServer {
protected:
    virtual void handleClient(jujimeizuo::Socket::ptr client) override;
};

void MyTcpServer::handleClient(jujimeizuo::Socket::ptr client) {
    JUJIMEIZUO_LOG_INFO(g_logger) << "new client: " << client->toString();
    static std::string buf;
    buf.resize(4096);
    client->recv(&buf[0], buf.length()); // 这里有读超时，由tcp_server.read_timeout配置项进行配置，默认120秒
    JUJIMEIZUO_LOG_INFO(g_logger) << "recv: " << buf;
    client->close();
}

void run() {
    jujimeizuo::TcpServer::ptr server(new MyTcpServer); // 内部依赖shared_from_this()，所以必须以智能指针形式创建对象
    auto addr = jujimeizuo::Address::LookupAny("0.0.0.0:12345");
    JUJIMEIZUO_ASSERT(addr);
    std::vector<jujimeizuo::Address::ptr> addrs;
    addrs.push_back(addr);

    std::vector<jujimeizuo::Address::ptr> fails;
    while(!server->bind(addrs, fails)) {
        sleep(2);
    }
    
    JUJIMEIZUO_LOG_INFO(g_logger) << "bind success, " << server->toString();

    server->start();
}

int main(int argc, char *argv[]) {
    jujimeizuo::EnvMgr::GetInstance()->init(argc, argv);
    jujimeizuo::Config::LoadFromConfDir(jujimeizuo::EnvMgr::GetInstance()->getConfigPath());

    jujimeizuo::IOManager iom(2);
    iom.schedule(&run);

    return 0;
}