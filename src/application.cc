#include "application.h"

#include <unistd.h>
#include <signal.h>

#include "tcp_server.h"
#include "daemon.h"
#include "config.h"
#include "env.h"
#include "log.h"
#include "module.h"
#include "worker.h"

namespace jujimeizuo {

static jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_NAME("system");

static jujimeizuo::ConfigVar<std::string>::ptr g_server_work_path =
    jujimeizuo::Config::Lookup("server.work_path"
            ,std::string("/apps/work/jujimeizuo")
            , "server work path");

static jujimeizuo::ConfigVar<std::string>::ptr g_server_pid_file =
    jujimeizuo::Config::Lookup("server.pid_file"
            ,std::string("jujimeizuo.pid")
            , "server pid file");

static jujimeizuo::ConfigVar<std::string>::ptr g_service_discovery_zk =
    jujimeizuo::Config::Lookup("service_discovery.zk"
            ,std::string("")
            , "service discovery zookeeper");


static jujimeizuo::ConfigVar<std::vector<TcpServerConf> >::ptr g_servers_conf
    = jujimeizuo::Config::Lookup("servers", std::vector<TcpServerConf>(), "http server config");

Application* Application::s_instance = nullptr;

Application::Application() {
    s_instance = this;
}

bool Application::init(int argc, char** argv) {
    m_argc = argc;
    m_argv = argv;

    jujimeizuo::EnvMgr::GetInstance()->addHelp("s", "start with the terminal");
    jujimeizuo::EnvMgr::GetInstance()->addHelp("d", "run as daemon");
    jujimeizuo::EnvMgr::GetInstance()->addHelp("c", "conf path default: ./conf");
    jujimeizuo::EnvMgr::GetInstance()->addHelp("p", "print help");

    bool is_print_help = false;
    if(!jujimeizuo::EnvMgr::GetInstance()->init(argc, argv)) {
        is_print_help = true;
    }

    if(jujimeizuo::EnvMgr::GetInstance()->has("p")) {
        is_print_help = true;
    }

    std::string conf_path = jujimeizuo::EnvMgr::GetInstance()->getConfigPath();
    JUJIMEIZUO_LOG_INFO(g_logger) << "load conf path:" << conf_path;
    jujimeizuo::Config::LoadFromConfDir(conf_path);
    
    std::string static_path = jujimeizuo::EnvMgr::GetInstance()->getStaticPath();
    JUJIMEIZUO_LOG_INFO(g_logger) << "load static path:" << static_path;
    jujimeizuo::Config::LoadFromStaticDir(static_path);

    ModuleMgr::GetInstance()->init();
    std::vector<Module::ptr> modules;
    ModuleMgr::GetInstance()->listAll(modules);

    for(auto i : modules) {
        i->onBeforeArgsParse(argc, argv);
    }

    if(is_print_help) {
        jujimeizuo::EnvMgr::GetInstance()->printHelp();
        return false;
    }

    for(auto i : modules) {
        i->onAfterArgsParse(argc, argv);
    }
    modules.clear();

    int run_type = 0;
    if(jujimeizuo::EnvMgr::GetInstance()->has("s")) {
        run_type = 1;
    }
    if(jujimeizuo::EnvMgr::GetInstance()->has("d")) {
        run_type = 2;
    }

    if(run_type == 0) {
        jujimeizuo::EnvMgr::GetInstance()->printHelp();
        return false;
    }

    std::string pidfile = g_server_work_path->getValue()
                                + "/" + g_server_pid_file->getValue();
    if(jujimeizuo::FSUtil::IsRunningPidfile(pidfile)) {
        JUJIMEIZUO_LOG_ERROR(g_logger) << "server is running:" << pidfile;
        return false;
    }

    if(!jujimeizuo::FSUtil::Mkdir(g_server_work_path->getValue())) {
        JUJIMEIZUO_LOG_FATAL(g_logger) << "create work path [" << g_server_work_path->getValue()
            << " errno=" << errno << " errstr=" << strerror(errno);
        return false;
    }
    return true;
}

bool Application::run() {
    bool is_daemon = jujimeizuo::EnvMgr::GetInstance()->has("d");
    return start_daemon(m_argc, m_argv,
            std::bind(&Application::main, this, std::placeholders::_1,
                std::placeholders::_2), is_daemon);
}

int Application::main(int argc, char** argv) {
    signal(SIGPIPE, SIG_IGN);
    JUJIMEIZUO_LOG_INFO(g_logger) << "main";
    std::string conf_path = jujimeizuo::EnvMgr::GetInstance()->getConfigPath();
    jujimeizuo::Config::LoadFromConfDir(conf_path, true);
    {
        std::string pidfile = g_server_work_path->getValue()
                                    + "/" + g_server_pid_file->getValue();
        std::ofstream ofs(pidfile);
        if(!ofs) {
            JUJIMEIZUO_LOG_ERROR(g_logger) << "open pidfile " << pidfile << " failed";
            return false;
        }
        ofs << getpid();
    }

    m_mainIOManager.reset(new jujimeizuo::IOManager(1, true, "main"));
    m_mainIOManager->schedule(std::bind(&Application::run_fiber, this));
    m_mainIOManager->addTimer(2000, [](){
            //JUJIMEIZUO_LOG_INFO(g_logger) << "hello";
    }, true);
    m_mainIOManager->stop();
    return 0;
}



bool Application::getServer(const std::string& type, std::vector<TcpServer::ptr>& svrs) {
    auto it = m_servers.find(type);
    if(it == m_servers.end()) {
        return false;
    }
    svrs = it->second;
    return true;
}

void Application::listAllServer(std::map<std::string, std::vector<TcpServer::ptr> >& servers) {
    servers = m_servers;
}

}