#include "scheduler.h"
#include "log.h"
#include "macro.h"

namespace jujimeizuo {

static Logger::ptr g_logger = JUJIMEIZUO_LOG_NAME("system");

static thread_local Scheduler* t_scheduler = nullptr;
static thread_local Fiber* t_scheduler_fiber = nullptr;

Scheduler::Scheduler(size_t threads = 1, bool use_caller = true, const std::string& name = "")
    : m_name(name) {
    JUJIMEIZUO_ASSERT(threads > 0);
    
    if (use_caller) {
        Fiber::GetThis();
        --threads;

        JUJIMEIZUO_ASSERT(GetThis() == nullptr);
        t_scheduler = this;

        m_rootFiber.reset(new Fiber(std::bind(&Scheduler::run, this)));
        Thread::SetName(m_name);
        t_scheduler_fiber = m_rootFiber.get();
        m_rootThread = GetThreadId();
        m_threadIds.push_back(m_rootThread);
    } else {
        m_rootThread = -1;
    }
    m_threadCount = threads;
}
Scheduler::~Scheduler() {
    JUJIMEIZUO_ASSERT(m_stopping);
    if (GetThis() == this) {
        t_scheduler = nullptr;
    }
}

Scheduler* Scheduler::GetThis() {
    return t_scheduler;
}

Fiber* Scheduler::GetMainFiber() {
    return t_scheduler_fiber;
}

void Scheduler::start() {
    MutexType::Lock lock(m_mutex);
    if (!m_stopping) {
        return ;
    }
    m_stopping = false;
    JUJIMEIZUO_ASSERT(m_threads.empty());

    m_threads.resize(m_threadCount);
    for (size_t i = 0; i < m_threadCount; ++i) {
        m_threads[i].reset(new Thread(std::bind(&Scheduler::run, this), m_name + "_" + std::to_string(i)));
        m_threadIds.push_back(m_threads[i] -> getId());
    }
    lock.unlock();
}

void Scheduler::stop() {
    m_autoStop = true;
    if (m_rootFiber
            && m_threadCount == 0
            && (m_rootFiber -> getState() == Fiber::TERM
                || m_rootFiber -> getState() == Fiber::INIT)) {
        JUJIMEIZUO_LOG_INFO(g_logger) << this << " stopped";
        m_stopping = true;

        if (stopping()) {
            return ;
        }
    }

    if (m_rootThread != -1) {
        JUJIMEIZUO_ASSERT(GetThis() == this);
    } else {
        JUJIMEIZUO_ASSERT(GetThis() != this);
    }

    m_stopping = true;
    for (size_t i = 0; i < m_threadCount; ++i) {
        tickle();
    }

    if (m_rootFiber) {
        tickle();
    }

    if (stopping()) {
        return ;
    }
}

void Scheduler::setThis() {
    t_scheduler = this;
}

void Scheduler::run() {
    setThis();
    if (GetThreadId() != m_rootThread) {
        t_scheduler_fiber = Fiber::GetThis().get();
    }

    Fiber::ptr idle_fiber(new Fiber(std::bind(&Scheduler::idle, this)));
}



}