#include <gtest/gtest.h>

#include "C1Interpreter.h"

namespace {
TEST (InterpreterTest, SimpleArithmetic) {
    Interpreter::Context context;
    auto expr = std::make_shared<Interpreter::AddExpression> (
    std::make_shared<Interpreter::NumberExpression> (5),
    std::make_shared<Interpreter::NumberExpression> (3));

    EXPECT_EQ (expr->interpret (context), 8);
}

TEST (InterpreterTest, VariableExpression) {
    Interpreter::Context context;
    context.setVariable ("x", 10);
    auto expr = std::make_shared<Interpreter::VariableExpression> ("x");

    EXPECT_EQ (expr->interpret (context), 10);
}
} // namespace
