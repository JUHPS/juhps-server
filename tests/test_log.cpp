#include "src/jujimeizuo.h"

#include <unistd.h>

jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_ROOT(); // 默认INFO级别

int main(int argc, char *argv[]) {
    jujimeizuo::EnvMgr::GetInstance()->init(argc, argv);
    jujimeizuo::Config::LoadFromConfDir(jujimeizuo::EnvMgr::GetInstance()->getConfigPath());

    JUJIMEIZUO_LOG_FATAL(g_logger) << "fatal msg";
    JUJIMEIZUO_LOG_ERROR(g_logger) << "err msg";
    JUJIMEIZUO_LOG_INFO(g_logger) << "info msg";
    JUJIMEIZUO_LOG_DEBUG(g_logger) << "debug msg";

    JUJIMEIZUO_LOG_FMT_FATAL(g_logger, "fatal %s:%d", __FILE__, __LINE__);
    JUJIMEIZUO_LOG_FMT_ERROR(g_logger, "err %s:%d", __FILE__, __LINE__);
    JUJIMEIZUO_LOG_FMT_INFO(g_logger, "info %s:%d", __FILE__, __LINE__);
    JUJIMEIZUO_LOG_FMT_DEBUG(g_logger, "debug %s:%d", __FILE__, __LINE__);
   
    sleep(1);
    jujimeizuo::SetThreadName("brand_new_thread");

    g_logger->setLevel(jujimeizuo::LogLevel::WARN);
    JUJIMEIZUO_LOG_FATAL(g_logger) << "fatal msg";
    JUJIMEIZUO_LOG_ERROR(g_logger) << "err msg";
    JUJIMEIZUO_LOG_INFO(g_logger) << "info msg"; // 不打印
    JUJIMEIZUO_LOG_DEBUG(g_logger) << "debug msg"; // 不打印


    jujimeizuo::FileLogAppender::ptr fileAppender(new jujimeizuo::FileLogAppender("./log.txt"));
    g_logger->addAppender(fileAppender);
    JUJIMEIZUO_LOG_FATAL(g_logger) << "fatal msg";
    JUJIMEIZUO_LOG_ERROR(g_logger) << "err msg";
    JUJIMEIZUO_LOG_INFO(g_logger) << "info msg"; // 不打印
    JUJIMEIZUO_LOG_DEBUG(g_logger) << "debug msg"; // 不打印

    jujimeizuo::Logger::ptr test_logger = JUJIMEIZUO_LOG_NAME("test_logger");
    jujimeizuo::StdoutLogAppender::ptr appender(new jujimeizuo::StdoutLogAppender);
    jujimeizuo::LogFormatter::ptr formatter(new jujimeizuo::LogFormatter("%d:%rms%T%p%T%c%T%f:%l %m%n")); // 时间：启动毫秒数 级别 日志名称 文件名：行号 消息 换行
    appender->setFormatter(formatter);
    test_logger->addAppender(appender);
    test_logger->setLevel(jujimeizuo::LogLevel::WARN);

    JUJIMEIZUO_LOG_ERROR(test_logger) << "err msg";
    JUJIMEIZUO_LOG_INFO(test_logger) << "info msg"; // 不打印

    // 输出全部日志器的配置
    g_logger->setLevel(jujimeizuo::LogLevel::INFO);
    JUJIMEIZUO_LOG_INFO(g_logger) << "logger config:" << jujimeizuo::LoggerMgr::GetInstance()->toYamlString();

    return 0;
}