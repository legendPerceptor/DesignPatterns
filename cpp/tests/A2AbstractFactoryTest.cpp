#include <gtest/gtest.h>

#include "A2AbstractFactory.h"

namespace {
TEST (AbstractFactoryTest, CreateWinFactory) {
    auto factory = std::make_unique<AbstractFactory::WinFactory> ();
    AbstractFactory::Application app (std::move (factory));
    EXPECT_NO_THROW ({ app.renderUI (); });
}

TEST (AbstractFactoryTest, CreateMacFactory) {
    auto factory = std::make_unique<AbstractFactory::MacFactory> ();
    AbstractFactory::Application app (std::move (factory));
    EXPECT_NO_THROW ({ app.renderUI (); });
}
} // namespace
