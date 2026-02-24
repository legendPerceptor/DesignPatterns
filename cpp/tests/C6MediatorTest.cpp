#include <gtest/gtest.h>

#include "C6Mediator.h"

namespace {
TEST (MediatorTest, AircraftRegistration) {
    auto tower = std::make_shared<Mediator::ControlTower> ();
    auto plane = std::make_shared<Mediator::Aircraft> ("Flight101", tower);

    testing::internal::CaptureStdout ();
    tower->registerAircraft (plane);
    std::string output = testing::internal::GetCapturedStdout ();

    EXPECT_TRUE (output.find ("registered") != std::string::npos);
}

TEST (MediatorTest, SendMessage) {
    auto tower  = std::make_shared<Mediator::ControlTower> ();
    auto plane1 = std::make_shared<Mediator::Aircraft> ("Flight101", tower);
    auto plane2 = std::make_shared<Mediator::Aircraft> ("Flight202", tower);

    tower->registerAircraft (plane1);
    tower->registerAircraft (plane2);

    testing::internal::CaptureStdout ();
    plane1->sendMessage ("Hello");
    std::string output = testing::internal::GetCapturedStdout ();

    EXPECT_TRUE (output.find ("Hello") != std::string::npos);
}
} // namespace
