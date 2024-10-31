#pragma once

#include <nlohmann/json.hpp>
#include <fstream>
#include <functional>
#include <nlohmann/json_fwd.hpp>
#include <string>

#include <filesystem>

using json = nlohmann::json;

void log_algorithm(const std::string& algorithm_name, const json& parameters,
                   std::function<void(std::function<void(const json&)>)> algorithm,
                   const std::string& log_path = "../build/files/") {

    json log_json;
    log_json["algorithm"] = algorithm_name;
    log_json["parameters"] = parameters;
    log_json["steps"] = json::array();

    auto log_step = [&log_json](const json& step) {
        log_json["steps"].push_back(step);
    };

    algorithm(log_step);

    std::filesystem::path path(log_path);
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directories(path);
    }

    std::ofstream outfile(path / (algorithm_name + "_log.json"));
    outfile << log_json.dump(4); 
    outfile.close();
}
