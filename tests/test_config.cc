#include "../src/config.h"
#include "../src/log.h"
#include <iostream>

jujimeizuo::ConfigVar<int>::ptr g_int_value_config =
	jujimeizuo::Config::Lookup("system.port", (int)8080, "system port");

jujimeizuo::ConfigVar<float>::ptr g_int_valuex_config =
	jujimeizuo::Config::Lookup("system.port", (float)8080, "system port");

jujimeizuo::ConfigVar<float>::ptr g_float_value_config =
	jujimeizuo::Config::Lookup("system.value", (float)10.2f, "system value");

jujimeizuo::ConfigVar<std::vector<int>>::ptr g_int_vec_value_config =
	jujimeizuo::Config::Lookup("system.int_vec", std::vector<int>{1, 2}, "system int vec");

jujimeizuo::ConfigVar<std::list<int>>::ptr g_int_list_value_config =
	jujimeizuo::Config::Lookup("system.int_list", std::list<int>{1, 2}, "system int list");

jujimeizuo::ConfigVar<std::set<int>>::ptr g_int_set_value_config =
	jujimeizuo::Config::Lookup("system.int_set", std::set<int>{1, 2}, "system int set");

jujimeizuo::ConfigVar<std::unordered_set<int>>::ptr g_int_unordered_set_value_config =
	jujimeizuo::Config::Lookup("system.int_unordered_set", std::unordered_set<int>{1, 2}, "system int map");

jujimeizuo::ConfigVar<std::map<std::string, int> >::ptr g_str_int_map_value_config =
	jujimeizuo::Config::Lookup("system.str_int_map", std::map<std::string, int>{{"k", 2}}, "system str int map");

jujimeizuo::ConfigVar<std::unordered_map<std::string, int> >::ptr g_str_int_unordered_map_value_config =
	jujimeizuo::Config::Lookup("system.str_int_unordered_map", std::unordered_map<std::string, int>{{"k", 2}}, "system str int unordered_map");


void print_yaml(const YAML::Node node, int level) {
	if (node.IsScalar()) {
		JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << std::string(level * 4, ' ')
			<< node.Scalar() << " - " << node.Type() << " - " << level;
	} else if (node.IsNull()) {
		JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << std::string(level * 4, ' ')
			<< "NULL - " << node.Type() << " - " << level;
	} else if (node.IsMap()) {
		for (auto it = node.begin(); it != node.end(); ++it) {
			JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << std::string(level * 4, ' ')
				<< it -> first << " - " << it -> second.Type() << " - " << level;
			print_yaml(it -> second, level + 1);
		}
	} else if (node.IsSequence()) {
		for (size_t i = 0; i < node.size(); ++i) {
			JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << std::string(level * 4, ' ')
				<< i << " - " << node[i].Type() << " - " << level;
			print_yaml(node[i], level);
		}
	}
}

static void ListAllMember(const std::string& prefix,
						  const YAML::Node& node,
						  std::list<std::pair<std::string, const YAML::Node>>& output) {
	if(prefix.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._012345678")
			!= std::string::npos) {
		JUJIMEIZUO_LOG_ERROR(JUJIMEIZUO_LOG_ROOT()) << "Config invalid name: " << prefix << " : " << node;
		return;
	}
	output.push_back(std::make_pair(prefix, node));
	if (node.IsMap()) {
		for (auto it = node.begin(); it != node.end(); ++it) {
			ListAllMember(prefix.empty() ? it -> first.Scalar() 
					: prefix + "." + it -> first.Scalar(), it -> second, output);
		}
	}
}

void LoadFromYaml(const YAML::Node& root) {
	std::list<std::pair<std::string, const YAML::Node> > all_nodes;
	  ListAllMember("", root, all_nodes);

	for (auto& i : all_nodes) {
		std::string key = i.first;
		if (key.empty()) {
			continue ;
		}

		std::transform(key.begin(), key.end(), key.begin(), ::tolower);
		jujimeizuo::ConfigVarBase::ptr var = jujimeizuo::Config::LookupBase(key);

		if (var) {
			if (i.second.IsScalar()) {
				var -> fromString(i.second.Scalar());
			} else {
				std::stringstream ss;
				ss << i.second;
				var -> fromString(ss.str());
			}
		}
	}
}









void test_yaml() {
	YAML::Node root = YAML::LoadFile("/Users/fengzetao/Desktop/WebServer/bin/conf/test.yml");
	print_yaml(root, 0);

	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << root.Scalar();
}

