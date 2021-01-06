#pragma once

#include <absl/status/statusor.h>

#include <cstdint>

namespace us {

struct IRangeProvider {
    struct Range {
        std::uint64_t start;
        std::uint64_t end;  // this endpoint is not included
    };

    virtual ~IRangeProvider() = default;
    virtual absl::StatusOr<Range> GetNewRange() = 0;
};

}  // namespace us