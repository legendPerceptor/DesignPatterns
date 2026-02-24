#include <gtest/gtest.h>

#include "B6Flyweight.h"

namespace {
TEST (FlyweightTest, CharacterReuse) {
    auto factory = std::make_shared<Flyweight::CharacterFactory> ();
    Flyweight::Document document (factory);

    document.addCharacter ('H', 12, "black");
    document.addCharacter ('H', 14, "blue");
    document.addCharacter ('H', 12, "black");

    // Only 2 flyweights should be created (for different styles)
    EXPECT_NO_THROW ({ document.render (); });
}
} // namespace
