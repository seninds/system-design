#pragma once

#include <absl/status/status.h>
#include <absl/status/statusor.h>

#include <string>

namespace us {

struct ICacheStorage {
    struct Record {
        std::string short_url;
        std::string dest_url;
    };

    virtual ~ICacheStorage() = default;
    virtual absl::StatusOr<Record> Read(const std::string& key) const = 0;
    virtual absl::Status Write(const std::string& key, Record record) = 0;
};

}  // namespace us