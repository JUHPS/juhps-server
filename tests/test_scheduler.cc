#include "src/jujimeizuo.h"

static jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_ROOT();

void test_fiber() {
    static int s_count = 3;
    JUJIMEIZUO_LOG_INFO(g_logger) << "test in fiber s_count=" << s_count;

    // sleep(1);
    if(--s_count >= 0) {
        jujimeizuo::Scheduler::GetThis()->schedule(&test_fiber, jujimeizuo::GetThreadId());
    }
}

int main(int argc, char** argv) {
    JUJIMEIZUO_LOG_INFO(g_logger) << "main";
    jujimeizuo::Scheduler sc(3, false, "test");
    sc.start();
    sleep(2);
    JUJIMEIZUO_LOG_INFO(g_logger) << "schedule";
    sc.schedule(&test_fiber);
    sc.stop();
    JUJIMEIZUO_LOG_INFO(g_logger) << "over";
    return 0;
}