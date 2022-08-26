#include "../src/config.h"
#include "../src/log.h"

jujimeizuo::ConfigVar<int>::ptr g_int_value_config =
	jujimeizuo::Config::Lookup("system.port", (int)8080, "system port");

jujimeizuo::ConfigVar<float>::ptr g_float_value_config =
	jujimeizuo::Config::Lookup("system.value", (float)10.2f, "system value");

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


void test_yaml() {
	YAML::Node root = YAML::LoadFile("/Users/fengzetao/Desktop/WebServer/bin/conf/log.yml");
	print_yaml(root, 0);

	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << root.Scalar();
}

void test_config() {
	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "before: " << g_int_value_config -> getValue();
	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "before: " << g_float_value_config -> toString();

	YAML::Node root = YAML::LoadFile("/Users/fengzetao/Desktop/WebServer/bin/conf/log.yml");
	jujimeizuo::Config::LoadFromYaml(root);

	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "after: " << g_int_value_config -> getValue();
	JUJIMEIZUO_LOG_INFO(JUJIMEIZUO_LOG_ROOT()) << "after: " << g_float_value_config -> toString();
	
}

int main(int argc, char** argv) {
	// test_yaml();
	test_config();
	return 0;
}