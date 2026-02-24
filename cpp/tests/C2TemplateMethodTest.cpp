#include <gtest/gtest.h>

#include "C2TemplateMethod.h"

namespace {
TEST (TemplateMethodTest, TextProcessorWorks) {
    TemplateMethod::TextTransformationProcessor processor;
    EXPECT_NO_THROW ({ processor.processData ("Hello"); });
}

TEST (TemplateMethodTest, NumberProcessorWorks) {
    TemplateMethod::NumberCalculationProcessor processor;
    EXPECT_NO_THROW ({ processor.processData ("10, 20, 30"); });
}
} // namespace
