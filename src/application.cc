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
            //JUJIMEIZUO_LOG_INFO(g_logger) << "hello";x
    }, true);
    m_mainIOManager->stop();
    return 0;
}

int Application::run_fiber() {
    std::vector<Module::ptr> modules;
    ModuleMgr::GetInstance()->listAll(modules);
    bool has_error = false;
    for(auto& i : modules) {
        if(!i->onLoad()) {
            JUJIMEIZUO_LOG_ERROR(g_logger) << "module name="
                << i->getName() << " version=" << i->getVersion()
                << " filename=" << i->getFilename();
            has_error = true;
        }
    }
    if(has_error) {
        _exit(0);
    }

    jujimeizuo::WorkerMgr::GetInstance()->init();

    auto http_confs = g_servers_conf->getValue();
    std::vector<TcpServer::ptr> svrs;
    for(auto& i : http_confs) {
        JUJIMEIZUO_LOG_DEBUG(g_logger) << std::endl << LexicalCast<TcpServerConf, std::string>()(i);

        std::vector<Address::ptr> address;
        for(auto& a : i.address) {
            size_t pos = a.find(":");
            if(pos == std::string::npos) {
                //JUJIMEIZUO_LOG_ERROR(g_logger) << "invalid address: " << a;
                address.push_back(UnixAddress::ptr(new UnixAddress(a)));
                continue;
            }
            int32_t port = atoi(a.substr(pos + 1).c_str());
            //127.0.0.1
            auto addr = jujimeizuo::IPAddress::Create(a.substr(0, pos).c_str(), port);
            if(addr) {
                address.push_back(addr);
                continue;
            }
            std::vector<std::pair<Address::ptr, uint32_t> > result;
            if(jujimeizuo::Address::GetInterfaceAddresses(result,
                                        a.substr(0, pos))) {
                for(auto& x : result) {
                    auto ipaddr = std::dynamic_pointer_cast<IPAddress>(x.first);
                    if(ipaddr) {
                        ipaddr->setPort(atoi(a.substr(pos + 1).c_str()));
                    }
                    address.push_back(ipaddr);
                }
                continue;
            }

            auto aaddr = jujimeizuo::Address::LookupAny(a);
            if(aaddr) {
                address.push_back(aaddr);
                continue;
            }
            JUJIMEIZUO_LOG_ERROR(g_logger) << "invalid address: " << a;
            _exit(0);
        }
        IOManager* accept_worker = jujimeizuo::IOManager::GetThis();
        IOManager* io_worker = jujimeizuo::IOManager::GetThis();
        IOManager* process_worker = jujimeizuo::IOManager::GetThis();
        if(!i.accept_worker.empty()) {
            accept_worker = jujimeizuo::WorkerMgr::GetInstance()->getAsIOManager(i.accept_worker).get();
            if(!accept_worker) {
                JUJIMEIZUO_LOG_ERROR(g_logger) << "accept_worker: " << i.accept_worker
                    << " not exists";
                _exit(0);
            }
        }
        if(!i.io_worker.empty()) {
            io_worker = jujimeizuo::WorkerMgr::GetInstance()->getAsIOManager(i.io_worker).get();
            if(!io_worker) {
                JUJIMEIZUO_LOG_ERROR(g_logger) << "io_worker: " << i.io_worker
                    << " not exists";
                _exit(0);
            }
        }
        if(!i.process_worker.empty()) {
            process_worker = jujimeizuo::WorkerMgr::GetInstance()->getAsIOManager(i.process_worker).get();
            if(!process_worker) {
                JUJIMEIZUO_LOG_ERROR(g_logger) << "process_worker: " << i.process_worker
                    << " not exists";
                _exit(0);
            }
        }

        TcpServer::ptr server;
        if(i.type == "http") {
            server.reset(new jujimeizuo::http::HttpServer(i.keepalive,
                            process_worker, io_worker, accept_worker));
        } else {
            JUJIMEIZUO_LOG_ERROR(g_logger) << "invalid server type=" << i.type
                << LexicalCast<TcpServerConf, std::string>()(i);
            _exit(0);
        }
        //server->start();
        m_servers[i.type].push_back(server);
        svrs.push_back(server);
    }

    for(auto& i : modules) {
        i->onServerReady();
    }

    for(auto& i : svrs) {
        i->start();
    }


    for(auto& i : modules) {
        i->onServerUp();
    }
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