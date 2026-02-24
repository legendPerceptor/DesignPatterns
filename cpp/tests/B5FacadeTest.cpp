#include <gtest/gtest.h>

#include "B5Facade.h"

namespace {
TEST (FacadeTest, HomeTheaterWatchMovie) {
    auto amp       = std::make_shared<Facade::Amplifier> ();
    auto tuner     = std::make_shared<Facade::Tuner> ();
    auto dvd       = std::make_shared<Facade::DvdPlayer> ();
    auto projector = std::make_shared<Facade::Projector> ();
    auto lights    = std::make_shared<Facade::TheaterLights> ();
    auto screen    = std::make_shared<Facade::Screen> ();
    auto popper    = std::make_shared<Facade::PopcornPopper> ();

    Facade::HomeTheaterFacade homeTheater (
    amp, tuner, dvd, nullptr, projector, lights, screen, popper);

    EXPECT_NO_THROW ({ homeTheater.watchMovie ("Inception"); });
}
} // namespace
