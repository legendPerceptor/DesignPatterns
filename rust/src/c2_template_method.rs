//! # Template Method Pattern
//!
//! The Template Method pattern defines the skeleton of an algorithm in an
//! operation, deferring some steps to subclasses. Template Method lets
//! subclasses redefine certain steps without changing the algorithm's structure.
//!
//! ## Key Participants
//! - **AbstractClass**: Defines abstract primitive operations and template method
//! - **ConcreteClass**: Implements the primitive operations
//!
//! ## Benefits
//! - Maximizes code reuse
//! - Common behavior defined once in abstract class
//! - Subclasses only implement varying parts

// Abstract Class
pub trait DataProcessor {
    // Template method
    fn process_data(&self, input_data: &str) {
        println!("===== Starting Data Processing =====");
        self.initialize();
        let validated = self.validate_input(input_data);
        let result = self.perform_core_operation(&validated);
        self.finalize(&result);
        println!("===== Data Processing Complete =====\n");
    }

    // Primitive operations to be implemented by subclasses
    fn initialize(&self);
    fn perform_core_operation(&self, data: &str) -> String;

    // Hook with default implementation
    fn validate_input(&self, data: &str) -> String {
        println!("[Default Validation] Checking input...");
        if data.is_empty() {
            panic!("Input cannot be empty!");
        }
        println!("[Default Validation] Passed");
        data.to_string()
    }

    fn finalize(&self, result: &str);
}

// Concrete Class 1: Text Transformation Processor
pub struct TextTransformationProcessor;

impl DataProcessor for TextTransformationProcessor {
    fn initialize(&self) {
        println!("[TextProcessor] Initializing text transformation engine...");
    }

    fn perform_core_operation(&self, data: &str) -> String {
        println!("[TextProcessor] Transforming to uppercase...");
        format!("*** {} ***", data.to_uppercase())
    }

    fn validate_input(&self, data: &str) -> String {
        println!("[TextProcessor] Custom validation...");
        if data.is_empty() {
            panic!("Input cannot be empty");
        }
        let validated = data.to_string(); // Default behavior: return input as-is
        if validated.len() < 3 {
            panic!("Text too short!");
        }
        validated
    }

    fn finalize(&self, result: &str) {
        println!("[TextProcessor] Result: {}", result);
    }
}

// Concrete Class 2: Number Calculation Processor
pub struct NumberCalculationProcessor;

impl DataProcessor for NumberCalculationProcessor {
    fn initialize(&self) {
        println!("[NumberProcessor] Initializing calculation engine...");
    }

    fn perform_core_operation(&self, data: &str) -> String {
        println!("[NumberProcessor] Parsing numbers...");
        let numbers: Vec<i32> = data
            .split(',')
            .filter_map(|s| s.trim().parse().ok())
            .collect();

        let total: i32 = numbers.iter().sum();
        let avg = if numbers.is_empty() {
            0.0
        } else {
            total as f64 / numbers.len() as f64
        };

        format!("Sum={}, Avg={:.1}", total, avg)
    }

    fn finalize(&self, result: &str) {
        println!("[NumberProcessor] Result: {}", result);
    }
}

// Caffeine Beverage Example
pub trait CaffeineBeverage {
    // Template method
    fn prepare_recipe(&self) {
        self.boil_water();
        self.brew();
        self.pour_in_cup();
        self.add_condiments();
    }

    fn boil_water(&self) {
        println!("Boiling water...");
    }

    fn pour_in_cup(&self) {
        println!("Pouring into cup...");
    }

    // Primitive operations
    fn brew(&self);
    fn add_condiments(&self);
}

pub struct Coffee;

impl CaffeineBeverage for Coffee {
    fn brew(&self) {
        println!("Dripping coffee through filter...");
    }

    fn add_condiments(&self) {
        println!("Adding sugar and milk...");
    }
}

pub struct Tea;

impl CaffeineBeverage for Tea {
    fn brew(&self) {
        println!("Steeping the tea...");
    }

    fn add_condiments(&self) {
        println!("Adding lemon...");
    }
}

/// Example demonstrating Template Method pattern
pub fn example() {
    println!("\n--- Template Method Pattern Example ---\n");

    println!("1. Data Processing Pipeline");
    let text_proc = TextTransformationProcessor;
    text_proc.process_data("hello world");

    let num_proc = NumberCalculationProcessor;
    num_proc.process_data("10,20,30,40");

    println!("\n2. Caffeine Beverage Preparation");
    println!("\n   Preparing Coffee:");
    Coffee.prepare_recipe();

    println!("\n   Preparing Tea:");
    Tea.prepare_recipe();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_text_processor() {
        let proc = TextTransformationProcessor;
        proc.process_data("hello");
        // Should not panic
    }

    #[test]
    #[should_panic(expected = "Text too short")]
    fn test_text_processor_validation() {
        let proc = TextTransformationProcessor;
        proc.process_data("hi");
    }

    #[test]
    #[should_panic(expected = "Input cannot be empty")]
    fn test_empty_input() {
        let proc = TextTransformationProcessor;
        proc.process_data("");
    }

    #[test]
    fn test_number_processor() {
        let proc = NumberCalculationProcessor;
        proc.process_data("10,20,30");
        // Should not panic
    }

    #[test]
    fn test_coffee_preparation() {
        Coffee.prepare_recipe();
        // Should not panic
    }

    #[test]
    fn test_tea_preparation() {
        Tea.prepare_recipe();
        // Should not panic
    }
}
