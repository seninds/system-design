#include "inc/cache_storage.h"

#include <stdexcept>

#include "absl/strings/str_cat.h"
#include "lib/in_process_cache.h"

namespace us {

std::unique_ptr<ICacheStorage> MakeCacheStorage(nlohmann::json::value_type& config) {
    const std::string cache_type = config["type"];
    if (cache_type == "in_process") {
        std::size_t max_records_count = config["max_records_count"];
        return std::make_unique<InProcessCache>(max_records_count);
    }
    throw std::runtime_error(absl::StrCat("unknown cache type: ", cache_type));
}

}  // namespace us