#include <gtest/gtest.h>

#include "C3ChainOfResponsibility.h"

namespace {
// Test that basic ticket is handled by Level 1 support
TEST (ChainOfResponsibilityTest, BasicTicketHandledByLevel1) {
    auto level1 = std::make_shared<ChainOfResponsibility::Level1Support> ();
    ChainOfResponsibility::SupportTicket ticket (
    "Password reset needed", ChainOfResponsibility::SupportTicket::Level::Basic);

    // Redirect cout to suppress output during testing
    testing::internal::CaptureStdout ();
    level1->handleTicket (ticket);
    std::string output = testing::internal::GetCapturedStdout ();

    EXPECT_TRUE (output.find ("Level 1 Support") != std::string::npos);
}

// Test that critical ticket escalates to Director
TEST (ChainOfResponsibilityTest, CriticalTicketEscalatesToDirector) {
    auto level1   = std::make_shared<ChainOfResponsibility::Level1Support> ();
    auto level2   = std::make_shared<ChainOfResponsibility::Level2Support> ();
    auto manager  = std::make_shared<ChainOfResponsibility::Manager> ();
    auto director = std::make_shared<ChainOfResponsibility::Director> ();

    // Build the chain
    level1->setNext (level2);
    level2->setNext (manager);
    manager->setNext (director);

    ChainOfResponsibility::SupportTicket ticket (
    "Security breach", ChainOfResponsibility::SupportTicket::Level::Critical);

    testing::internal::CaptureStdout ();
    level1->handleTicket (ticket);
    std::string output = testing::internal::GetCapturedStdout ();

    // Should escalate through all levels to Director
    EXPECT_TRUE (output.find ("Director") != std::string::npos);
}

// Test chain can be built with different handlers
TEST (ChainOfResponsibilityTest, ChainConstruction) {
    auto level1 = std::make_shared<ChainOfResponsibility::Level1Support> ();
    auto level2 = std::make_shared<ChainOfResponsibility::Level2Support> ();

    EXPECT_NO_THROW ({ level1->setNext (level2); });
}

// Test intermediate ticket escalates past Level 1
TEST (ChainOfResponsibilityTest, IntermediateTicketEscalatesPastLevel1) {
    auto level1 = std::make_shared<ChainOfResponsibility::Level1Support> ();
    auto level2 = std::make_shared<ChainOfResponsibility::Level2Support> ();

    level1->setNext (level2);

    ChainOfResponsibility::SupportTicket ticket (
    "Software installation failure", ChainOfResponsibility::SupportTicket::Level::Intermediate);

    testing::internal::CaptureStdout ();
    level1->handleTicket (ticket);
    std::string output = testing::internal::GetCapturedStdout ();

    EXPECT_TRUE (output.find ("Level 2 Support") != std::string::npos);
}

// Test ticket level string conversion
TEST (ChainOfResponsibilityTest, TicketLevelString) {
    ChainOfResponsibility::SupportTicket basic (
    "Test", ChainOfResponsibility::SupportTicket::Level::Basic);
    EXPECT_EQ (basic.getLevelString (), "Basic");

    ChainOfResponsibility::SupportTicket critical (
    "Test", ChainOfResponsibility::SupportTicket::Level::Critical);
    EXPECT_EQ (critical.getLevelString (), "Critical");
}
} // namespace
