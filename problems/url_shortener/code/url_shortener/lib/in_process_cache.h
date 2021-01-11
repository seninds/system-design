#pragma once

#include <string>
#include <unordered_map>

#include "inc/cache_storage.h"

namespace us {

struct InProcessCache : ICacheStorage {
    absl::StatusOr<ICacheStorage::Record> Read(const std::string& key) const override;
    absl::Status Write(const std::string& key, ICacheStorage::Record record) override;

   private:
    std::unordered_map<std::string, ICacheStorage::Record> data_;
};

}  // namespace us