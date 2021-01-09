#pragma once

#include <absl/status/status.h>
#include <absl/status/statusor.h>

#include <cstdint>
#include <string>

namespace us {

struct IUrlStorage {
    struct Record {
        std::string short_url;
        std::string long_url;
        std::uint64_t created_at;
        std::uint64_t expired_at;
    };

    virtual ~IUrlStorage() = default;
    virtual absl::StatusOr<std::string> ReadLongUrl(const std::string& short_url) const = 0;
    virtual absl::Status Write(Record record) = 0;
};

}  // namespace us