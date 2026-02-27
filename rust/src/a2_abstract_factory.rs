//! # Abstract Factory Pattern
//!
//! The Abstract Factory pattern provides an interface for creating families
//! of related objects without specifying their concrete classes.
//!
//! ## Key Participants
//! - **AbstractFactory**: Declares interface for creating abstract products
//! - **ConcreteFactory**: Implements operations to create concrete products
//! - **AbstractProduct**: Declares interface for a type of product
//! - **ConcreteProduct**: Defines product object to be created by concrete factory
//!
//! ## Benefits
//! - Isolates client classes from concrete classes
//! - Easy to exchange product families
//! - Promotes consistency among products

use std::rc::Rc;

// Abstract Products
pub trait Button {
    fn paint(&self);
}

pub trait Checkbox {
    fn paint(&self);
}

// Concrete Products for Windows
pub struct WindowsButton;

impl Button for WindowsButton {
    fn paint(&self) {
        println!("[Windows] Rendering button");
    }
}

pub struct WindowsCheckbox;

impl Checkbox for WindowsCheckbox {
    fn paint(&self) {
        println!("[Windows] Rendering checkbox");
    }
}

// Concrete Products for Mac
pub struct MacButton;

impl Button for MacButton {
    fn paint(&self) {
        println!("[Mac] Rendering button");
    }
}

pub struct MacCheckbox;

impl Checkbox for MacCheckbox {
    fn paint(&self) {
        println!("[Mac] Rendering checkbox");
    }
}

// Abstract Factory
pub trait GUIFactory {
    fn create_button(&self) -> Rc<dyn Button>;
    fn create_checkbox(&self) -> Rc<dyn Checkbox>;
}

// Concrete Factories
pub struct WindowsFactory;

impl GUIFactory for WindowsFactory {
    fn create_button(&self) -> Rc<dyn Button> {
        Rc::new(WindowsButton)
    }

    fn create_checkbox(&self) -> Rc<dyn Checkbox> {
        Rc::new(WindowsCheckbox)
    }
}

pub struct MacFactory;

impl GUIFactory for MacFactory {
    fn create_button(&self) -> Rc<dyn Button> {
        Rc::new(MacButton)
    }

    fn create_checkbox(&self) -> Rc<dyn Checkbox> {
        Rc::new(MacCheckbox)
    }
}

// Client
pub struct Application {
    factory: Rc<dyn GUIFactory>,
}

impl Application {
    pub fn new(factory: Rc<dyn GUIFactory>) -> Self {
        Self { factory }
    }

    pub fn create_ui(&self) {
        let button = self.factory.create_button();
        let checkbox = self.factory.create_checkbox();

        button.paint();
        checkbox.paint();
    }
}

/// Example demonstrating Abstract Factory pattern
pub fn example() {
    println!("\n--- Abstract Factory Pattern Example ---\n");

    println!("1. Windows Application:");
    let windows_factory = Rc::new(WindowsFactory);
    let windows_app = Application::new(windows_factory);
    windows_app.create_ui();

    println!("\n2. Mac Application:");
    let mac_factory = Rc::new(MacFactory);
    let mac_app = Application::new(mac_factory);
    mac_app.create_ui();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_windows_factory_creates_windows_widgets() {
        let factory = WindowsFactory;
        let button = factory.create_button();
        let checkbox = factory.create_checkbox();

        // Type checking through dynamic dispatch
        button.paint();
        checkbox.paint();
    }

    #[test]
    fn test_mac_factory_creates_mac_widgets() {
        let factory = MacFactory;
        let button = factory.create_button();
        let checkbox = factory.create_checkbox();

        button.paint();
        checkbox.paint();
    }

    #[test]
    fn test_application_with_windows_factory() {
        let factory = Rc::new(WindowsFactory);
        let app = Application::new(factory);
        app.create_ui(); // Should not panic
    }

    #[test]
    fn test_application_with_mac_factory() {
        let factory = Rc::new(MacFactory);
        let app = Application::new(factory);
        app.create_ui(); // Should not panic
    }
}
