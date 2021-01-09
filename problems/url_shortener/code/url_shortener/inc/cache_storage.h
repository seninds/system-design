#pragma once

#include <absl/status/status.h>
#include <absl/status/statusor.h>

#include <nlohmann/json.hpp>
#include <string>

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

std::unique_ptr<ICacheStorage> MakeCacheStorage(nlohmann::json::object& config);

}  // namespace us