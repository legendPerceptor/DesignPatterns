#include <gtest/gtest.h>

#include "B2Bridge.h"

namespace {
TEST (BridgeTest, RemoteControlWithTV) {
    auto tv = std::make_unique<Bridge::TV> ("Sony");
    Bridge::RemoteControl remote (std::move (tv));

    EXPECT_NO_THROW ({
        remote.togglePower ();
        remote.volumeUp ();
    });
}
} // namespace
