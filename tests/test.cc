#include <iostream>
#include <thread>
#include "../src/log.h"
#include "../src/util.h"

int main(int argc, char** argv) {
    jujimeizuo::Logger::ptr logger(new jujimeizuo::Logger);
    logger -> addAppender(jujimeizuo::LogAppender::ptr(new jujimeizuo::StdoutLogAppender));

    jujimeizuo::FileLogAppender::ptr file_appender(new jujimeizuo::FileLogAppender("./log.txt"));

    jujimeizuo::LogFormatter::ptr fmt(new jujimeizuo::LogFormatter("%d%T%p%T%m%n"));
    file_appender -> setFormatter(fmt);
    
    file_appender -> setLevel(jujimeizuo::LogLevel::ERROR);

    logger -> addAppender(file_appender);
    // jujimeizuo::LogEvent:: ptr event(new jujimeizuo::LogEvent(__FILE__, __LINE__, 0, jujimeizuo::GetThreadId(), jujimeizuo::GetFiberId(), time(0)));
    // event -> getSS() << "hello jujimeizuo log";
    
    // logger -> log(jujimeizuo::LogLevel::DEBUG, event);
    std::cout << "hello jujimeizuo log" << std::endl;

    JUJIMEIZUO_LOG_INFO(logger) << "test macro";
    JUJIMEIZUO_LOG_ERROR(logger) << "test macro error";

    JUJIMEIZUO_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");
    
    auto l = jujimeizuo::LoggerMgr::GetInstance() -> getLogger("xx");
    JUJIMEIZUO_LOG_INFO(l) << "xxx";

    return 0;
}