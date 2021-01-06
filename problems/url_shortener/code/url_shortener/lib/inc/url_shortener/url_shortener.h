#pragma once

#include <absl/status/statusor.h>

#include <memory>
#include <string>

#include "cache_storage.h"
#include "range_provider.h"
#include "traffic_logger.h"
#include "url_storage.h"

namespace us {

struct IUrlShortener {
    struct Record {
        std::string long_url;
        std::uint64_t created_at;
        std::uint64_t expired_at;
    };

    virtual ~IUrlShortener() = default;
    virtual absl::StatusOr<std::string> Write(Record record) = 0;
    virtual absl::StatusOr<std::string> ReadLongUrl(std::string short_url) = 0;
};

std::unique_ptr<IUrlShortener> MakeUrlShortener(std::unique_ptr<IUrlStorage> url_storage,
                                                std::unique_ptr<ICacheStorage> cache_storage,
                                                std::unique_ptr<IRangeProvider> range_provider,
                                                std::unique_ptr<ITrafficLogger> traffic_logger);

}  // namespace us