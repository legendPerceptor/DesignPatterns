#include <gtest/gtest.h>

#include "C10Strategy.h"

namespace {
// Test that different payment strategies can be set
TEST (StrategyTest, SetPaymentStrategy) {
    Strategy::ShoppingCart cart;

    EXPECT_NO_THROW ({
        cart.setPaymentStrategy (std::make_shared<Strategy::CreditCardPayment> (
        "4111-1111-1111-1111", "John Doe"));
    });

    EXPECT_NO_THROW ({
        cart.setPaymentStrategy (
        std::make_shared<Strategy::PayPalPayment> ("john@email.com"));
    });

    EXPECT_NO_THROW ({
        cart.setPaymentStrategy (std::make_shared<Strategy::ApplePayPayment> ("device-123"));
    });
}

// Test that strategy returns correct name
TEST (StrategyTest, StrategyNames) {
    auto creditCard =
    std::make_shared<Strategy::CreditCardPayment> ("4111-1111-1111-1111", "John");
    auto payPal = std::make_shared<Strategy::PayPalPayment> ("test@email.com");
    auto applePay = std::make_shared<Strategy::ApplePayPayment> ("device-123");
    auto crypto   = std::make_shared<Strategy::CryptoPayment> ("0x1234");

    EXPECT_EQ (creditCard->getName (), "Credit Card");
    EXPECT_EQ (payPal->getName (), "PayPal");
    EXPECT_EQ (applePay->getName (), "Apple Pay");
    EXPECT_EQ (crypto->getName (), "Cryptocurrency");
}

// Test compression strategies
TEST (StrategyTest, CompressionStrategies) {
    Strategy::FileCompressor compressor;

    EXPECT_NO_THROW ({
        compressor.setCompressionStrategy (std::make_shared<Strategy::ZipCompression> ());
        compressor.compressFile ("test.txt");
    });

    EXPECT_NO_THROW ({
        compressor.setCompressionStrategy (std::make_shared<Strategy::RarCompression> ());
        compressor.compressFile ("test.txt");
    });

    EXPECT_NO_THROW ({
        compressor.setCompressionStrategy (
        std::make_shared<Strategy::SevenZipCompression> ());
        compressor.compressFile ("test.txt");
    });
}

// Test route calculation strategies
TEST (StrategyTest, RouteStrategies) {
    Strategy::Location start ("Home", 40.7128, -74.0060);
    Strategy::Location end ("Work", 40.7589, -73.9851);

    auto fastest   = std::make_shared<Strategy::FastestRouteStrategy> ();
    auto shortest  = std::make_shared<Strategy::ShortestRouteStrategy> ();
    auto avoidHwys = std::make_shared<Strategy::AvoidHighwaysStrategy> ();
    auto eco       = std::make_shared<Strategy::EcoFriendlyRouteStrategy> ();

    EXPECT_EQ (fastest->getStrategyName (), "Fastest Route");
    EXPECT_EQ (shortest->getStrategyName (), "Shortest Distance");
    EXPECT_EQ (avoidHwys->getStrategyName (), "Avoid Highways");
    EXPECT_EQ (eco->getStrategyName (), "Eco-Friendly");

    // Suppress output during route calculation
    testing::internal::CaptureStdout ();
    EXPECT_NO_THROW ({
        fastest->buildRoute (start, end);
        shortest->buildRoute (start, end);
        avoidHwys->buildRoute (start, end);
        eco->buildRoute (start, end);
    });
    testing::internal::GetCapturedStdout ();
}

// Test strategy can be changed at runtime
TEST (StrategyTest, RuntimeStrategyChange) {
    Strategy::ShoppingCart cart;

    cart.setPaymentStrategy (
    std::make_shared<Strategy::CreditCardPayment> ("4111...", "John"));
    cart.setPaymentStrategy (
    std::make_shared<Strategy::PayPalPayment> ("john@email.com"));

    // Should successfully change strategy
    testing::internal::CaptureStdout ();
    EXPECT_NO_THROW ({ cart.checkout (100.0); });
    testing::internal::GetCapturedStdout ();
}
} // namespace
