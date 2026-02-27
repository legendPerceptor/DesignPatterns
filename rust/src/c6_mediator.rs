//! # Mediator Pattern
//!
//! The Mediator pattern defines an object that encapsulates how a set of objects
//! interact. Mediator promotes loose coupling by keeping objects from referring
//! to each other explicitly.
//!
//! ## Key Participants
//! - **Mediator**: Defines interface for communication with Colleagues
//! - **ConcreteMediator**: Implements cooperative behavior
//! - **Colleague**: Knows its Mediator and communicates with it
//!
//! ## Benefits
//! - Decouples colleagues
//! - Simplifies object protocols
//! - Centralizes control

use std::rc::Rc;
use std::cell::RefCell;

// Mediator trait
pub trait ATCMediator {
    fn register_aircraft(&mut self, aircraft: Aircraft);
    fn send_message(&self, message: &str, sender_name: &str);
    fn request_landing(&self, aircraft: &Aircraft);
    fn request_takeoff(&self, aircraft: &Aircraft);
}

// Colleague
pub struct Aircraft {
    name: String,
    mediator: Option<Rc<RefCell<dyn ATCMediator>>>,
    flying: bool,
    in_airspace: bool,
}

impl Aircraft {
    pub fn new(name: &str) -> Self {
        Self {
            name: name.to_string(),
            mediator: None,
            flying: false,
            in_airspace: true,
        }
    }

    pub fn set_mediator(&mut self, mediator: Rc<RefCell<dyn ATCMediator>>) {
        self.mediator = Some(mediator);
    }

    pub fn land(&self) {
        if self.flying {
            if let Some(ref med) = self.mediator {
                med.borrow().request_landing(self);
            }
        } else {
            println!("[{}] Already on the ground.", self.name);
        }
    }

    pub fn takeoff(&self) {
        if !self.flying && self.in_airspace {
            if let Some(ref med) = self.mediator {
                med.borrow().request_takeoff(self);
            }
        } else {
            println!("[{}] Cannot take off.", self.name);
        }
    }

    pub fn send_message(&self, message: &str) {
        println!("[{} sends] {}", self.name, message);
        if let Some(ref med) = self.mediator {
            med.borrow().send_message(message, &self.name);
        }
    }

    pub fn receive_message(&self, message: &str) {
        println!("[{} receives] {}", self.name, message);
    }

    pub fn set_flying(&mut self, flying: bool) {
        self.flying = flying;
        let status = if flying { "Taking off..." } else { "Landing..." };
        println!("[{}] {}", self.name, status);
    }

    pub fn get_name(&self) -> &str {
        &self.name
    }

    pub fn is_flying(&self) -> bool {
        self.flying
    }
}

// Concrete Mediator
pub struct ControlTower {
    aircraft: Vec<Aircraft>,
    runway_available: bool,
}

impl ControlTower {
    pub fn new() -> Self {
        Self {
            aircraft: Vec::new(),
            runway_available: true,
        }
    }
}

impl ATCMediator for ControlTower {
    fn register_aircraft(&mut self, aircraft: Aircraft) {
        println!("[ATC] {} registered in airspace.", aircraft.get_name());
        self.aircraft.push(aircraft);
    }

    fn send_message(&self, message: &str, sender_name: &str) {
        for aircraft in &self.aircraft {
            if aircraft.get_name() != sender_name {
                aircraft.receive_message(message);
            }
        }
    }

    fn request_landing(&self, aircraft: &Aircraft) {
        println!("[ATC] {} requests landing.", aircraft.get_name());

        if !self.runway_available {
            println!("[ATC] Runway occupied. {}, please hold.", aircraft.get_name());
            return;
        }

        for other in &self.aircraft {
            if other.get_name() != aircraft.get_name() && other.is_flying() {
                println!("[ATC] Traffic nearby. {}, maintain separation.", aircraft.get_name());
                return;
            }
        }

        println!("[ATC] {}, cleared to land runway 27L.", aircraft.get_name());
    }

    fn request_takeoff(&self, aircraft: &Aircraft) {
        println!("[ATC] {} requests takeoff.", aircraft.get_name());

        if !self.runway_available {
            println!("[ATC] Runway occupied. {}, hold short.", aircraft.get_name());
            return;
        }

        for other in &self.aircraft {
            if other.get_name() != aircraft.get_name() && other.is_flying() {
                println!("[ATC] Departing traffic. {}, wait.", aircraft.get_name());
                return;
            }
        }

        println!("[ATC] {}, cleared for takeoff runway 09R.", aircraft.get_name());
    }
}

impl Default for ControlTower {
    fn default() -> Self {
        Self::new()
    }
}

/// Example demonstrating Mediator pattern
pub fn example() {
    println!("\n--- Mediator Pattern Example ---\n");

    let tower = ControlTower::new();
    let tower_rc = Rc::new(RefCell::new(tower));

    // Create aircraft and set up mediator before registering
    let mut plane1 = Aircraft::new("Flight 123");
    let mut plane2 = Aircraft::new("Flight 456");
    let mut plane3 = Aircraft::new("Flight 789");

    plane1.set_mediator(tower_rc.clone());
    plane2.set_mediator(tower_rc.clone());
    plane3.set_mediator(tower_rc.clone());

    println!("\n--- Registration Test ---");
    tower_rc.borrow_mut().register_aircraft(plane1);
    tower_rc.borrow_mut().register_aircraft(plane2);
    tower_rc.borrow_mut().register_aircraft(plane3);
    println!("All aircraft registered with control tower.");

    // Note: After registration, aircraft are owned by the tower
    // In a real application, you'd use Rc or similar for shared ownership
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_aircraft_creation() {
        let aircraft = Aircraft::new("Test");
        assert_eq!(aircraft.get_name(), "Test");
    }

    #[test]
    fn test_control_tower() {
        let tower = ControlTower::new();
        // Should not panic
    }
}
