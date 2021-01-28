#include "gtest/gtest.h"
#include "lib/in_memory_cache.h"

using namespace us;

TEST(InMemoryCache, ReadWriteTest) {
    InMemoryCache cache(2);
    auto expected0 = InMemoryCache::Record().WithShortUrl("short0").WithLongUrl("long0");
    {
        auto status = cache.Write("key0", expected0);
        ASSERT_TRUE(status.ok());

        auto result = cache.Read("key0");
        ASSERT_TRUE(result.ok());
        ASSERT_EQ(*result, expected0);
    }
}