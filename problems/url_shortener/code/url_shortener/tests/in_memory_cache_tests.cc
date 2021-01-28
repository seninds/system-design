#include "gtest/gtest.h"
#include "lib/in_memory_cache.h"

using namespace us;

TEST(InMemoryCache, ReadWriteTest) {
    InMemoryCache cache(2);
    auto expected0 = InMemoryCache::Record().WithShortUrl("short0").WithLongUrl("long0");
    {
        ASSERT_TRUE(cache.Write("key0", expected0).ok());
        ASSERT_TRUE(cache.Read("key0").ok());
        ASSERT_EQ(*cache.Read("key0"), expected0);
    }

    auto expected1 = InMemoryCache::Record().WithShortUrl("short1").WithLongUrl("long1");
    {
        ASSERT_TRUE(cache.Write("key1", expected1).ok());

        ASSERT_TRUE(cache.Read("key0").ok());
        ASSERT_EQ(*cache.Read("key0"), expected0);
        ASSERT_TRUE(cache.Read("key1").ok());
        ASSERT_EQ(*cache.Read("key1"), expected1);
    }

    auto expected2 = InMemoryCache::Record().WithShortUrl("short2").WithLongUrl("long2");
    {
        ASSERT_TRUE(cache.Write("key2", expected2).ok());

        ASSERT_TRUE(absl::IsNotFound(cache.Read("key0").status()));
        ASSERT_TRUE(cache.Read("key1").ok());
        ASSERT_EQ(*cache.Read("key1"), expected1);
        ASSERT_TRUE(cache.Read("key2").ok());
        ASSERT_EQ(*cache.Read("key2"), expected2);
    }

    {
        ASSERT_TRUE(absl::IsNotFound(cache.Read("key0").status()));
        ASSERT_TRUE(cache.Write("key0", expected0).ok());

        ASSERT_TRUE(absl::IsNotFound(cache.Read("key1").status()));
        ASSERT_TRUE(cache.Read("key0").ok());
        ASSERT_EQ(*cache.Read("key0"), expected0);
        ASSERT_TRUE(cache.Read("key2").ok());
        ASSERT_EQ(*cache.Read("key2"), expected2);
    }
}