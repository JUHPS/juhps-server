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

    JUJIMEIZUO_LOG_INFO(g_logger) << "before run_in_fiber yield";
    jujimeizuo::Fiber::GetThis()->yield();
    JUJIMEIZUO_LOG_INFO(g_logger) << "after run_in_fiber yield";

    JUJIMEIZUO_LOG_INFO(g_logger) << "run_in_fiber end";
    // fiber结束之后会自动返回主协程运行
}

void test_fiber() {
    JUJIMEIZUO_LOG_INFO(g_logger) << "test_fiber begin";

    // 初始化线程主协程
    jujimeizuo::Fiber::GetThis();

    jujimeizuo::Fiber::ptr fiber(new jujimeizuo::Fiber(run_in_fiber, 0, false));
    JUJIMEIZUO_LOG_INFO(g_logger) << "use_count:" << fiber.use_count(); // 1

    JUJIMEIZUO_LOG_INFO(g_logger) << "before test_fiber resume";
    fiber->resume();
    JUJIMEIZUO_LOG_INFO(g_logger) << "after test_fiber resume";

    /** 
     * 关于fiber智能指针的引用计数为3的说明：
     * 一份在当前函数的fiber指针，一份在MainFunc的cur指针
     * 还有一份在在run_in_fiber的GetThis()结果的临时变量里
     */
    JUJIMEIZUO_LOG_INFO(g_logger) << "use_count:" << fiber.use_count(); // 3

    JUJIMEIZUO_LOG_INFO(g_logger) << "fiber status: " << fiber->getState(); // READY

    JUJIMEIZUO_LOG_INFO(g_logger) << "before test_fiber resume again";
    fiber->resume();
    JUJIMEIZUO_LOG_INFO(g_logger) << "after test_fiber resume again";

    JUJIMEIZUO_LOG_INFO(g_logger) << "use_count:" << fiber.use_count(); // 1
    JUJIMEIZUO_LOG_INFO(g_logger) << "fiber status: " << fiber->getState(); // TERM

    fiber->reset(run_in_fiber2); // 上一个协程结束之后，复用其栈空间再创建一个新协程
    fiber->resume();

    JUJIMEIZUO_LOG_INFO(g_logger) << "use_count:" << fiber.use_count(); // 1
    JUJIMEIZUO_LOG_INFO(g_logger) << "test_fiber end";
}

int main(int argc, char *argv[]) {
    jujimeizuo::EnvMgr::GetInstance()->init(argc, argv);
    jujimeizuo::Config::LoadFromConfDir(jujimeizuo::EnvMgr::GetInstance()->getConfigPath());

    jujimeizuo::SetThreadName("main_thread");
    JUJIMEIZUO_LOG_INFO(g_logger) << "main begin";

    std::vector<jujimeizuo::Thread::ptr> thrs;
    for (int i = 0; i < 2; i++) {
        thrs.push_back(jujimeizuo::Thread::ptr(
            new jujimeizuo::Thread(&test_fiber, "thread_" + std::to_string(i))));
    }

    for (auto i : thrs) {
        i->join();
    }

    JUJIMEIZUO_LOG_INFO(g_logger) << "main end";
    return 0;
}