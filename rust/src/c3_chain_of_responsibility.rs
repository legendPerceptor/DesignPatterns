//! # Chain of Responsibility Pattern
//!
//! The Chain of Responsibility pattern avoids coupling the sender of a request
//! to its receiver by giving more than one object a chance to handle the request.
//!
//! ## Key Participants
//! - **Handler**: Defines interface for handling requests and successor link
//! - **ConcreteHandler**: Handles requests it's responsible for; accesses successor
//! - **Client**: Initiates the request
//!
//! ## Benefits
//! - Decouples sender from receiver
//! - Simplifies object interconnections
//! - Flexible responsibility assignment

use std::cell::RefCell;
use std::rc::Rc;

// Request
#[derive(Debug, Clone, PartialEq)]
pub enum Level {
    Basic,
    Intermediate,
    Advanced,
    Critical,
}

#[derive(Debug, Clone)]
pub struct SupportTicket {
    pub description: String,
    pub level: Level,
}

impl SupportTicket {
    pub fn new(description: &str, level: Level) -> Self {
        Self {
            description: description.to_string(),
            level,
        }
    }

    pub fn get_level_string(&self) -> &str {
        match self.level {
            Level::Basic => "Basic",
            Level::Intermediate => "Intermediate",
            Level::Advanced => "Advanced",
            Level::Critical => "Critical",
        }
    }
}

// Handler trait
pub trait SupportHandler {
    fn set_next(&mut self, handler: Rc<RefCell<dyn SupportHandler>>);
    fn handle_ticket(&self, ticket: &SupportTicket);
    fn can_handle(&self, ticket: &SupportTicket) -> bool;
    fn do_handle(&self, ticket: &SupportTicket);
}

// Concrete Handlers
pub struct Level1Support {
    next: RefCell<Option<Rc<RefCell<dyn SupportHandler>>>>,
}

impl Level1Support {
    pub fn new() -> Self {
        Self {
            next: RefCell::new(None),
        }
    }
}

impl SupportHandler for Level1Support {
    fn set_next(&mut self, handler: Rc<RefCell<dyn SupportHandler>>) {
        *self.next.borrow_mut() = Some(handler);
    }

    fn handle_ticket(&self, ticket: &SupportTicket) {
        if self.can_handle(ticket) {
            self.do_handle(ticket);
        } else if let Some(next) = self.next.borrow().as_ref() {
            println!("Level1Support escalating...");
            next.borrow().handle_ticket(ticket);
        } else {
            println!("No handler available for this ticket.");
        }
    }

    fn can_handle(&self, ticket: &SupportTicket) -> bool {
        ticket.level == Level::Basic
    }

    fn do_handle(&self, ticket: &SupportTicket) {
        println!(
            "[Level 1 Support] Handling: {}",
            ticket.description
        );
        println!("  Issue resolved with basic troubleshooting.");
    }
}

pub struct Level2Support {
    next: RefCell<Option<Rc<RefCell<dyn SupportHandler>>>>,
}

impl Level2Support {
    pub fn new() -> Self {
        Self {
            next: RefCell::new(None),
        }
    }
}

impl SupportHandler for Level2Support {
    fn set_next(&mut self, handler: Rc<RefCell<dyn SupportHandler>>) {
        *self.next.borrow_mut() = Some(handler);
    }

    fn handle_ticket(&self, ticket: &SupportTicket) {
        if self.can_handle(ticket) {
            self.do_handle(ticket);
        } else if let Some(next) = self.next.borrow().as_ref() {
            println!("Level2Support escalating...");
            next.borrow().handle_ticket(ticket);
        } else {
            println!("No handler available for this ticket.");
        }
    }

    fn can_handle(&self, ticket: &SupportTicket) -> bool {
        ticket.level == Level::Intermediate
    }

    fn do_handle(&self, ticket: &SupportTicket) {
        println!(
            "[Level 2 Support] Handling: {}",
            ticket.description
        );
        println!("  Issue resolved with technical expertise.");
    }
}

pub struct Manager {
    next: RefCell<Option<Rc<RefCell<dyn SupportHandler>>>>,
}

impl Manager {
    pub fn new() -> Self {
        Self {
            next: RefCell::new(None),
        }
    }
}

impl SupportHandler for Manager {
    fn set_next(&mut self, handler: Rc<RefCell<dyn SupportHandler>>) {
        *self.next.borrow_mut() = Some(handler);
    }

