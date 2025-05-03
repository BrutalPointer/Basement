#include "Log.h"
#include "ConfigData.h"
#include "NetConnectPool.h"

#include <iostream>

int main() {
    // Logger usage
    Singleton<Logger>::Init(LogLevel::Debug, "log.txt");
    Singleton<Logger>::Instance().Log(LogLevel::Info, "Logger initialized.");

    // ConfigManager usage
    ConfigData data = {{ {"env", "production"}, {"version", "2.1.3"} }};
    Singleton<ConfigManager>::Init(data);
    std::cout << "Version: " << Singleton<ConfigManager>::Instance().Get("version") << std::endl;

    // NetConnectionPool usage
    std::vector<std::string> logs;
    Singleton<NetConnectionPool>::Init(2, &logs);
    Singleton<NetConnectionPool>::Instance().Connect("192.168.0.1");
    Singleton<NetConnectionPool>::Instance().Connect("192.168.0.2");
    Singleton<NetConnectionPool>::Instance().Connect("192.168.0.3"); // should fail

    for (const auto& log : logs) {
        std::cout << log << std::endl;
    }

    return 0;
}
