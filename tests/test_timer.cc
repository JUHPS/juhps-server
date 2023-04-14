#include "src/jujimeizuo.h"

static jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_ROOT();

static int timeout = 1000;
static jujimeizuo::Timer::ptr s_timer;

void timer_callback() {
    JUJIMEIZUO_LOG_INFO(g_logger) << "timer callback, timeout = " << timeout;
    timeout += 1000;
    if(timeout < 5000) {
        s_timer->reset(timeout, true);
    } else {
        s_timer->cancel();
    }
}

void test_timer() {
    jujimeizuo::IOManager iom;

    // 循环定时器
    s_timer = iom.addTimer(1000, timer_callback, true);
    
    // 单次定时器
    iom.addTimer(500, []{
        JUJIMEIZUO_LOG_INFO(g_logger) << "500ms timeout";
    });
    iom.addTimer(5000, []{
        JUJIMEIZUO_LOG_INFO(g_logger) << "5000ms timeout";
    });
}

int main(int argc, char *argv[]) {
    jujimeizuo::EnvMgr::GetInstance()->init(argc, argv);
    jujimeizuo::Config::LoadFromConfDir(jujimeizuo::EnvMgr::GetInstance()->getConfigPath());

    test_timer();

    JUJIMEIZUO_LOG_INFO(g_logger) << "end";

    return 0;
}