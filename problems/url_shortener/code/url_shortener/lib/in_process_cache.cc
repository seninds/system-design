#include "in_process_cache.h"

namespace us {

absl::StatusOr<ICacheStorage::Record> InProcessCache::Read(const std::string& key) const {
    return ICacheStorage::Record().WithShortUrl("fake_short_url").WithLongUrl("fake_long_url");
}

absl::Status InProcessCache::Write(const std::string& key, ICacheStorage::Record record) {
    return absl::OkStatus();
}

}  // namespace us