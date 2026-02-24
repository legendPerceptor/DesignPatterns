#include <gtest/gtest.h>

#include "B4Decorator.h"

namespace {
// Test plain espresso cost
TEST (DecoratorTest, PlainEspressoCost) {
    std::unique_ptr<Decorator::Beverage> beverage =
    std::make_unique<Decorator::Espresso> ();
    EXPECT_NEAR (beverage->cost (), 1.99, 0.01);
    EXPECT_EQ (beverage->getDescription (), "Espresso");
}

// Test espresso with mocha decorator
TEST (DecoratorTest, EspressoWithMocha) {
    std::unique_ptr<Decorator::Beverage> beverage =
    std::make_unique<Decorator::Espresso> ();
    beverage = std::make_unique<Decorator::Mocha> (std::move (beverage));
    EXPECT_NEAR (beverage->cost (), 1.99 + 0.20, 0.01);
    EXPECT_EQ (beverage->getDescription (), "Espresso, Mocha");
}

// Test multiple decorators
TEST (DecoratorTest, EspressoWithMochaAndWhip) {
    std::unique_ptr<Decorator::Beverage> beverage =
    std::make_unique<Decorator::Espresso> ();
    beverage = std::make_unique<Decorator::Mocha> (std::move (beverage));
    beverage = std::make_unique<Decorator::Whip> (std::move (beverage));
    EXPECT_NEAR (beverage->cost (), 1.99 + 0.20 + 0.10, 0.01);
    EXPECT_EQ (beverage->getDescription (), "Espresso, Mocha, Whip");
}

// Test dark roast with double mocha
TEST (DecoratorTest, DarkRoastWithDoubleMocha) {
    std::unique_ptr<Decorator::Beverage> beverage =
    std::make_unique<Decorator::DarkRoast> ();
    beverage = std::make_unique<Decorator::Mocha> (std::move (beverage));
    beverage = std::make_unique<Decorator::Mocha> (std::move (beverage));
    EXPECT_NEAR (beverage->cost (), 0.99 + 0.20 + 0.20, 0.01);
}

// Test house blend with soy and whip
TEST (DecoratorTest, HouseBlendWithSoyAndWhip) {
    std::unique_ptr<Decorator::Beverage> beverage =
    std::make_unique<Decorator::HouseBlend> ();
    beverage = std::make_unique<Decorator::Soy> (std::move (beverage));
    beverage = std::make_unique<Decorator::Whip> (std::move (beverage));
    EXPECT_NEAR (beverage->cost (), 0.89 + 0.15 + 0.10, 0.01);
    EXPECT_EQ (beverage->getDescription (), "House Blend Coffee, Soy, Whip");
}

// Test all decorators on espresso
TEST (DecoratorTest, EspressoWithAllCondiments) {
    std::unique_ptr<Decorator::Beverage> beverage =
    std::make_unique<Decorator::Espresso> ();
    beverage = std::make_unique<Decorator::Milk> (std::move (beverage));
    beverage = std::make_unique<Decorator::Mocha> (std::move (beverage));
    beverage = std::make_unique<Decorator::Soy> (std::move (beverage));
    beverage = std::make_unique<Decorator::Whip> (std::move (beverage));

    double expectedCost = 1.99 + 0.10 + 0.20 + 0.15 + 0.10;
    EXPECT_NEAR (beverage->cost (), expectedCost, 0.01);
    EXPECT_EQ (beverage->getDescription (), "Espresso, Milk, Mocha, Soy, Whip");
}
} // namespace
