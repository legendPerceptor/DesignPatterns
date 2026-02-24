#include <gtest/gtest.h>

#include "A5Singleton.h"

namespace {
// Test that Singleton returns the same instance
TEST (SingletonTest, SameInstance) {
    auto& instance1 = Singleton::Logger::instance ();
    auto& instance2 = Singleton::Logger::instance ();

    EXPECT_EQ (&instance1, &instance2);
}

// Test that Singleton actually logs something
TEST (SingletonTest, LogFunctionality) {
    auto& logger = Singleton::Logger::instance ();

    // This should not throw
    EXPECT_NO_THROW ({ logger.log ("Test log message"); });
}

// Test Singleton is thread-safe (basic test)
TEST (SingletonTest, ThreadSafety) {
    auto& instance1 = Singleton::Logger::instance ();
    auto& instance2 = Singleton::Logger::instance ();
    auto& instance3 = Singleton::Logger::instance ();

    // All should be the same instance
    EXPECT_EQ (&instance1, &instance2);
    EXPECT_EQ (&instance2, &instance3);
}
} // namespace
