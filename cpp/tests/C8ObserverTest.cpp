#include <gtest/gtest.h>

#include "C8Observer.h"

namespace {
TEST (ObserverTest, AttachObserver) {
    Observer::WeatherStation station;
    auto display = std::make_shared<Observer::PhoneDisplay> ("Alice");

    testing::internal::CaptureStdout ();
    station.attach (display);
    std::string output = testing::internal::GetCapturedStdout ();

    EXPECT_TRUE (output.find ("Attached") != std::string::npos);
}

TEST (ObserverTest, NotifyObservers) {
    Observer::WeatherStation station;
    auto display = std::make_shared<Observer::PhoneDisplay> ("Alice");
    station.attach (display);

    testing::internal::CaptureStdout ();
    station.setMeasurements (25.0, 65.0, 1013.0);
    std::string output = testing::internal::GetCapturedStdout ();

    EXPECT_TRUE (output.find ("25") != std::string::npos);
}

TEST (ObserverTest, YouTubeChannel) {
    Observer::YouTubeChannel channel ("TestChannel");
    auto sub = std::make_shared<Observer::Subscriber> ("Alice");
    channel.attach (sub);

    testing::internal::CaptureStdout ();
    channel.uploadVideo ("New Video");
    std::string output = testing::internal::GetCapturedStdout ();

    EXPECT_TRUE (output.find ("New Video") != std::string::npos);
}
} // namespace
