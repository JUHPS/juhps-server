#include "scheduler.h"
#include "log.h"

namespace jujimeizuo {

static Logger::ptr g_logger = JUJIMEIZUO_LOG_NAME("system");

Scheduler::Scheduler(size_t threads = 1, bool use_caller = true, const std::string& name = "") {
}
Scheduler::~Scheduler() {
}

Scheduler* Scheduler::GetThis() {
}

Fiber* Scheduler::GetMainFiber() {
}

void Scheduler::start() {
}

void Scheduler::stop() {
}


}