#ifndef __SRC_TIMER_H__
#define __SRC_TIMER_H__

#include <memory>
#include <set>
#include <vector>
#include "thread.h"

namespace jujimeizuo {

class TimerManager;
class Timer : public std::enable_shared_from_this<Timer> {
friend class TimerManager;
public:
    typedef std::shared_ptr<Timer> ptr;
private:
    /**
     * @brief 构造函数
     * @param[in] ms 定时器执行间隔时间
     * @param[in] cb 回调函数
     * @param[in] recurring 是否循环
     * @param[in] manager 定时器管理器
     */
    Timer(uint64_t ms, std::function<void()> cb, bool recurring, TimerManager* manager);
    /**
     * @brief 构造函数
     * @param[in] next 执行的时间戳(毫秒)
     */
    Timer(uint64_t next);
private:
    bool m_recurring = false;           // 是否循环定时器
    uint64_t m_ms = 0;                  // 执行周期
    uint64_t m_next = 0;                // 精确的执行时间
    std::function<void()> m_cb;           // 回调函数
    TimerManager* m_manager = nullptr;  // 定时器管理器
private:
    /**
     * @brief 定时器比较仿函数
     */
    struct Comparator {
        /**
         * @brief 比较定时器的智能指针的大小(按执行时间排序)
         * @param[in] lhs 定时器智能指针
         * @param[in] rhs 定时器智能指针
         */
        bool operator()(const Timer::ptr& lhs, const Timer::ptr& rhs) const;
    };
};

class TimerManager {
friend class Timer;
public:
    typedef RWMutex RWMutexType;        // RWMutex

    /**
     * @brief 构造函数
    */
    TimerManager();
    /**
     * @brief 析构函数
    */
    virtual ~TimerManager();
    /**
     * @brief 添加定时器
     * @param[in] ms 定时器执行间隔时间
     * @param[in] cb 定时器回调函数
     * @param[in] recurring 是否循环定时器
     */
    Timer::ptr addTimer(uint64_t ms, std::function<void()> cb, bool recurring = false);
    /**
     * @brief 添加条件定时器
     * @param[in] ms 定时器执行间隔时间
     * @param[in] cb 定时器回调函数
     * @param[in] weak_cond 条件
     * @param[in] recurring 是否循环
     */
    Timer::ptr addConditionTimer(uint64_t ms, std::function<void()> cb, std::weak_ptr<void> weak_cond, bool recurring = false);
protected:
    /**
     * @brief 当有新的定时器插入到定时器的首部,执行该函数
     */
    virtual void onTimerInsertedAtFront() = 0;
private:
    RWMutexType m_mutex;                                // Mutex
    std::set<Timer::ptr, Timer::Comparator> m_timers;   // 定时器集合
};

}

#endif