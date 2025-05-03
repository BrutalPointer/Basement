#pragma once
#include <array>
#include <optional>
#include <cstdint>

template<typename T, std::size_t Max>
class InstancePool {
public:
    std::optional<uint16_t> New() {
        if (instanceCount_ >= Max) return std::nullopt;
        return instanceCount_++;
    }

    void Free(uint16_t id) {
        if (id + 1 == instanceCount_ && instanceCount_ > 0) {
            --instanceCount_;
        }
    }

    T* Get(uint16_t id) {
        if (id < instanceCount_) return &instances_[id];
        return nullptr;
    }

    uint16_t GetId(const T* ptr) const {
        if (ptr >= &instances_[0] && ptr < &instances_[instanceCount_]) {
            return static_cast<uint16_t>(ptr - &instances_[0]);
        }
        return UINT16_MAX;
    }

    uint16_t ActiveNum() const { return instanceCount_; }

private:
    std::array<T, Max> instances_;
    uint16_t instanceCount_ = 0;
};
