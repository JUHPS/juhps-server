#include "my_module.h"
#include "config.h"
#include "log.h"

namespace name_space {

static jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_ROOT();

MyModule::MyModule()
    :jujimeizuo::Module("project_name", "1.0", "") {
}

bool MyModule::onLoad() {
    JUJIMEIZUO_LOG_INFO(g_logger) << "onLoad";
    return true;
}

bool MyModule::onUnload() {
    JUJIMEIZUO_LOG_INFO(g_logger) << "onUnload";
    return true;
}

bool MyModule::onServerReady() {
    JUJIMEIZUO_LOG_INFO(g_logger) << "onServerReady";
    return true;
}

bool MyModule::onServerUp() {
    JUJIMEIZUO_LOG_INFO(g_logger) << "onServerUp";
    return true;
}

}

extern "C" {

jujimeizuo::Module* CreateModule() {
    jujimeizuo::Module* module = new name_space::MyModule;
    JUJIMEIZUO_LOG_INFO(name_space::g_logger) << "CreateModule " << module;
    return module;
}

void DestoryModule(jujimeizuo::Module* module) {
    JUJIMEIZUO_LOG_INFO(name_space::g_logger) << "CreateModule " << module;
    delete module;
}

}