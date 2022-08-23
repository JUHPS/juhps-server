<div align='center' >
	<font size='70'>WebServer</font>
</div>

## 1. 日志模块

### Class

- `Logger`: 日志器
- `LogEevel`: 日志级别
- `LogEvent`: 日志事件
- `LogEventWrap`: 日志事件包装器
- `LogFormatter`: 日志格式化
- `LogAppender`: 日志输出目标
- `LoggerManger`: 日志管理器

### Usage

#### 使用流式方式将日志级别level的日志写入到logger

```C++
JUJIMEIZUO_LOG_INFO(logger) << "test macro";
```

```bash
2022-08-24 00:13:51     4294967295      0       [INFO]  [root]  /Users/fengzetao/Desktop/WebServer/tests/test.cc:24     test macro
```

#### 使用格式化方式将日志级别level的日志写入到logger
```C++
JUJIMEIZUO_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");
```

```bash
2022-08-24 00:13:51     4294967295      0       [ERROR] [root]  /Users/fengzetao/Desktop/WebServer/tests/test.cc:27     test macro fmt error aa
```

#### 日志管理器集成方式将日志级别level的日志写入到logger
```C++
auto l = jujimeizuo::LoggerMgr::GetInstance() -> getLogger("xx");
JUJIMEIZUO_LOG_INFO(l) << "xxx";
```

```bash
2022-08-24 00:13:51     4294967295      0       [INFO]  [root]  /Users/fengzetao/Desktop/WebServer/tests/test.cc:30     xxx
```