    fn handle_ticket(&self, ticket: &SupportTicket) {
        if self.can_handle(ticket) {
            self.do_handle(ticket);
        } else if let Some(next) = self.next.borrow().as_ref() {
            println!("Manager escalating...");
            next.borrow().handle_ticket(ticket);
        } else {
            println!("No handler available for this ticket.");
        }
    }

    fn can_handle(&self, ticket: &SupportTicket) -> bool {
        ticket.level == Level::Advanced
    }

    fn do_handle(&self, ticket: &SupportTicket) {
        println!("[Manager] Handling: {}", ticket.description);
        println!("  Issue resolved with management authority.");
    }
}

pub struct Director {
    next: RefCell<Option<Rc<RefCell<dyn SupportHandler>>>>,
}

impl Director {
    pub fn new() -> Self {
        Self {
            next: RefCell::new(None),
        }
    }
}

impl SupportHandler for Director {
    fn set_next(&mut self, handler: Rc<RefCell<dyn SupportHandler>>) {
        *self.next.borrow_mut() = Some(handler);
    }

    fn handle_ticket(&self, ticket: &SupportTicket) {
        if self.can_handle(ticket) {
            self.do_handle(ticket);
        } else if let Some(next) = self.next.borrow().as_ref() {
            println!("Director escalating...");
            next.borrow().handle_ticket(ticket);
        } else {
            println!("No handler available for this ticket.");
        }
    }

    fn can_handle(&self, ticket: &SupportTicket) -> bool {
        ticket.level == Level::Critical
    }

    fn do_handle(&self, ticket: &SupportTicket) {
        println!("[Director] Handling: {}", ticket.description);
        println!("  Issue resolved at highest level.");
    }
}

/// Example demonstrating Chain of Responsibility pattern
pub fn example() {
    println!("\n--- Chain of Responsibility Pattern Example ---\n");

    // Build the chain
    let level1 = Rc::new(RefCell::new(Level1Support::new()));
    let level2 = Rc::new(RefCell::new(Level2Support::new()));
    let manager = Rc::new(RefCell::new(Manager::new()));
    let director = Rc::new(RefCell::new(Director::new()));

    // Set up chain
    level1.borrow_mut().set_next(level2.clone());
    level2.borrow_mut().set_next(manager.clone());
    manager.borrow_mut().set_next(director.clone());

    // Create tickets
    let tickets = vec![
        SupportTicket::new("Can't connect to WiFi", Level::Basic),
        SupportTicket::new("Server crash", Level::Intermediate),
        SupportTicket::new("Datacenter outage", Level::Critical),
        SupportTicket::new("Budget approval needed", Level::Advanced),
    ];

    for ticket in tickets {
        println!(
            "\nProcessing: {} ({})",
            ticket.description,
            ticket.get_level_string()
        );
        level1.borrow().handle_ticket(&ticket);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_basic_ticket_handled_by_level1() {
        let level1 = Rc::new(RefCell::new(Level1Support::new()));
        let level2 = Rc::new(RefCell::new(Level2Support::new()));
        level1.borrow_mut().set_next(level2);

        let ticket = SupportTicket::new("WiFi issue", Level::Basic);
        level1.borrow().handle_ticket(&ticket);
        // Should not panic
    }

    #[test]
    fn test_intermediate_ticket_escalates() {
        let level1 = Rc::new(RefCell::new(Level1Support::new()));
        let level2 = Rc::new(RefCell::new(Level2Support::new()));
        level1.borrow_mut().set_next(level2);

        let ticket = SupportTicket::new("Server crash", Level::Intermediate);
        level1.borrow().handle_ticket(&ticket);
        // Should not panic
    }

    #[test]
    fn test_advanced_ticket_handled_by_manager() {
        let level1 = Rc::new(RefCell::new(Level1Support::new()));
        let manager = Rc::new(RefCell::new(Manager::new()));
        level1.borrow_mut().set_next(manager);

        let ticket = SupportTicket::new("Budget needed", Level::Advanced);
        level1.borrow().handle_ticket(&ticket);
        // Should not panic
    }

    #[test]
    fn test_ticket_get_level_string() {
        let ticket = SupportTicket::new("Test", Level::Basic);
        assert_eq!(ticket.get_level_string(), "Basic");
    }
}
