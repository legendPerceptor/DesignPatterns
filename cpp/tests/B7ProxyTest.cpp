#include <gtest/gtest.h>

#include "B7Proxy.h"

namespace {
TEST (ProxyTest, VirtualProxyLazyLoading) {
    auto image = std::make_shared<Proxy::ProxyImage> ("photo.jpg");

    testing::internal::CaptureStdout ();
    image->display ();
    std::string output = testing::internal::GetCapturedStdout ();

    EXPECT_TRUE (output.find ("Loading") != std::string::npos);
}

TEST (ProxyTest, ProtectionProxyAccessControl) {
    Proxy::ProxyInternet internet;

    testing::internal::CaptureStdout ();
    internet.connectTo ("google.com");
    std::string output = testing::internal::GetCapturedStdout ();

    // The proxy allows connection to non-blocked sites
    // Output should be "Connecting to google.com"
    EXPECT_TRUE (output.find ("Connecting to") != std::string::npos);
}

TEST (ProxyTest, ProtectionProxyBlockedSite) {
    Proxy::ProxyInternet internet;

    testing::internal::CaptureStdout ();
    internet.connectTo ("blocked.com");
    std::string output = testing::internal::GetCapturedStdout ();

    // The proxy blocks access to blocked.com
    EXPECT_TRUE (output.find ("Access Denied") != std::string::npos ||
    output.find ("blocked") != std::string::npos);
}

TEST (ProxyTest, SmartPtrReferenceCounting) {
    testing::internal::CaptureStdout ();
    {
        Proxy::SmartPtr<Proxy::Resource> ptr1 (new Proxy::Resource ());
        {
            Proxy::SmartPtr<Proxy::Resource> ptr2 = ptr1;
            ptr2->doSomething ();
        }
    }
    std::string output = testing::internal::GetCapturedStdout ();

    // Verify reference counting worked
    EXPECT_TRUE (output.find ("ref count = 1") != std::string::npos);
    EXPECT_TRUE (output.find ("ref count = 2") != std::string::npos);
    EXPECT_TRUE (output.find ("Last reference deleted") != std::string::npos);
}
} // namespace
