#pragma once

#include "Singleton.h"

#include <string>
#include <unordered_map>

struct ConfigData {
    std::unordered_map<std::string, std::string> values;
};

class ConfigManager {
public:
    explicit ConfigManager(const ConfigData& config) : config_(config) {}

    std::string Get(const std::string& key) const {
        auto it = config_.values.find(key);
        return (it != config_.values.end()) ? it->second : "";
    }

private:
    ConfigData config_;
};