#pragma once

#include <string>

#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "nlohmann/json.hpp"

namespace us {

struct ICacheStorage {
    struct Record {
        std::string short_url;
        std::string long_url;
    };

    virtual ~ICacheStorage() = default;
    virtual absl::StatusOr<Record> Read(const std::string& key) const = 0;
    virtual absl::Status Write(const std::string& key, Record record) = 0;
};

std::unique_ptr<ICacheStorage> MakeCacheStorage(nlohmann::json::object& config) {
    return std::unique_ptr{};
}

}  // namespace us