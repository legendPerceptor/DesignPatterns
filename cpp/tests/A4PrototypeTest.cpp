#include <gtest/gtest.h>

#include "A4Prototype.h"

namespace {
TEST (PrototypeTest, CloneCircle) {
    Prototype::PrototypeRegistry registry;
    auto redCircle = std::make_unique<Prototype::Circle> ();
    redCircle->setRadius (10.0);
    redCircle->setColor ("red");

    registry.registerPrototype ("red-circle", std::move (redCircle));

    auto cloned = registry.create ("red-circle");
    ASSERT_NE (cloned, nullptr);
    EXPECT_NO_THROW ({
        auto circle = dynamic_cast<Prototype::Circle*> (cloned.get ());
        ASSERT_NE (circle, nullptr);
        EXPECT_EQ (circle->getRadius (), 10.0);
    });
}
} // namespace
