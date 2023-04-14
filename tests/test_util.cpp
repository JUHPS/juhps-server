#include "src/jujimeizuo.h"

jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_ROOT();

void test2() {
    std::cout << jujimeizuo::BacktraceToString() << std::endl;
}
void test1() {
    test2();
}

void test_backtrace() {
    test1();
}

int main() {
    JUJIMEIZUO_LOG_INFO(g_logger) << jujimeizuo::GetCurrentMS();
    JUJIMEIZUO_LOG_INFO(g_logger) << jujimeizuo::GetCurrentUS();
    JUJIMEIZUO_LOG_INFO(g_logger) << jujimeizuo::ToUpper("hello");
    JUJIMEIZUO_LOG_INFO(g_logger) << jujimeizuo::ToLower("HELLO");
    JUJIMEIZUO_LOG_INFO(g_logger) << jujimeizuo::Time2Str();
    JUJIMEIZUO_LOG_INFO(g_logger) << jujimeizuo::Str2Time("1970-01-01 00:00:00"); // -28800

    std::vector<std::string> files;
    jujimeizuo::FSUtil::ListAllFile(files, "./jujimeizuo", ".cpp");
    for (auto &i : files) {
        JUJIMEIZUO_LOG_INFO(g_logger) << i;
    }

    // todo, more...

    test_backtrace();

    JUJIMEIZUO_ASSERT2(false, "assert");
    return 0;
}