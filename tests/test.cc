#include <iostream>
#include "../src/log.h"

int main(int argc, char** argv) {
    jujimeizuo::Logger::ptr logger(new jujimeizuo::Logger);
    logger -> addAppender(jujimeizuo::LogAppender::ptr(new jujimeizuo::StdoutLogAppender));

    jujimeizuo::LogEvent:: ptr event(new jujimeizuo::LogEvent(__FILE__, __LINE__, 0, 1, 2, time(0)));
    event -> getSS() << "hello jujimeizuo log";

    logger -> log(jujimeizuo::LogLevel::DEBUG, event);

    std::cout << "hello jujimeizuo log" << std::endl;

    return 0;
}