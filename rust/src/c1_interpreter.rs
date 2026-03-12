//! # Interpreter Pattern
//!
//! The Interpreter pattern defines a representation for a grammar's language
//! along with an interpreter that uses the representation to interpret sentences.
//!
//! ## Key Participants
//! - **AbstractExpression**: Declares abstract interpret operation
//! - **TerminalExpression**: Implements interpret for terminal symbols
//! - **NonTerminalExpression**: Implements interpret for non-terminal symbols
//! - **Context**: Contains information global to the interpreter
//!
//! ## Benefits
//! - Easier to change and extend the grammar
//! - Implementing the grammar is easy
//! - Complex grammars become hard to maintain

use std::collections::HashMap;

// Context
pub struct Context {
    variables: HashMap<String, i32>,
}

impl Context {
    pub fn new() -> Self {
        Self {
            variables: HashMap::new(),
        }
    }

    pub fn set_variable(&mut self, name: &str, value: i32) {
        self.variables.insert(name.to_string(), value);
    }

    pub fn get_variable(&self, name: &str) -> i32 {
        *self.variables.get(name).unwrap_or(&0)
    }
}

impl Default for Context {
    fn default() -> Self {
        Self::new()
    }
}

// Abstract Expression
pub trait Expression {
    fn interpret(&self, context: &Context) -> i32;
}

// Terminal Expressions
pub struct NumberExpression {
    value: i32,
}

impl NumberExpression {
    pub fn new(value: i32) -> Self {
        Self { value }
    }
}

impl Expression for NumberExpression {
    fn interpret(&self, _context: &Context) -> i32 {
        self.value
    }
}

pub struct VariableExpression {
    name: String,
}

impl VariableExpression {
    pub fn new(name: &str) -> Self {
        Self {
            name: name.to_string(),
        }
    }
}

impl Expression for VariableExpression {
    fn interpret(&self, context: &Context) -> i32 {
        context.get_variable(&self.name)
    }
}

// Non-Terminal Expressions
pub struct AddExpression {
    left: Box<dyn Expression>,
    right: Box<dyn Expression>,
}

impl AddExpression {
    pub fn new(left: Box<dyn Expression>, right: Box<dyn Expression>) -> Self {
        Self { left, right }
    }
}

impl Expression for AddExpression {
    fn interpret(&self, context: &Context) -> i32 {
        self.left.interpret(context) + self.right.interpret(context)
    }
}

pub struct SubtractExpression {
    left: Box<dyn Expression>,
    right: Box<dyn Expression>,
}

impl SubtractExpression {
    pub fn new(left: Box<dyn Expression>, right: Box<dyn Expression>) -> Self {
        Self { left, right }
    }
}

impl Expression for SubtractExpression {
    fn interpret(&self, context: &Context) -> i32 {
        self.left.interpret(context) - self.right.interpret(context)
    }
}

pub struct MultiplyExpression {
    left: Box<dyn Expression>,
    right: Box<dyn Expression>,
}

impl MultiplyExpression {
    pub fn new(left: Box<dyn Expression>, right: Box<dyn Expression>) -> Self {
        Self { left, right }
    }
}

impl Expression for MultiplyExpression {
    fn interpret(&self, context: &Context) -> i32 {
        self.left.interpret(context) * self.right.interpret(context)
    }
}

pub struct DivideExpression {
    left: Box<dyn Expression>,
    right: Box<dyn Expression>,
}

impl DivideExpression {
    pub fn new(left: Box<dyn Expression>, right: Box<dyn Expression>) -> Self {
        Self { left, right }
    }
}

impl Expression for DivideExpression {
    fn interpret(&self, context: &Context) -> i32 {
        let divisor = self.right.interpret(context);
        if divisor == 0 {
            println!("Error: Division by zero!");
            return 0;
        }
        self.left.interpret(context) / divisor
    }
}

