#include "src/jujimeizuo.h"

static jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_ROOT();

jujimeizuo::Timer::ptr timer;
int server_main(int argc, char **argv) {
    JUJIMEIZUO_LOG_INFO(g_logger) << jujimeizuo::ProcessInfoMgr::GetInstance()->toString();
    jujimeizuo::IOManager iom(1);
    timer = iom.addTimer(
        1000, []() {
            JUJIMEIZUO_LOG_INFO(g_logger) << "onTimer";
            static int count = 0;
            if (++count > 10) {
                exit(1);
            }
        },
        true);
    return 0;
}

int main(int argc, char **argv) {
    return jujimeizuo::start_daemon(argc, argv, server_main, argc != 1);
}
