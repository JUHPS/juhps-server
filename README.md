<div align='center' >
	<font size='70'>WebServer</font>
</div>

## 1. 日志模块

支持流式日志风格写日志和格式化风格写日志，支持日志格式自定义，日志级别，多日志分离等等功能

流式日志使用：`SYLAR_LOG_INFO(g_logger) << "this is a log";`

格式化日志使用：`SYLAR_LOG_FMT_INFO(g_logger, "%s", "this is a log"); `

支持时间,线程id,线程名称,日志级别,日志名称,文件名,行号等内容的自由配置

### Class

- `Logger`: 日志器
- `LogEevel`: 日志级别
- `LogEvent`: 日志事件
- `LogEventWrap`: 日志事件包装器
- `LogFormatter`: 日志格式化
- `LogAppender`: 日志输出目标
- `LoggerManger`: 日志管理器
- `LoggerMgr`: 日志器管理类单例模式

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

## 2. 配置模块

采用约定由于配置的思想。定义即可使用。不需要单独去解析。支持变更通知功能。使用YAML文件做为配置内容。支持STL容器(vector, list, set, unoredered_set, map, unordered_map等等),支持自定义类型（需要实现序列化和反序列化方法)

配置的事件机制，当一个配置项发生修改的时候，可以反向通知对应的代码，回调。

### Class
- `ConfigVarBase`: 配置变量的基类
- `ConfigVar`: 配置参数模板子类
- `Config`: ConfigVar的管理类
- `LexicalCast`: 类型转换模板类（偏特化）

### Usage

```bash
YAML::Node root = YAML::LoadFile("url/*.yml");
jujimeizuo::Config::LoadFromYaml(root);
```

### yaml-cpp

```c++
YAML::Node node = YAML::LoadFile(const string& filename);

if (node.IsMap()) {
	for (auto it = node.begin(); it != node.end(); ++it) ;
}

if (node.IsSequence()) {
	for (size_t i = 0; i < node.size(); ++i) ;
}

if (node.IsScalar());

```

### 用unordered_map来做配置

```C++
jujimeizuo::ConfigVar<std::unordered_map<std::string, int> >::ptr g_str_int_unordered_map_value_config =
	jujimeizuo::Config::Lookup("system.str_int_unordered_map", std::unordered_map<std::string, int>{{"k", 2}}, "system str int unordered_map");
```

```bash
2022-09-01 00:03:24     4294967295      0       [INFO]  [root]  /Users/fengzetao/Desktop/WebServer/tests/test_config.cc:142     after str_int_unordered_map: {k2 - 20}
```

### setFormatter
当yml里没有格式要求时，logger初始化时m_hasFormatter为false，当后续代码中用setFormatter修改格式时，会将所有的m_hasFormatter为false的formatter修改为用户修改的格式。

## Author

[sylar](https://github.com/sylar-yin/sylar)