void test_config() {
	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "before: " << g_int_value_config -> getValue();
	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "before: " << g_float_value_config -> toString();

#define XX(g_var, name, prefix) \
	{ \
		auto& v = g_var -> getValue(); \
		for (auto& i : v) { \
			JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << #prefix " " #name ": " << i; \
		} \
		JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << #prefix " " #name " yaml: " << g_var -> toString(); \
	}

#define XX_M(g_var, name, prefix) \
	{ \
		auto& v = g_var -> getValue(); \
		for (auto& i : v) { \
			JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << #prefix " " #name ": {" \
					<< i.first << " - " << i.second << "}"; \
		} \
		JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << #prefix " " #name " yaml: " << g_var -> toString(); \
	}

	XX(g_int_vec_value_config, int_vec, before);
	XX(g_int_list_value_config, int_list, before);
	XX(g_int_set_value_config, int_set, before);
	XX(g_int_unordered_set_value_config, int_unordered_set, before);
	XX_M(g_str_int_map_value_config, str_int_map, before);
	XX_M(g_str_int_unordered_map_value_config, str_int_unordered_map, before);

	YAML::Node root = YAML::LoadFile("/Users/fengzetao/Desktop/WebServer/bin/conf/test.yml");
	LoadFromYaml(root);

	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "after: " << g_int_value_config -> getValue();
	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "after: " << g_float_value_config -> toString();

	XX(g_int_vec_value_config, int_vec, after);
	XX(g_int_list_value_config, int_list, after);
	XX(g_int_set_value_config, int_set, after);
	XX(g_int_unordered_set_value_config, int_unordered_set, after);
	XX_M(g_str_int_map_value_config, str_int_map, after);
	XX_M(g_str_int_unordered_map_value_config, str_int_unordered_map, after);
}

class Person {
public:
	Person() {}
	std::string m_name;
	int m_age = 0;
	bool m_sex = 0;

	std::string toString() const {
		std::stringstream ss;
		ss << "[Person name=" << m_name
		   << " age=" << m_age
		   << " sex=" << m_sex
		   << "]";
		return ss.str();
	}

	bool operator==(const Person& oth) const {
		return m_name == oth.m_name
			&& m_age == oth.m_age
			&& m_sex == oth.m_sex;
	}
};

namespace jujimeizuo {

template <>
class LexicalCast<std::string, Person> {
public:
	Person operator()(const std::string& v) {
		YAML::Node node = YAML::Load(v);
		Person p;
		p.m_name = node["name"].as<std::string>();
		p.m_age = node["age"].as<int>();
		p.m_sex = node["sex"].as<bool>();
		return p;
	}
};

template <>
class LexicalCast<Person, std::string> {
public:
	std::string operator()(const Person& p) {
		YAML::Node node;
		node["name"] = p.m_name;
		node["age"] = p.m_age;
		node["sex"] = p.m_sex;
		std::stringstream ss;
		ss << node;
		return ss.str();
	}
};

}

jujimeizuo::ConfigVar<Person>::ptr g_person =
	jujimeizuo::Config::Lookup("class.person", Person(), "class person");

jujimeizuo::ConfigVar<std::map<std::string, Person> >::ptr g_person_map =
	jujimeizuo::Config::Lookup("class.map", std::map<std::string, Person>(), "class person");

jujimeizuo::ConfigVar<std::map<std::string, std::vector<Person> > >::ptr g_person_vec_map =
	jujimeizuo::Config::Lookup("class.vec_map", std::map<std::string, std::vector<Person> >(), "class person");

void test_class() {
	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "before: " << g_person -> getValue().toString() << " - " << g_person -> toString();
	
#define XX_PM(g_var, prefix) \
	{ \
		auto m = g_var -> getValue(); \
		for (auto& i : m) { \
			JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << #prefix << ": " << i.first << " - " << i.second.toString(); \
		} \
		JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << #prefix << ": size=" << m.size(); \
	}

	g_person -> addListener(10, [](const Person& old_value, const Person& new_value) {
		JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "old_value=" << old_value.toString()
				<< "new_value=" << new_value.toString();
	});

	XX_PM(g_person_map, "class.map before");
	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "before: " << g_person_vec_map -> toString();

	YAML::Node root = YAML::LoadFile("/Users/fengzetao/Desktop/WebServer/bin/conf/test.yml");
	LoadFromYaml(root);

	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "after: " << g_person -> getValue().toString() << " - " << g_person -> toString();
	XX_PM(g_person_map, "class.map after");
	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "after: " << g_person_vec_map -> toString();
}

int main(int argc, char** argv) {
	// test_yaml();
	// test_config();
	test_class();
	return 0;
}