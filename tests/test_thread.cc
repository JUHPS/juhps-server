#include "src/jujimeizuo.h"

jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_ROOT();

int count = 0;
jujimeizuo::Mutex s_mutex;

void func1(void *arg) {
    JUJIMEIZUO_LOG_INFO(g_logger) << "name:" << jujimeizuo::Thread::GetName()
        << " this.name:" << jujimeizuo::Thread::GetThis()->getName()
        << " thread name:" << jujimeizuo::GetThreadName()
        << " id:" << jujimeizuo::GetThreadId()
        << " this.id:" << jujimeizuo::Thread::GetThis()->getId();
    JUJIMEIZUO_LOG_INFO(g_logger) << "arg: " << *(int*)arg;
    for(int i = 0; i < 10000; i++) {
        jujimeizuo::Mutex::Lock lock(s_mutex);
        ++count;
    }
}

int main(int argc, char *argv[]) {
    jujimeizuo::EnvMgr::GetInstance()->init(argc, argv);
    jujimeizuo::Config::LoadFromConfDir(jujimeizuo::EnvMgr::GetInstance()->getConfigPath());

    std::vector<jujimeizuo::Thread::ptr> thrs;
    int arg = 123456;
    for(int i = 0; i < 3; i++) {
        // 带参数的线程用std::bind进行参数绑定
        jujimeizuo::Thread::ptr thr(new jujimeizuo::Thread(std::bind(func1, &arg), "thread_" + std::to_string(i)));
        thrs.push_back(thr);
    }

    for(int i = 0; i < 3; i++) {
        thrs[i]->join();
    }
    
    JUJIMEIZUO_LOG_INFO(g_logger) << "count = " << count;
    return 0;
}

