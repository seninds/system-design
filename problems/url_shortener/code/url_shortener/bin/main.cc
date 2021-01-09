
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

constexpr char kProgramName[] = "URL Shortener";

cxxopts::Options MakeOptions() {
    cxxopts::Options options(kProgramName, "Service to shorten long URLs.");
    auto adder = options.add_options();
    adder("c,config", "config path", cxxopts::value<std::string>()->default_value("/dev/stdin"));
    adder("h,help", "print help message");
    return options;
}

json ReadConfig(const std::string& path, bool print_result = false) {
    json config;
    std::ifstream(path) >> config;
    if (print_result) {
        std::cout << "================================ CONFIG CONTENT ================================" << std::endl;
        std::cout << "Config content: " << std::endl;
        std::cout << config.dump(4) << std::endl;
        std::cout << "================================================================================" << std::endl;
    }
    return config;
}

int main(int argc, char** argv) {
    auto options = MakeOptions();
    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    auto config_path = result["config"].as<std::string>();
    std::cout << "Run " << kProgramName << " with config: " << config_path << std::endl;
    auto config = ReadConfig(config_path, /* print_result */ true);

    return 0;
}