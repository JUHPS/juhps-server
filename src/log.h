#ifndef __SRC_LOG_H__
#define __SRC_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include "singleton.h"

/**
 * @brief 使用流式方式将日志级别level的日志写入到logger
 */
#define JUJIMEIZUO_LOG_LEVEL(logger, level) \
	if (logger -> getLevel() <= level) \
		jujimeizuo::LogEventWrap(jujimeizuo::LogEvent::ptr(new jujimeizuo::LogEvent(logger, level, \
				__FILE__, __LINE__, 0, jujimeizuo::GetThreadId(), \
			jujimeizuo::GetFiberId(), time(0)))).getSS()


/**
 * @brief 使用流式方式将日志级别debug的日志写入到logger
 */
#define JUJIMEIZUO_LOG_DEBUG(logger) JUJIMEIZUO_LOG_LEVEL(logger, jujimeizuo::LogLevel::DEBUG)

/**
 * @brief 使用流式方式将日志级别info的日志写入到logger
 */
#define JUJIMEIZUO_LOG_INFO(logger) JUJIMEIZUO_LOG_LEVEL(logger, jujimeizuo::LogLevel::INFO)

/**
 * @brief 使用流式方式将日志级别warn的日志写入到logger
 */
#define JUJIMEIZUO_LOG_WARN(logger) JUJIMEIZUO_LOG_LEVEL(logger, jujimeizuo::LogLevel::WARN)

/**
 * @brief 使用流式方式将日志级别error的日志写入到logger
 */
#define JUJIMEIZUO_LOG_ERROR(logger) JUJIMEIZUO_LOG_LEVEL(logger, jujimeizuo::LogLevel::ERROR)

/**
 * @brief 使用流式方式将日志级别fatal的日志写入到logger
 */
#define JUJIMEIZUO_LOG_FATAL(logger) JUJIMEIZUO_LOG_LEVEL(logger, jujimeizuo::LogLevel::FATAL)


/**
 * @brief 使用格式化方式将日志级别level的日志写入到logger
 */
#define JUJIMEIZUO_LOG_FMT_LEVEL(logger, level, fmt, ...) \
	if (logger -> getLevel() <= level) \
	jujimeizuo::LogEventWrap(jujimeizuo::LogEvent::ptr(new jujimeizuo::LogEvent(logger, level, \
				__FILE__, __LINE__, 0, jujimeizuo::GetThreadId(), \
			jujimeizuo::GetFiberId(), time(0)))).getEvent() -> format(fmt, __VA_ARGS__)	


/**
 * @brief 使用格式化方式将日志级别debug的日志写入到logger
 */
#define JUJIMEIZUO_LOG_FMT_DEBUG(logger, fmt, ...) JUJIMEIZUO_LOG_FMT_LEVEL(logger, jujimeizuo::LogLevel::DEBUG, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别info的日志写入到logger
 */
#define JUJIMEIZUO_LOG_FMT_INFO(logger, fmt, ...)  JUJIMEIZUO_LOG_FMT_LEVEL(logger, jujimeizuo::LogLevel::INFO, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别warn的日志写入到logger
 */
#define JUJIMEIZUO_LOG_FMT_WARN(logger, fmt, ...)  JUJIMEIZUO_LOG_FMT_LEVEL(logger, jujimeizuo::LogLevel::WARN, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别error的日志写入到logger
 */
#define JUJIMEIZUO_LOG_FMT_ERROR(logger, fmt, ...) JUJIMEIZUO_LOG_FMT_LEVEL(logger, jujimeizuo::LogLevel::ERROR, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别fatal的日志写入到logger
 */
#define JUJIMEIZUO_LOG_FMT_FATAL(logger, fmt, ...) JUJIMEIZUO_LOG_FMT_LEVEL(logger, jujimeizuo::LogLevel::FATAL, fmt, __VA_ARGS__)


namespace jujimeizuo {

class Logger;

/**
 * @brief 日志级别
 */
class LogLevel {
public:
	enum Level {
		DEBUG = 1,
		INFO = 2,
		WARN = 3,
		ERROR = 4,
		FATAL = 5
	};

	static const char* ToString(LogLevel::Level level);
};

/**
 * @brief 日志事件
 */
class LogEvent {
public:
	typedef std::shared_ptr<LogEvent> ptr;

	/**
     * @brief 构造函数
     * @param[in] logger 日志器
     * @param[in] level 日志级别
     * @param[in] file 文件名
     * @param[in] line 文件行号
     * @param[in] elapse 程序启动依赖的耗时(毫秒)
     * @param[in] thread_id 线程id
     * @param[in] fiber_id 协程id
     * @param[in] time 日志事件(秒)
     * @param[in] thread_name 线程名称
     */
	LogEvent(std::shared_ptr<Logger> logger,LogLevel::Level level
			, const char* file, int32_t line, uint32_t elapse
			, uint32_t thread_id, uint32_t fiber_id, uint64_t time) ;
	
	const char* getFile() 				const { return m_file; }
	int32_t getLine() 					const { return m_line; }
	uint32_t getElapse() 				const { return m_elapse; }
	uint32_t getThreadId() 				const { return m_threadId; }
	uint32_t getFiberId() 				const { return m_fiberId; }
	uint64_t getTime() 					const { return m_time; }
	const std::string& getThreadName() 	const { return m_threadName;}
	std::string getContent() 			const { return m_ss.str(); }
	std::shared_ptr<Logger> getLogger() const { return m_logger; }
	LogLevel::Level getLevel() 			const { return m_level; }
	std::stringstream& getSS() 			{ return m_ss; }

