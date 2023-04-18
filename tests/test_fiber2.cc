#include "src/fiber.h"
#include "src/jujimeizuo.h"
#include <string>
#include <vector>

jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_ROOT();

void run_in_fiber2() {
    JUJIMEIZUO_LOG_INFO(g_logger) << "run_in_fiber2 begin";
    JUJIMEIZUO_LOG_INFO(g_logger) << "run_in_fiber2 end";
}

void run_in_fiber() {
    JUJIMEIZUO_LOG_INFO(g_logger) << "run_in_fiber begin";

    /**
     * 非对称协程，子协程不能创建并运行新的子协程，下面的操作是有问题的，
     * 子协程再创建子协程，原来的主协程就跑飞了
     */
    jujimeizuo::Fiber::ptr fiber(new jujimeizuo::Fiber(run_in_fiber2, 0, false));
    fiber->resume();

    JUJIMEIZUO_LOG_INFO(g_logger) << "run_in_fiber end";
}

int main(int argc, char *argv[]) {
    jujimeizuo::EnvMgr::GetInstance()->init(argc, argv);
    jujimeizuo::Config::LoadFromConfDir(jujimeizuo::EnvMgr::GetInstance()->getConfigPath());

    JUJIMEIZUO_LOG_INFO(g_logger) << "main begin";

    jujimeizuo::Fiber::GetThis();

    jujimeizuo::Fiber::ptr fiber(new jujimeizuo::Fiber(run_in_fiber, 0, false));
    fiber->resume();

    JUJIMEIZUO_LOG_INFO(g_logger) << "main end";
    return 0;
}