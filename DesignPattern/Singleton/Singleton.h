#pragma once

#include <stdexcept>

template<typename T>
class Singleton {
public:
    template<typename... Args>
    static void Init(Args&&... args) {
        if (!single_instance) {
            single_instance = new T(std::forward<Args>(args)...);
        }
    }

    static T& Instance() {
        if (!single_instance) {
            throw std::runtime_error("Singleton not initialized");
        }
        return *single_instance;
    }

    static void Destroy() {
        delete single_instance;
        single_instance = nullptr;
    }

private:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T* single_instance;
};

template<typename T>
T* Singleton<T>::single_instance = nullptr;