	void format(const char* fmt, ...);
	void format(const char* fmt, va_list al);
private:
	const char* m_file = nullptr; 		// 文件名
	int32_t m_line = 0;			  		// 行号
	uint32_t m_elapse = 0;				// 程序启动开始到现在的毫秒数
	uint32_t m_threadId = 0;			// 线程id
	uint32_t m_fiberId = 0;				// 协程id
	uint64_t m_time = 0;				// 时间戳
	std::string m_threadName;			// 线程名称
	std::stringstream m_ss;				// 日志内容流
	std::shared_ptr<Logger> m_logger; 	// 日志器
	LogLevel::Level m_level;			// 日志等级
};

/**
 * @brief 日志事件包装器
 */
class LogEventWrap {
public:
	LogEventWrap(LogEvent::ptr e);
	~LogEventWrap();

	LogEvent::ptr getEvent() const { return m_event; }
	std::stringstream& getSS();
private:
	LogEvent::ptr m_event;

};

// 日志格式化
class LogFormatter {
public:
	typedef std::shared_ptr<LogFormatter> ptr;
	LogFormatter(const std::string& pattern);

	/**
     * @brief 返回格式化日志文本
     * @param[in] logger 日志器
     * @param[in] level 日志级别
     * @param[in] event 日志事件
     */
	std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
	std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
public:
	/**
	 * @brief 日志内容项格式化
	 */
	class FormatItem {
	public:
		typedef std::shared_ptr<FormatItem> ptr;
		/**
         * @brief 析构函数
         */
		virtual ~FormatItem() {}
		/**
         * @brief 格式化日志到流
         * @param[in, out] os 日志输出流
         * @param[in] logger 日志器
         * @param[in] level 日志等级
         * @param[in] event 日志事件
         */
		virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
	};

	/**
     * @brief 初始化,解析日志模板
     */
	void init();

	/**
	 * @brief 是否有错误
	 */
	bool isError() const { return m_error; }
private:
	// 日志格式模版
	std::string m_pattern;
	// 日志格式解析后格式
	std::vector<FormatItem::ptr> m_items;
	// 是否有错误
	bool m_error = false;
};

/**
 * @brief 日志输出目标
 */
class LogAppender {
public:
	typedef std::shared_ptr<LogAppender> ptr;
	virtual ~LogAppender() {}

	/**
     * @brief 写入日志
     * @param[in] logger 日志器
     * @param[in] level 日志级别
     * @param[in] event 日志事件
     */
	virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

	/**
     * @brief 更改日志格式器
     */
	void setFormatter(LogFormatter::ptr val) { m_formatter = val; }
	
	/**
	 * @brief 获取日志格式器
	 */
	LogFormatter::ptr getFormatter() const { return m_formatter; }
	
	/**
     * @brief 获取日志级别
     */
	LogLevel::Level getLevel() const { return m_level; }
	
	/**
     * @brief 设置日志级别
     */
	void setLevel(LogLevel::Level val) { m_level = val; }
protected:
	LogLevel::Level m_level = LogLevel::DEBUG;	// 日志级别
	LogFormatter::ptr m_formatter;				// 日志格式器
};

// 日志器
class Logger : public std::enable_shared_from_this<Logger> {
public:
	typedef std::shared_ptr<Logger> ptr;

	/**
     * @brief 构造函数
     * @param[in] name 日志器名称
     */
	Logger(const std::string& name = "root") ;
	
	/**
     * @brief 写日志
     * @param[in] level 日志级别
     * @param[in] event 日志事件
     */
	void log(LogLevel::Level level, LogEvent::ptr event);

	/**
     * @brief 写debug级别日志
     * @param[in] event 日志事件
     */
	void debug(LogEvent::ptr event);
	
	/**
     * @brief 写info级别日志
     * @param[in] event 日志事件
     */
	void info(LogEvent::ptr event);
	
	/**
     * @brief 写warn级别日志
     * @param[in] event 日志事件
     */
	void warn(LogEvent::ptr event);
	
	/**
     * @brief 写error级别日志
     * @param[in] event 日志事件
     */
	void error(LogEvent::ptr event);
	
	/**
     * @brief 写fatal级别日志
     * @param[in] event 日志事件
     */
	void fatal(LogEvent::ptr event);

	/**
     * @brief 添加日志目标
     * @param[in] appender 日志目标
     */
	void addAppender(LogAppender::ptr appender);

	/**
     * @brief 删除日志目标
     * @param[in] appender 日志目标
     */
	void delAppender(LogAppender::ptr appender);
	LogLevel::Level getLevel() const { return m_level; }
	void setLevel(LogLevel::Level val) { m_level = val; }

	/**
     * @brief 返回日志名称
     */
	const std::string& getName() const { return m_name; }
private:
	std::string m_name;							// 日志名称
	LogLevel::Level m_level;					// 日志级别
	std::list<LogAppender::ptr> m_appenders;  	// Appender集合
	LogFormatter::ptr m_formatter;
};

/**
 * @brief 输出到控制台的Appender
 */
class StdoutLogAppender : public LogAppender {
public:
	typedef std::shared_ptr<StdoutLogAppender> ptr;
	void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
};

/**
 * @brief 输出到文件的Appender
 */
class FileLogAppender : public LogAppender {
public:
	typedef std::shared_ptr<FileLogAppender> ptr;
	FileLogAppender(const std::string& filename);
	virtual void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

	// 重新打开文件，文件打开成功返回true
	bool reopen();
private:
	std::string m_filename;
	std::ofstream m_filestream;

};

/**
 * @brief 日志器管理类
 */
class LoggerManger {
public:
	LoggerManger();
	Logger::ptr getLogger(const std::string& name);

	void init();
private:
	std::map<std::string, Logger::ptr> m_loggers;
	Logger::ptr m_root;
};

typedef jujimeizuo::Singleton<LoggerManger> LoggerMgr;


};


#endif
