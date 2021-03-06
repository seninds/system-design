#pragma once

#include <list>
#include <string>
#include <unordered_map>

#include "inc/cache_storage.h"

namespace us {

struct InMemoryCache : ICacheStorage {
    using Record = ICacheStorage::Record;
    InMemoryCache(size_t max_records_count) : max_records_count_(max_records_count) {}

    absl::StatusOr<Record> Read(const std::string& key) const override;
    absl::Status Write(const std::string& key, Record record) override;

   private:
    struct ListItem {
        std::string key;
        Record record;
    };
    using ListIter = std::list<ListItem>::iterator;

    const std::size_t max_records_count_ = 0;
    mutable std::unordered_map<std::string, ListIter> iters_;
    mutable std::list<ListItem> records_;
};

}  // namespace us