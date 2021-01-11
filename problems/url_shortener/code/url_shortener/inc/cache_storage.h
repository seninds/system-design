#pragma once

#include <memory>
#include <string>

#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "nlohmann/json.hpp"

namespace us {

struct ICacheStorage {
    struct Record {
        std::string short_url;
        std::string long_url;

        Record& WithShortUrl(std::string url);
        Record& WithLongUrl(std::string url);
    };

    virtual ~ICacheStorage() = default;
    virtual absl::StatusOr<Record> Read(const std::string& key) const = 0;
    virtual absl::Status Write(const std::string& key, Record record) = 0;
};

std::unique_ptr<ICacheStorage> MakeCacheStorage(nlohmann::json::value_type& config);

inline ICacheStorage::Record& ICacheStorage::Record::WithShortUrl(std::string url) {
    short_url = std::move(url);
    return *this;
}

inline ICacheStorage::Record& ICacheStorage::Record::WithLongUrl(std::string url) {
    long_url = std::move(url);
    return *this;
}

}  // namespace us