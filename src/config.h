#ifndef __SRC_CONFIG_H__
#define __SRC_CONFIG_H__

#include <memory>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "log.h"
#include <unordered_map>

namespace jujimeizuo {

/**
 * @brief 配置变量的基类
 */
class ConfigVarBase {
public:
	typedef std::shared_ptr<ConfigVarBase> ptr;
	ConfigVarBase(const std::string& name, const std::string& description = "")
		: m_name(name)
		, m_description(description) {
		}
	virtual ~ConfigVarBase() {}

	const std::string& getName() const { return m_name; }
	const std::string& getDescription() const { return m_description; }

	virtual std::string toString() = 0;
	virtual bool fromString(const std::string& val) = 0;
protected:
	std::string m_name;
	std::string m_description;
};

/**
 * @brief 配置参数模板子类,保存对应类型的参数值
 * @details T 参数的具体类型
 *          FromStr 从std::string转换成T类型的仿函数
 *          ToStr 从T转换成std::string的仿函数
 *          std::string 为YAML格式的字符串
 */
template <class T>
class ConfigVar : public ConfigVarBase {
public:
	typedef std::shared_ptr<ConfigVar> ptr;

	ConfigVar(const std::string& name
			, const T& default_value
			, const std::string& description = "")
		: ConfigVarBase(name, description)
		, m_val(default_value) {
	}

	std::string toString() override {
		try {
			return boost::lexical_cast<std::string>(m_val);
		} catch(std::exception& e) {
			JUJIMEIZUO_LOG_ERROR(JUJIMEIZUO_LOG_ROOT()) << "ConfigVar::toString exception"
				<< e.what() << " convert: " << typeid(m_val).name() << " to string";
		}
		return "";
	}

	bool fromString(const std::string& val) override {
		try {
			m_val = boost::lexical_cast<T>(val);
		}	catch(std::exception& e) {
			JUJIMEIZUO_LOG_ERROR(JUJIMEIZUO_LOG_ROOT()) << "ConfigVar::toString exception"
				<< e.what() << " convert: string to" << typeid(m_val).name();
		}
		return false;
	}

	const T getValue() const { return m_val; }
	void setValue(const T& v) { m_val = v; }

private:
	T m_val;
};

/**
 * @brief ConfigVar的管理类
 * @details 提供便捷的方法创建/访问ConfigVar
 */
class Config {
public:
	typedef std::unordered_map<std::string, ConfigVarBase::ptr> ConfigVarMap;

	/**
     * @brief 获取/创建对应参数名的配置参数
     * @param[in] name 配置参数名称
     * @param[in] default_value 参数默认值
     * @param[in] description 参数描述
     * @details 获取参数名为name的配置参数,如果存在直接返回
     *          如果不存在,创建参数配置并用default_value赋值
     * @return 返回对应的配置参数,如果参数名存在但是类型不匹配则返回nullptr
     * @exception 如果参数名包含非法字符[^0-9a-z_.] 抛出异常 std::invalid_argument
     */
	template <class T>
	static typename ConfigVar<T>::ptr Lookup(const std::string& name,
			const T& default_value, const std::string& description = "") {
		auto tmp = Lookup<T>(name);
		if (tmp) {
			JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "Lookup name=" << name << "exists";
			return tmp;
		}
		if (name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._012345678")
				!= std::string::npos) {
			JUJIMEIZUO_LOG_ERROR(JUJIMEIZUO_LOG_ROOT()) << "Lookup name invalid " << name;
			throw std::invalid_argument(name);
		}
		
		typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
		s_datas[name] = v;
		return v;
	}

	/**
     * @brief 查找配置参数
     * @param[in] name 配置参数名称
     * @return 返回配置参数名为name的配置参数
     */
	template <class T>
	static typename ConfigVar<T>::ptr Lookup(const std::string& name) {
		auto it = s_datas.find(name);
		if (it == s_datas.end()) {
			return nullptr;
		}
		return std::dynamic_pointer_cast<ConfigVar<T> >(it -> second);
	}
private:
	static ConfigVarMap s_datas;
};

}


#endif