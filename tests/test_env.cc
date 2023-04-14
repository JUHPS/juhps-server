#include "src/jujimeizuo.h"

jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_ROOT();

jujimeizuo::Env *g_env = jujimeizuo::EnvMgr::GetInstance();

int main(int argc, char *argv[]) {
    g_env->addHelp("h", "print this help message");

    bool is_print_help = false;
    if(!g_env->init(argc, argv)) {
        is_print_help = true;
    }
    if(g_env->has("h")) {
        is_print_help = true;
    }

    if(is_print_help) {
        g_env->printHelp();
        return false;
    }

    JUJIMEIZUO_LOG_INFO(g_logger)<< "exe: " << g_env->getExe();
    JUJIMEIZUO_LOG_INFO(g_logger) <<"cwd: " << g_env->getCwd();
    JUJIMEIZUO_LOG_INFO(g_logger) << "absoluth path of test: " << g_env->getAbsolutePath("test");
    JUJIMEIZUO_LOG_INFO(g_logger) << "conf path:" << g_env->getConfigPath();

    g_env->add("key1", "value1");
    JUJIMEIZUO_LOG_INFO(g_logger) << "key1: " << g_env->get("key1");

    g_env->setEnv("key2", "value2");
    JUJIMEIZUO_LOG_INFO(g_logger) << "key2: " << g_env->getEnv("key2");

    JUJIMEIZUO_LOG_INFO(g_logger) << g_env->getEnv("PATH");

    return 0;
}