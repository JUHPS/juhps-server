#include "src/jujimeizuo.h"
#include <unistd.h>

jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_ROOT();

void fun1() {
    JUJIMEIZUO_LOG_INFO(g_logger) << "name: " << jujimeizuo::Thread::GetName()
                                  << " this.name: " << jujimeizuo::Thread::GetThis() -> getName()
                                  << " id: " << jujimeizuo::GetThreadId()
                                  << " this.id: " << jujimeizuo::Thread::GetThis() -> getId();
    sleep(20);
}

void fun2() {

}

int main(int argc, char** argv) {
    JUJIMEIZUO_LOG_INFO(g_logger) << "thread test begin";
    std::vector<jujimeizuo::Thread::ptr> thrs;
    for (int i = 0; i < 5; i++) {
        jujimeizuo::Thread::ptr thr(new jujimeizuo::Thread(&fun1, "name_" + std::to_string(i)));
        thrs.push_back(thr);
    }

    for (int i = 0; i < 5; i++) {
        thrs[i] -> join();
    }
    JUJIMEIZUO_LOG_INFO(g_logger) << "thread test end";
    return 0;
}