/// Example demonstrating Interpreter pattern
pub fn example() {
    println!("\n--- Interpreter Pattern Example ---\n");

    let mut context = Context::new();
    context.set_variable("x", 10);
    context.set_variable("y", 5);
    context.set_variable("z", 2);

    // Expression: (x + y) * z
    println!("1. Arithmetic Expression: (x + y) * z");
    println!("   Where: x=10, y=5, z=2");

    let expr = MultiplyExpression::new(
        Box::new(AddExpression::new(
            Box::new(NumberExpression::new(10)),
            Box::new(NumberExpression::new(5)),
        )),
        Box::new(NumberExpression::new(2)),
    );

    let result = expr.interpret(&context);
    println!("   Result: {}", result);

    // Using variables
    println!("\n2. Using Variables: x + y * z");
    let expr2 = AddExpression::new(
        Box::new(VariableExpression::new("x")),
        Box::new(MultiplyExpression::new(
            Box::new(VariableExpression::new("y")),
            Box::new(VariableExpression::new("z")),
        )),
    );

    let result2 = expr2.interpret(&context);
    println!("   Result: {}", result2);

    // Subtraction
    println!("\n3. Subtraction: (x - y) / z");
    let expr3 = DivideExpression::new(
        Box::new(SubtractExpression::new(
            Box::new(VariableExpression::new("x")),
            Box::new(VariableExpression::new("y")),
        )),
        Box::new(VariableExpression::new("z")),
    );

    let result3 = expr3.interpret(&context);
    println!("   Result: {}", result3);

    // Division by zero example
    println!("\n4. Division by zero: x / 0");
    let expr4 = DivideExpression::new(
        Box::new(VariableExpression::new("x")),
        Box::new(NumberExpression::new(0)),
    );

    let result4 = expr4.interpret(&context);
    println!("   Result: {}", result4);
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_number_expression() {
        let expr = NumberExpression::new(42);
        let context = Context::new();
        assert_eq!(expr.interpret(&context), 42);
    }

    #[test]
    fn test_variable_expression() {
        let mut context = Context::new();
        context.set_variable("x", 10);
        let expr = VariableExpression::new("x");
        assert_eq!(expr.interpret(&context), 10);
    }

    #[test]
    fn test_add_expression() {
        let expr = AddExpression::new(
            Box::new(NumberExpression::new(5)),
            Box::new(NumberExpression::new(3)),
        );
        let context = Context::new();
        assert_eq!(expr.interpret(&context), 8);
    }

    #[test]
    fn test_subtract_expression() {
        let expr = SubtractExpression::new(
            Box::new(NumberExpression::new(10)),
            Box::new(NumberExpression::new(3)),
        );
        let context = Context::new();
        assert_eq!(expr.interpret(&context), 7);
    }

    #[test]
    fn test_multiply_expression() {
        let expr = MultiplyExpression::new(
            Box::new(NumberExpression::new(5)),
            Box::new(NumberExpression::new(4)),
        );
        let context = Context::new();
        assert_eq!(expr.interpret(&context), 20);
    }

    #[test]
    fn test_divide_expression() {
        let expr = DivideExpression::new(
            Box::new(NumberExpression::new(20)),
            Box::new(NumberExpression::new(4)),
        );
        let context = Context::new();
        assert_eq!(expr.interpret(&context), 5);
    }

    #[test]
    fn test_divide_by_zero() {
        let expr = DivideExpression::new(
            Box::new(NumberExpression::new(10)),
            Box::new(NumberExpression::new(0)),
        );
        let context = Context::new();
        assert_eq!(expr.interpret(&context), 0);
    }

    #[test]
    fn test_complex_expression() {
        // (5 + 3) * 2
        let expr = MultiplyExpression::new(
            Box::new(AddExpression::new(
                Box::new(NumberExpression::new(5)),
                Box::new(NumberExpression::new(3)),
            )),
            Box::new(NumberExpression::new(2)),
        );
        let context = Context::new();
        assert_eq!(expr.interpret(&context), 16);
    }
}
