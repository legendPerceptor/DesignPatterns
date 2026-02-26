"""
The intent is to define a representation for a grammar's language along with
an interpreter that uses the representation to interpret sentences in the language.

Key participants:
- AbstractExpression: Declares an abstract Interpret operation
- TerminalExpression: Implements interpret for terminal symbols
- NonterminalExpression: Implements interpret for non-terminal symbols
- Context: Contains information global to the interpreter
- Client: Builds the abstract syntax tree

Key benefits:
- Easier to change and extend the grammar
- Implementing the grammar is easy

Real-world example: A simple expression evaluator for mathematical expressions.
"""

from abc import ABC, abstractmethod
from typing import Dict


# ----- Context -----
class Context:
    def __init__(self) -> None:
        self._variables: Dict[str, int] = {}

    def set_variable(self, name: str, value: int) -> None:
        self._variables[name] = value

    def get_variable(self, name: str) -> int:
        return self._variables.get(name, 0)


# ----- Abstract Expression -----
class Expression(ABC):
    @abstractmethod
    def interpret(self, context: Context) -> int:
        pass


# ----- Terminal Expressions -----
class NumberExpression(Expression):
    def __init__(self, value: int) -> None:
        self._value = value

    def interpret(self, context: Context) -> int:
        return self._value


class VariableExpression(Expression):
    def __init__(self, name: str) -> None:
        self._name = name

    def interpret(self, context: Context) -> int:
        return context.get_variable(self._name)


# ----- Non-Terminal Expressions -----
class AddExpression(Expression):
    def __init__(self, left: Expression, right: Expression) -> None:
        self._left = left
        self._right = right

    def interpret(self, context: Context) -> int:
        return self._left.interpret(context) + self._right.interpret(context)


class SubtractExpression(Expression):
    def __init__(self, left: Expression, right: Expression) -> None:
        self._left = left
        self._right = right

    def interpret(self, context: Context) -> int:
        return self._left.interpret(context) - self._right.interpret(context)


class MultiplyExpression(Expression):
    def __init__(self, left: Expression, right: Expression) -> None:
        self._left = left
        self._right = right

    def interpret(self, context: Context) -> int:
        return self._left.interpret(context) * self._right.interpret(context)


class DivideExpression(Expression):
    def __init__(self, left: Expression, right: Expression) -> None:
        self._left = left
        self._right = right

    def interpret(self, context: Context) -> int:
        divisor = self._right.interpret(context)
        if divisor == 0:
            print("Error: Division by zero!")
            return 0
        return self._left.interpret(context) // divisor


# ----- Boolean Expression Example -----
class BooleanExpression(ABC):
    @abstractmethod
    def evaluate(self, context: Context) -> bool:
        pass


class AndExpression(BooleanExpression):
    def __init__(self, left: BooleanExpression, right: BooleanExpression) -> None:
        self._left = left
        self._right = right

    def evaluate(self, context: Context) -> bool:
        return self._left.evaluate(context) and self._right.evaluate(context)


class OrExpression(BooleanExpression):
    def __init__(self, left: BooleanExpression, right: BooleanExpression) -> None:
        self._left = left
        self._right = right

    def evaluate(self, context: Context) -> bool:
        return self._left.evaluate(context) or self._right.evaluate(context)


class NotExpression(BooleanExpression):
    def __init__(self, expr: BooleanExpression) -> None:
        self._expr = expr

    def evaluate(self, context: Context) -> bool:
        return not self._expr.evaluate(context)


# ----- Example usage -----
def interpreter_example() -> None:
    """
    Demonstrates the Interpreter pattern with mathematical and boolean expressions.
    """
    print("\n--- Interpreter Pattern Example ---")

    context = Context()
    context.set_variable("x", 10)
    context.set_variable("y", 5)
    context.set_variable("z", 2)

    # Expression: (x + y) * z
    print("\n1. Arithmetic Expression: (x + y) * z")
    print("   Where: x=10, y=5, z=2")

    expr = MultiplyExpression(
        AddExpression(NumberExpression(10), NumberExpression(5)),
        NumberExpression(2)
    )

    result = expr.interpret(context)
    print(f"   Result: {result}")

    # Using variables
    print("\n2. Using Variables: x + y * z")
    expr2 = AddExpression(
        VariableExpression("x"),
        MultiplyExpression(VariableExpression("y"), VariableExpression("z"))
    )
    result2 = expr2.interpret(context)
    print(f"   Result: {result2}")


# Test helpers
def get_expression_tree() -> Expression:
    """Helper for tests: Create a sample expression tree."""
    return AddExpression(NumberExpression(5), NumberExpression(3))


def get_context_with_variables() -> Context:
    """Helper for tests: Create context with predefined variables."""
    context = Context()
    context.set_variable("a", 10)
    context.set_variable("b", 5)
    return context


if __name__ == "__main__":
    interpreter_example()
