#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <cerrno>
#include <filesystem>


class Logger {
private:
    std::ofstream file_;

public:
    Logger(const std::string& filename) : file_(filename) {
        if (!file_.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
        }
    }

    ~Logger() { 
        file_.close();
    }

    void log(const std::string& message) {
        if (file_.is_open()) {
            file_ << message << "\n";
        }
    }
};


std::string get_absolute_path(const std::string& relative_path) {
    return std::filesystem::canonical(relative_path).string();
}
