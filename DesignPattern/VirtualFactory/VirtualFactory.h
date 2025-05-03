#pragma once
#include <functional>
#include <unordered_map>
#include <memory>
#include <any>

template<typename T>
class VirtualFactory {
public:
    using Creator = std::function<std::unique_ptr<T>(const std::any&)>;
    using Destructor = std::function<void(T*)>;

    void Register(uint32_t typeId, Creator creator, Destructor destructor = nullptr) {
        creators_[typeId] = creator;
        destructors_[typeId] = destructor;
    }

    std::unique_ptr<T> Create(uint32_t typeId, const std::any& param = {}) const {
        auto it = creators_.find(typeId);
        if (it != creators_.end()) {
            return it->second(param);
        }
        return nullptr;
    }

    void Destroy(uint32_t typeId, T* instance) const {
        auto it = destructors_.find(typeId);
        if (it != destructors_.end() && it->second) {
            it->second(instance);
        }
    }

private:
    std::unordered_map<uint32_t, Creator> creators_;
    std::unordered_map<uint32_t, Destructor> destructors_;
};