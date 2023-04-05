#ifndef __SRC_NS_NAME_SERVER_MODULE_H__
#define __SRC_NS_NAME_SERVER_MODULE_H__

#include "../module.h"
#include "ns_protocol.h"

namespace jujimeizuo {
namespace ns {

class NameServerModule;
class NSClientInfo {
friend class NameServerModule;
public:
    typedef std::shared_ptr<NSClientInfo> ptr;
private:
    NSNode::ptr m_node;
    std::map<std::string, std::set<uint32_t> > m_domain2cmds;
};

class NameServerModule : public RockModule {
public:
    typedef std::shared_ptr<NameServerModule> ptr;
    NameServerModule();

    virtual bool handleRockRequest(jujimeizuo::RockRequest::ptr request
                        ,jujimeizuo::RockResponse::ptr response
                        ,jujimeizuo::RockStream::ptr stream) override;
    virtual bool handleRockNotify(jujimeizuo::RockNotify::ptr notify
                        ,jujimeizuo::RockStream::ptr stream) override;
    virtual bool onConnect(jujimeizuo::Stream::ptr stream) override;
    virtual bool onDisconnect(jujimeizuo::Stream::ptr stream) override;
    virtual std::string statusString() override;
private:
    bool handleRegister(jujimeizuo::RockRequest::ptr request
                        ,jujimeizuo::RockResponse::ptr response
                        ,jujimeizuo::RockStream::ptr stream);
    bool handleQuery(jujimeizuo::RockRequest::ptr request
                        ,jujimeizuo::RockResponse::ptr response
                        ,jujimeizuo::RockStream::ptr stream);
    bool handleTick(jujimeizuo::RockRequest::ptr request
                        ,jujimeizuo::RockResponse::ptr response
                        ,jujimeizuo::RockStream::ptr stream);

private:
    NSClientInfo::ptr get(jujimeizuo::RockStream::ptr rs);
    void set(jujimeizuo::RockStream::ptr rs, NSClientInfo::ptr info);

    void setQueryDomain(jujimeizuo::RockStream::ptr rs, const std::set<std::string>& ds);

    void doNotify(std::set<std::string>& domains, std::shared_ptr<NotifyMessage> nty);

    std::set<jujimeizuo::RockStream::ptr> getStreams(const std::string& domain);
private:
    NSDomainSet::ptr m_domains;

    jujimeizuo::RWMutex m_mutex;
    std::map<jujimeizuo::RockStream::ptr, NSClientInfo::ptr> m_sessions;

    /// sessoin 关注的域名
    std::map<jujimeizuo::RockStream::ptr, std::set<std::string> > m_queryDomains;
    /// 域名对应关注的session
    std::map<std::string, std::set<jujimeizuo::RockStream::ptr> > m_domainToSessions;
};

}
}

#endif
