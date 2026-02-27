//! # Strategy Pattern
//!
//! The Strategy pattern defines a family of algorithms, encapsulates each one,
//! and makes them interchangeable. Strategy lets the algorithm vary independently
//! from clients that use it.
//!
//! ## Key Participants
//! - **Strategy**: Declares interface common to all supported algorithms
//! - **ConcreteStrategy**: Implements algorithm using Strategy interface
//! - **Context**: Configured with ConcreteStrategy
//!
//! ## Benefits
//! - Families of related algorithms defined
//! - Alternative strategies swapped at runtime
//! - Eliminates conditional statements

// Payment Strategy Example
pub trait PaymentStrategy {
    fn pay(&self, amount: f64);
    fn get_name(&self) -> &str;
}

pub struct CreditCardPayment {
    card_number: String,
    name: String,
}

impl CreditCardPayment {
    pub fn new(card_number: &str, name: &str) -> Self {
        Self {
            card_number: card_number.to_string(),
            name: name.to_string(),
        }
    }
}

impl PaymentStrategy for CreditCardPayment {
    fn pay(&self, amount: f64) {
        let last_four = &self.card_number[self.card_number.len() - 4..];
        println!("[Credit Card] Paying ${} using card ending in {}", amount, last_four);
        println!("[Credit Card] Cardholder: {}", self.name);
        println!("[Credit Card] Transaction approved.");
    }

    fn get_name(&self) -> &str {
        "Credit Card"
    }
}

pub struct PayPalPayment {
    email: String,
}

impl PayPalPayment {
    pub fn new(email: &str) -> Self {
        Self {
            email: email.to_string(),
        }
    }
}

impl PaymentStrategy for PayPalPayment {
    fn pay(&self, amount: f64) {
        println!("[PayPal] Paying ${} using PayPal account", amount);
        println!("[PayPal] Account: {}", self.email);
        println!("[PayPal] Payment completed successfully.");
    }

    fn get_name(&self) -> &str {
        "PayPal"
    }
}

// Context
pub struct ShoppingCart {
    payment_strategy: Option<Box<dyn PaymentStrategy>>,
}

impl ShoppingCart {
    pub fn new() -> Self {
        Self {
            payment_strategy: None,
        }
    }

    pub fn set_payment_strategy(&mut self, strategy: Box<dyn PaymentStrategy>) {
        println!(
            "[Shopping Cart] Payment method set to: {}",
            strategy.get_name()
        );
        self.payment_strategy = Some(strategy);
    }

    pub fn checkout(&self, total: f64) {
        println!("\n[Shopping Cart] Starting checkout...");
        println!("[Shopping Cart] Total amount: ${}", total);
        if let Some(ref strategy) = self.payment_strategy {
            strategy.pay(total);
        } else {
            println!("[Shopping Cart] ERROR: No payment method selected!");
        }
        println!("[Shopping Cart] Checkout complete.\n");
    }
}

impl Default for ShoppingCart {
    fn default() -> Self {
        Self::new()
    }
}

// Route Strategy Example
pub trait RouteStrategy {
    fn build_route(&self, start: &str, end: &str);
    fn get_strategy_name(&self) -> &str;
}

pub struct FastestRouteStrategy;

impl RouteStrategy for FastestRouteStrategy {
    fn build_route(&self, start: &str, end: &str) {
        println!(
            "[Fastest Route] Calculating route from {} to {}",
            start, end
        );
        println!("[Fastest Route] Prioritizing highways...");
        println!("[Fastest Route] Real-time traffic analysis...");
        println!("[Fastest Route] Take Highway 101, then I-95 to {}", end);
        println!("[Fastest Route] Estimated time: 45 minutes");
    }

    fn get_strategy_name(&self) -> &str {
        "Fastest Route"
    }
}

pub struct ShortestRouteStrategy;

impl RouteStrategy for ShortestRouteStrategy {
    fn build_route(&self, start: &str, end: &str) {
        println!(
            "[Shortest Route] Calculating route from {} to {}",
            start, end
        );
        println!("[Shortest Route] Finding minimum distance...");
        println!("[Shortest Route] Take local roads to {}", end);
        println!("[Shortest Route] Distance: 18.5 miles");
    }

    fn get_strategy_name(&self) -> &str {
        "Shortest Distance"
    }
}

// Navigation System
pub struct NavigationSystem {
    user_name: String,
    route_strategy: Option<Box<dyn RouteStrategy>>,
}

impl NavigationSystem {
    pub fn new(user_name: &str) -> Self {
        Self {
            user_name: user_name.to_string(),
            route_strategy: None,
        }
    }

    pub fn set_route_strategy(&mut self, strategy: Box<dyn RouteStrategy>) {
        self.route_strategy = Some(strategy);
    }

    pub fn navigate(&self, start: &str, end: &str) {
        println!(
            "\n[Navigation System] User: {}",
            self.user_name
        );
        if let Some(ref strategy) = self.route_strategy {
            println!(
                "[Navigation System] Strategy: {}",
                strategy.get_strategy_name()
            );
        } else {
            println!("[Navigation System] Strategy: None");
        }
        println!("[Navigation System] Calculating route...\n");

        if let Some(ref strategy) = self.route_strategy {
            strategy.build_route(start, end);
            println!("\n[Navigation System] Route ready!\n");
        }
    }
}

/// Example demonstrating Strategy pattern
pub fn example() {
    println!("\n--- Strategy Pattern Example ---\n");

    // Payment Strategy Example
    println!("\n1. Payment Processing");
    let mut cart = ShoppingCart::new();

    cart.set_payment_strategy(Box::new(CreditCardPayment::new(
        "1234567890123456",
        "John Doe",
    )));
    cart.checkout(99.99);

    cart.set_payment_strategy(Box::new(PayPalPayment::new(
        "john@example.com",
    )));
    cart.checkout(49.99);

    // Route Strategy Example
    println!("\n2. Navigation System");
    let mut nav = NavigationSystem::new("Alice");

    nav.set_route_strategy(Box::new(FastestRouteStrategy));
    nav.navigate("New York", "Boston");

    nav.set_route_strategy(Box::new(ShortestRouteStrategy));
    nav.navigate("New York", "Boston");
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_shopping_cart() {
        let cart = ShoppingCart::new();
        // Should not panic
    }

    #[test]
    fn test_credit_card_payment() {
        let payment = CreditCardPayment::new("1234567890123456", "John Doe");
        payment.pay(99.99);
        // Should not panic
    }

    #[test]
    fn test_paypal_payment() {
        let payment = PayPalPayment::new("john@example.com");
        payment.pay(49.99);
        // Should not panic
    }

    #[test]
    fn test_navigation_system() {
        let nav = NavigationSystem::new("Test");
        // Should not panic
    }

    #[test]
    fn test_strategy_swapping() {
        let mut nav = NavigationSystem::new("Test");
        nav.set_route_strategy(Box::new(FastestRouteStrategy));
        nav.set_route_strategy(Box::new(ShortestRouteStrategy));
        // Should not panic
    }
}
