#include "in_process_cache.h"

#include "absl/strings/str_cat.h"

namespace us {

absl::StatusOr<ICacheStorage::Record> InProcessCache::Read(const std::string& key) const {
    if (auto it = data_.find(key); it != data_.end()) {
        return it->second;
    }
    return absl::NotFoundError(absl::StrCat("couldn't find key ", key, " in cache"));
}

absl::Status InProcessCache::Write(const std::string& key, ICacheStorage::Record record) {
    data_[key] = std::move(record);
    return absl::OkStatus();
}

}  // namespace us