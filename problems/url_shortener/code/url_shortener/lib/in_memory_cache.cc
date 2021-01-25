#include "in_memory_cache.h"

#include "absl/strings/str_cat.h"

namespace us {

absl::StatusOr<ICacheStorage::Record> InMemoryCache::Read(const std::string& key) const {
    if (auto it = iters_.find(key); it != iters_.end()) {
        records_.splice(records_.end(), records_, it->second);
        return it->second->record;
    }
    return absl::NotFoundError(absl::StrCat("couldn't find key ", key, " in cache"));
}

absl::Status InMemoryCache::Write(const std::string& key, ICacheStorage::Record record) {
    if (auto it = iters_.find(key); it != iters_.end()) {
        it->second->record = std::move(record);
        records_.splice(records_.end(), records_, it->second);
    } else {
        if (records_.size() >= max_records_count_) {
            auto& item = records_.front();
            iters_.erase(item.key);
            records_.pop_front();
        }
        records_.push_back(ListItem{key, std::move(record)});
        iters_[key] = std::prev(records_.end());
    }
    return absl::OkStatus();
}

}  // namespace us