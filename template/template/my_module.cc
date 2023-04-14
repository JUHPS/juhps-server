#include "my_module.h"
#include "src/config.h"
#include "src/log.h"

namespace name_space {

jujimeizuo::IOManager::ptr worker;

static jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_ROOT();

MyModule::MyModule(int argc, char** argv) {
    jujimeizuo::EnvMgr::GetInstance()->init(argc, argv);
    jujimeizuo::Config::LoadFromConfDir(jujimeizuo::EnvMgr::GetInstance()->getConfigPath());
    jujimeizuo::Config::LoadFromStaticDir(jujimeizuo::EnvMgr::GetInstance()->getStaticPath());
    worker.reset(new jujimeizuo::IOManager(3, false, "default"));

    worker->schedule(run);
}

bool MyModule::run() {

}

}