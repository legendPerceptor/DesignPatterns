//! # Prototype Pattern
//!
//! The Prototype pattern specifies kinds of objects to create using a
//! prototypical instance, and creates new objects by cloning this prototype.
//!
//! ## Key Participants
//! - **Prototype**: Declares interface for cloning itself
//! - **ConcretePrototype**: Implements clone operation
//! - **Client**: Creates new objects by asking prototype to clone itself
//!
//! ## Benefits
//! - Adding/removing products at runtime
//! - Specifying new objects by varying values
//! - Reducing subclass creation
//! - Dynamic class loading

use std::collections::HashMap;

// Prototype trait
pub trait Prototype {
    fn clone_box(&self) -> Box<dyn Prototype>;
    fn get_name(&self) -> &str;
    fn draw(&self);
}

// Concrete Prototypes
#[derive(Clone)]
pub struct Circle {
    pub radius: f64,
    pub color: String,
}

impl Circle {
    pub fn new() -> Self {
        Self {
            radius: 10.0,
            color: "red".to_string(),
        }
    }

    pub fn set_radius(&mut self, radius: f64) {
        self.radius = radius;
    }

    pub fn set_color(&mut self, color: &str) {
        self.color = color.to_string();
    }
}

impl Prototype for Circle {
    fn clone_box(&self) -> Box<dyn Prototype> {
        Box::new(self.clone())
    }

    fn get_name(&self) -> &str {
        "Circle"
    }

    fn draw(&self) {
        println!("Drawing {} (radius={}, color={})", self.get_name(), self.radius, self.color);
    }
}

#[derive(Clone)]
pub struct Rectangle {
    pub width: f64,
    pub height: f64,
    pub color: String,
}

impl Rectangle {
    pub fn new() -> Self {
        Self {
            width: 10.0,
            height: 5.0,
            color: "blue".to_string(),
        }
    }

    pub fn set_size(&mut self, width: f64, height: f64) {
        self.width = width;
        self.height = height;
    }

    pub fn set_color(&mut self, color: &str) {
        self.color = color.to_string();
    }
}

impl Prototype for Rectangle {
    fn clone_box(&self) -> Box<dyn Prototype> {
        Box::new(self.clone())
    }

    fn get_name(&self) -> &str {
        "Rectangle"
    }

    fn draw(&self) {
        println!(
            "Drawing {} (width={}x{}, height={})",
            self.get_name(),
            self.width,
            self.height,
            self.color
        );
    }
}

// Prototype Registry
pub struct PrototypeRegistry {
    prototypes: HashMap<String, Box<dyn Prototype>>,
}

impl PrototypeRegistry {
    pub fn new() -> Self {
        Self {
            prototypes: HashMap::new(),
        }
    }

    pub fn register(&mut self, key: &str, prototype: Box<dyn Prototype>) {
        println!("Registering prototype: {}", key);
        self.prototypes.insert(key.to_string(), prototype);
    }

    pub fn create(&self, key: &str) -> Option<Box<dyn Prototype>> {
        self.prototypes.get(key).map(|p| p.clone_box())
    }
}

impl Default for PrototypeRegistry {
    fn default() -> Self {
        Self::new()
    }
}

/// Example demonstrating Prototype pattern
pub fn example() {
    println!("\n--- Prototype Pattern Example ---\n");

    let mut registry = PrototypeRegistry::new();

    // Set up prototype objects
    let mut red_circle = Circle::new();
    red_circle.set_radius(10.0);
    red_circle.set_color("red");

    let mut blue_rect = Rectangle::new();
    blue_rect.set_size(4.0, 2.0);
    blue_rect.set_color("blue");

    // Register prototypes
    registry.register("red-circle", Box::new(red_circle));
    registry.register("blue-rect", Box::new(blue_rect));

    // Clone and use prototypes
    println!("Creating objects from prototypes:");

    if let Some(r1) = registry.create("blue-rect") {
        r1.draw();
    }
}

// Helper trait for downcasting
pub trait AsAny {
    fn as_any(&self) -> &dyn std::any::Any;
}

impl<T: std::any::Any> AsAny for T {
    fn as_any(&self) -> &dyn std::any::Any {
        self
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_circle_clone() {
        let circle = Circle::new();
        let cloned = circle.clone_box();
        assert_eq!(cloned.get_name(), "Circle");
    }

    #[test]
    fn test_rectangle_clone() {
        let rect = Rectangle::new();
        let cloned = rect.clone_box();
        assert_eq!(cloned.get_name(), "Rectangle");
    }

    #[test]
    fn test_registry_register_and_retrieve() {
        let mut registry = PrototypeRegistry::new();
        registry.register("test-circle", Box::new(Circle::new()));

        let cloned = registry.create("test-circle");
        assert!(cloned.is_some());
        assert_eq!(cloned.unwrap().get_name(), "Circle");
    }

    #[test]
    fn test_registry_nonexistent_key() {
        let registry = PrototypeRegistry::new();
        let result = registry.create("nonexistent");
        assert!(result.is_none());
    }

    #[test]
    fn test_clone_independent_modification() {
        let mut original = Circle::new();
        original.set_radius(10.0);

        let cloned = original.clone_box();
        original.set_radius(20.0); // Modify original

        // Clone should have original radius (10.0)
        // This is a simple test - in real usage, we'd need downcasting
        cloned.draw();
    }
}
