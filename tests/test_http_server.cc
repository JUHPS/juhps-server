#include "src/jujimeizuo.h"

static jujimeizuo::Logger::ptr g_logger = JUJIMEIZUO_LOG_ROOT();

#define XX(...) #__VA_ARGS__

jujimeizuo::IOManager::ptr worker;


void run() {
    g_logger->setLevel(jujimeizuo::LogLevel::INFO);
    jujimeizuo::http::HttpServer::ptr server(new jujimeizuo::http::HttpServer(true));
    jujimeizuo::Address::ptr addr = jujimeizuo::Address::LookupAnyIPAddress("0.0.0.0:8020");
    while (!server->bind(addr)) {
        sleep(2);
    }
    auto sd = server->getServletDispatch();
    
    sd->addServlet("/jujimeizuo/hello", [](jujimeizuo::http::HttpRequest::ptr req, jujimeizuo::http::HttpResponse::ptr rsp, jujimeizuo::http::HttpSession::ptr session){
      rsp->setBody(rsp->toString());
      return 0;
    });


    sd->addGlobServlet("/jujimeizuo/*", [](jujimeizuo::http::HttpRequest::ptr req, jujimeizuo::http::HttpResponse::ptr rsp, jujimeizuo::http::HttpSession::ptr session) {
        rsp->setBody(XX(<html>
                                <head><title> 404 Not Found</ title></ head>
                                <body>
                                <center><h1> 404 Not Found</ h1></ center>
                                   <hr><center>
                                    nginx /
                                1.16.0 <
                            / center >
                            </ body>
                            </ html> < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >));
        return 0;
    });

    server->start();
}

int main(int argc, char **argv) {
    jujimeizuo::EnvMgr::GetInstance()->init(argc, argv);
    jujimeizuo::Config::LoadFromConfDir(jujimeizuo::EnvMgr::GetInstance()->getConfigPath());
    jujimeizuo::Config::LoadFromStaticDir(jujimeizuo::EnvMgr::GetInstance()->getStaticPath());
    jujimeizuo::IOManager iom(1, true, "main");
    worker.reset(new jujimeizuo::IOManager(3, false, "worker"));
    iom.schedule(run);
    return 0;
}
