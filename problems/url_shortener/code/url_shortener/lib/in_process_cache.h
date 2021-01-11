#pragma once

#include "inc/cache_storage.h"

namespace us {

struct InProcessCache : ICacheStorage {
    absl::StatusOr<ICacheStorage::Record> Read(const std::string& key) const override;
    absl::Status Write(const std::string& key, ICacheStorage::Record record) override;
};

}  // namespace us