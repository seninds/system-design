#pragma once

#include <absl/status/status.h>
#include <absl/status/statusor.h>

#include <cstdint>
#include <string>

namespace us {

struct ITrafficLogger {
    enum class RequestType {
        kAddLongUrl = 0,
        kFindLongUrl,
    };

    struct Record {
        std::uint64_t timestamp;
        RequestType request;
        std::string short_url;
        std::string long_url;
    };

    virtual ~ITrafficLogger() = default;
    virtual absl::Status Write(Record record) = 0;
};

}  // namespace us