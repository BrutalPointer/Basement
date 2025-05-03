#pragma once

#include "Singleton.h"
#include <string>
#include <vector>

class NetConnectionPool {
public:
    NetConnectionPool(int maxConns, std::vector<std::string>* logs)
        : _maxConns(maxConns), _logs(logs) {}

    void Connect(const std::string& host) {
        if ((int)_activeConns < _maxConns) {
            _logs->push_back("Connected to " + host);
            _activeConns++;
        } else {
            _logs->push_back("Connection pool full.");
        }
    }

private:
    int _maxConns;
    int _activeConns = 0;
    std::vector<std::string>* _logs;
};