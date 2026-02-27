//! # Decorator Pattern
//!
//! The Decorator pattern attaches additional responsibilities to an object
//! dynamically. Decorators provide a flexible alternative to subclassing for
//! extending functionality.
//!
//! ## Key Participants
//! - **Component**: Defines interface for objects that can have responsibilities
//! - **ConcreteComponent**: Defines object to which additional responsibilities can be attached
//! - **Decorator**: Maintains reference to Component and implements Component interface
//! - **ConcreteDecorator**: Adds responsibilities to the component
//!
//! ## Benefits
//! - More flexible than static inheritance
//! - Avoids feature-laden classes high up in hierarchy
//! - Simplifies adding responsibilities

// Component trait
pub trait Beverage {
    fn description(&self) -> String;
    fn cost(&self) -> f64;
}

// Concrete Component
pub struct Espresso;

impl Beverage for Espresso {
    fn description(&self) -> String {
        "Espresso".to_string()
    }

    fn cost(&self) -> f64 {
        1.99
    }
}

pub struct DarkRoast;

impl Beverage for DarkRoast {
    fn description(&self) -> String {
        "Dark Roast Coffee".to_string()
    }

    fn cost(&self) -> f64 {
        0.99
    }
}

pub struct HouseBlend;

impl Beverage for HouseBlend {
    fn description(&self) -> String {
        "House Blend Coffee".to_string()
    }

    fn cost(&self) -> f64 {
        0.89
    }
}

// Decorator
pub struct CondimentDecorator {
    beverage: Box<dyn Beverage>,
    description_addition: String,
    cost_addition: f64,
}

impl CondimentDecorator {
    pub fn new(beverage: Box<dyn Beverage>, description: &str, cost: f64) -> Self {
        Self {
            beverage,
            description_addition: format!(", {}", description),
            cost_addition: cost,
        }
    }
}

impl Beverage for CondimentDecorator {
    fn description(&self) -> String {
        format!("{}{}", self.beverage.description(), self.description_addition)
    }

    fn cost(&self) -> f64 {
        self.beverage.cost() + self.cost_addition
    }
}

// Specific Condiment Decorators
pub struct Mocha {
    beverage: Box<dyn Beverage>,
}

impl Mocha {
    pub fn new(beverage: Box<dyn Beverage>) -> Self {
        Self { beverage }
    }
}

impl Beverage for Mocha {
    fn description(&self) -> String {
        format!("{}, Mocha", self.beverage.description())
    }

    fn cost(&self) -> f64 {
        self.beverage.cost() + 0.20
    }
}

pub struct Whip {
    beverage: Box<dyn Beverage>,
}

impl Whip {
    pub fn new(beverage: Box<dyn Beverage>) -> Self {
        Self { beverage }
    }
}

impl Beverage for Whip {
    fn description(&self) -> String {
        format!("{}, Whip", self.beverage.description())
    }

    fn cost(&self) -> f64 {
        self.beverage.cost() + 0.10
    }
}

pub struct Milk {
    beverage: Box<dyn Beverage>,
}

impl Milk {
    pub fn new(beverage: Box<dyn Beverage>) -> Self {
        Self { beverage }
    }
}

impl Beverage for Milk {
    fn description(&self) -> String {
        format!("{}, Milk", self.beverage.description())
    }

    fn cost(&self) -> f64 {
        self.beverage.cost() + 0.10
    }
}

pub struct Soy {
    beverage: Box<dyn Beverage>,
}

impl Soy {
    pub fn new(beverage: Box<dyn Beverage>) -> Self {
        Self { beverage }
    }
}

impl Beverage for Soy {
    fn description(&self) -> String {
        format!("{}, Soy", self.beverage.description())
    }

    fn cost(&self) -> f64 {
        self.beverage.cost() + 0.15
    }
}

/// Example demonstrating Decorator pattern
pub fn example() {
    println!("\n--- Decorator Pattern Example ---\n");

    println!("Ordering beverages with various toppings:\n");

    println!("1. Simple Espresso:");
    let espresso = Box::new(Espresso);
    println!("  {} ${:.2}", espresso.description(), espresso.cost());

    println!("\n2. Dark Roast + Mocha + Mocha + Whip:");
    let mut beverage: Box<dyn Beverage> = Box::new(DarkRoast);
    beverage = Box::new(Mocha::new(beverage));
    beverage = Box::new(Mocha::new(beverage));
    beverage = Box::new(Whip::new(beverage));
    println!("  {} ${:.2}", beverage.description(), beverage.cost());

    println!("\n3. House Blend + Soy + Mocha + Whip:");
    let mut beverage: Box<dyn Beverage> = Box::new(HouseBlend);
    beverage = Box::new(Soy::new(beverage));
    beverage = Box::new(Mocha::new(beverage));
    beverage = Box::new(Whip::new(beverage));
    println!("  {} ${:.2}", beverage.description(), beverage.cost());
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_plain_espresso() {
        let espresso = Espresso;
        assert_eq!(espresso.description(), "Espresso");
        assert_eq!(espresso.cost(), 1.99);
    }

    #[test]
    fn test_single_mocha_decorator() {
        let beverage = Box::new(Mocha::new(Box::new(Espresso)));
        assert!(beverage.description().contains("Mocha"));
        assert!((beverage.cost() - 2.19).abs() < 0.01); // 1.99 + 0.20
    }

    #[test]
    fn test_multiple_decorators() {
        let beverage: Box<dyn Beverage> = Box::new(Whip::new(
            Box::new(Mocha::new(
                Box::new(Mocha::new(Box::new(DarkRoast)))
            ))
        ));

        let desc = beverage.description();
        assert!(desc.contains("Dark Roast"));
        assert!(desc.contains("Mocha"));
        assert!(desc.contains("Whip"));

        // 0.99 + 0.20 + 0.20 + 0.10 = 1.49
        assert!((beverage.cost() - 1.49).abs() < 0.01);
    }

    #[test]
    fn test_decorator_preserves_base_description() {
        let beverage = Box::new(Milk::new(Box::new(Espresso)));
        assert!(beverage.description().contains("Espresso"));
        assert!(beverage.description().contains("Milk"));
    }

    #[test]
    fn test_decorator_costs_accumulate() {
        let beverage: Box<dyn Beverage> = Box::new(Soy::new(
            Box::new(Milk::new(Box::new(HouseBlend)))
        ));

        // 0.89 + 0.15 + 0.10 = 1.14
        assert!((beverage.cost() - 1.14).abs() < 0.01);
    }
}
