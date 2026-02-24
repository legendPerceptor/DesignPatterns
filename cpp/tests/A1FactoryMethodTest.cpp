#include <gtest/gtest.h>

#include "A1FactoryMethod.h"

namespace {
TEST (FactoryMethodTest, CreateSpreadsheetApp) {
    auto app = std::make_unique<FactoryMethod::SpreadsheetApp> ();
    EXPECT_NO_THROW ({ app->newDocument (); });
}

TEST (FactoryMethodTest, CreateTextEditorApp) {
    auto app = std::make_unique<FactoryMethod::TextEditorApp> ();
    EXPECT_NO_THROW ({ app->newDocument (); });
}
} // namespace
