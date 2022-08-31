#include "../src/config.h"
#include "../src/log.h"
#include <iostream>

jujimeizuo::ConfigVar<int>::ptr g_int_value_config =
	jujimeizuo::Config::Lookup("system.port", (int)8080, "system port");

jujimeizuo::ConfigVar<float>::ptr g_float_value_config =
	jujimeizuo::Config::Lookup("system.value", (float)10.2f, "system value");

jujimeizuo::ConfigVar<std::vector<int>>::ptr g_int_vec_value_config =
	jujimeizuo::Config::Lookup("system.int_vec", std::vector<int>{1, 2}, "system int vec");


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
	YAML::Node root = YAML::LoadFile("/Users/fengzetao/Desktop/WebServer/bin/conf/log.yml");
	print_yaml(root, 0);

	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << root.Scalar();
}

void test_config() {
	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "before: " << g_int_value_config -> getValue();
	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "before: " << g_float_value_config -> toString();
	auto v = g_int_vec_value_config -> getValue();
	for (auto& i : v) {
		JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "before intvec: " << i;
	}

	YAML::Node root = YAML::LoadFile("/Users/fengzetao/Desktop/WebServer/bin/conf/log.yml");
	LoadFromYaml(root);

	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "after: " << g_int_value_config -> getValue();
	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "after: " << g_float_value_config -> toString();

	v = g_int_vec_value_config -> getValue();
	for (auto& i : v) {
		JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "after int_vec: " << i;
	}
}

int main(int argc, char** argv) {
	// test_yaml();
	test_config();
	return 0;
}