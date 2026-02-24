#include <gtest/gtest.h>

#include "A3Builder.h"

namespace {
TEST (BuilderTest, BuildHttpRequest) {
    auto req = Builder::HttpRequestBuilder ()
               .method ("POST")
               .url ("https://example.com/api")
               .body ("{\"id\":42}")
               .timeout (5000)
               .build ();

    EXPECT_EQ (req.method, "POST");
    EXPECT_EQ (req.url, "https://example.com/api");
    EXPECT_EQ (req.body, "{\"id\":42}");
    EXPECT_EQ (req.timeoutMs, 5000);
}
} // namespace
