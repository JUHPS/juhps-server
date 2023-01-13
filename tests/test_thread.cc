#include "src/jujimeizuo.h"
#include <unistd.h>

jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_ROOT();
int count = 0;
// jujimeizuo::RWMutex s_mutex;
jujimeizuo::Mutex s_mutex;

void fun1() {
    JUJIMEIZUO_LOG_INFO(g_logger) << "name: " << jujimeizuo::Thread::GetName()
                                  << " this.name: " << jujimeizuo::Thread::GetThis() -> getName()
                                  << " id: " << jujimeizuo::GetThreadId()
                                  << " this.id: " << jujimeizuo::Thread::GetThis() -> getId();

    for (int i = 0; i < 1000000; i++) {
        // jujimeizuo::RWMutex::WriteLock lock(s_mutex);
        jujimeizuo::Mutex::Lock lock(s_mutex);
        ++count;
    }
}

void fun2() {
    while (true) {
        JUJIMEIZUO_LOG_INFO(g_logger) << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    }
}

void fun3() {
    while (true) {
        JUJIMEIZUO_LOG_INFO(g_logger) << "==================================";
    }
}

int main(int argc, char** argv) {
    JUJIMEIZUO_LOG_INFO(g_logger) << "thread test begin";
    YAML::Node root = YAML::LoadFile("/root/WebServer/bin/conf/log2.yml");
    jujimeizuo::Config::LoadFromYaml(root);
    std::vector<jujimeizuo::Thread::ptr> thrs;
    for (int i = 0; i < 2; i++) {
        jujimeizuo::Thread::ptr thr(new jujimeizuo::Thread(&fun2, "name_" + std::to_string(i * 2)));
        jujimeizuo::Thread::ptr thr2(new jujimeizuo::Thread(&fun3, "name_" + std::to_string(i * 2 + 1)));
        thrs.push_back(thr);
        thrs.push_back(thr2);
    }

    for (size_t i = 0; i < thrs.size(); i++) {
        thrs[i] -> join();
    }
    JUJIMEIZUO_LOG_INFO(g_logger) << "thread test end";
    JUJIMEIZUO_LOG_INFO(g_logger) << "count=" << count;
    return 0;
}
