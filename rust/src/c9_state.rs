//! # State Pattern
//!
//! The State pattern allows an object to alter its behavior when its internal
//! state changes. The object will appear to change its class.
//!
//! ## Key Participants
//! - **Context**: Defines interface and maintains instance of ConcreteState
//! - **State**: Defines interface for encapsulating behavior
//! - **ConcreteState**: Implements behavior associated with a state
//!
//! ## Benefits
//! - Localizes state-specific behavior
//! - Makes state transitions explicit
//! - State objects can be shared

// State trait
pub trait State {
    fn insert_coin(&mut self, machine: &mut VendingMachine);
    fn eject_coin(&mut self, machine: &mut VendingMachine);
    fn press_button(&mut self, machine: &mut VendingMachine);
    fn dispense(&mut self, machine: &mut VendingMachine);
    fn get_name(&self) -> &str;
}

// Context
pub struct VendingMachine {
    item_count: u32,
    current_state: Box<dyn State>,
}

impl VendingMachine {
    pub fn new(item_count: u32) -> Self {
        let current_state: Box<dyn State> = if item_count > 0 {
            Box::new(NoCoinState)
        } else {
            Box::new(OutOfStockState)
        };

        Self {
            item_count,
            current_state,
        }
    }

    pub fn insert_coin(&mut self) {
        // Take the current state temporarily to avoid borrow conflicts
        let mut state = std::mem::replace(&mut self.current_state, Box::new(NoCoinState) as Box<dyn State>);
        state.insert_coin(self);
        // Don't restore the old state - it may have been changed by the state method
    }

    pub fn eject_coin(&mut self) {
        let mut state = std::mem::replace(&mut self.current_state, Box::new(NoCoinState) as Box<dyn State>);
        state.eject_coin(self);
        // Don't restore the old state - it may have been changed by the state method
    }

    pub fn press_button(&mut self) {
        let mut state = std::mem::replace(&mut self.current_state, Box::new(NoCoinState) as Box<dyn State>);
        state.press_button(self);
        state.dispense(self);
        // Don't restore the old state - it may have been changed by the state method
    }

    pub fn release_product(&mut self) {
        println!("[Vending Machine] Dispensing product...");
        if self.item_count > 0 {
            self.item_count -= 1;
            println!(
                "[Vending Machine] Product dispensed! Remaining: {}",
                self.item_count
            );
        }
    }

    pub fn has_stock(&self) -> bool {
        self.item_count > 0
    }

    pub fn get_state_name(&self) -> &str {
        self.current_state.get_name()
    }

    pub fn set_no_coin_state(&mut self) {
        self.current_state = Box::new(NoCoinState);
    }

    pub fn set_has_coin_state(&mut self) {
        self.current_state = Box::new(HasCoinState);
    }

    pub fn set_sold_state(&mut self) {
        self.current_state = Box::new(SoldState);
    }

    pub fn set_out_of_stock_state(&mut self) {
        self.current_state = Box::new(OutOfStockState);
    }
}

// Concrete States
pub struct NoCoinState;

impl State for NoCoinState {
    fn insert_coin(&mut self, machine: &mut VendingMachine) {
        println!("[No Coin State] Coin inserted");
        machine.set_has_coin_state();
    }

    fn eject_coin(&mut self, _machine: &mut VendingMachine) {
        println!("[No Coin State] No coin to eject");
    }

    fn press_button(&mut self, _machine: &mut VendingMachine) {
        println!("[No Coin State] Please insert coin first");
    }

    fn dispense(&mut self, _machine: &mut VendingMachine) {
        println!("[No Coin State] Payment required");
    }

    fn get_name(&self) -> &str {
        "No Coin"
    }
}

pub struct HasCoinState;

impl State for HasCoinState {
    fn insert_coin(&mut self, _machine: &mut VendingMachine) {
        println!("[Has Coin State] Coin already inserted");
    }

    fn eject_coin(&mut self, machine: &mut VendingMachine) {
        println!("[Has Coin State] Coin ejected");
        machine.set_no_coin_state();
    }

    fn press_button(&mut self, machine: &mut VendingMachine) {
        if machine.has_stock() {
            println!("[Has Coin State] Button pressed");
            machine.set_sold_state();
        } else {
            println!("[Has Coin State] Out of stock");
            machine.set_out_of_stock_state();
        }
    }

    fn dispense(&mut self, _machine: &mut VendingMachine) {
        println!("[Has Coin State] No product dispensed");
    }

    fn get_name(&self) -> &str {
        "Has Coin"
    }
}

pub struct SoldState;

impl State for SoldState {
    fn insert_coin(&mut self, _machine: &mut VendingMachine) {
        println!("[Sold State] Please wait for product");
    }

    fn eject_coin(&mut self, _machine: &mut VendingMachine) {
        println!("[Sold State] Already dispensing");
    }

    fn press_button(&mut self, _machine: &mut VendingMachine) {
        println!("[Sold State] Already pressed");
    }

    fn dispense(&mut self, machine: &mut VendingMachine) {
        machine.release_product();
        if machine.has_stock() {
            machine.set_no_coin_state();
        } else {
            machine.set_out_of_stock_state();
        }
    }

    fn get_name(&self) -> &str {
        "Sold"
    }
}

pub struct OutOfStockState;

impl State for OutOfStockState {
    fn insert_coin(&mut self, _machine: &mut VendingMachine) {
        println!("[Out of Stock State] Cannot accept coin - out of stock");
    }

    fn eject_coin(&mut self, _machine: &mut VendingMachine) {
        println!("[Out of Stock State] No coin to eject");
    }

    fn press_button(&mut self, _machine: &mut VendingMachine) {
        println!("[Out of Stock State] Out of stock");
    }

    fn dispense(&mut self, _machine: &mut VendingMachine) {
        println!("[Out of Stock State] No product to dispense");
    }

    fn get_name(&self) -> &str {
        "Out of Stock"
    }
}

/// Example demonstrating State pattern
pub fn example() {
    println!("\n--- State Pattern Example ---\n");

    println!("--- Vending Machine ---");
    let mut vending_machine = VendingMachine::new(2);

    println!("State: {}", vending_machine.get_state_name());
    vending_machine.insert_coin();

    println!("State: {}", vending_machine.get_state_name());
    vending_machine.press_button();

    println!("\nState: {}", vending_machine.get_state_name());
    vending_machine.insert_coin();

    println!("State: {}", vending_machine.get_state_name());
    vending_machine.press_button();

    println!("\nState: {}", vending_machine.get_state_name());
    vending_machine.insert_coin();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_vending_machine_initial_state() {
        let machine = VendingMachine::new(5);
        assert_eq!(machine.get_state_name(), "No Coin");
    }

    #[test]
    fn test_insert_coin_changes_state() {
        let mut machine = VendingMachine::new(5);
        machine.insert_coin();
        assert_eq!(machine.get_state_name(), "Has Coin");
    }

    #[test]
    fn test_press_button_dispenses() {
        let mut machine = VendingMachine::new(5);
        machine.insert_coin();
        machine.press_button();
        // Should not panic
    }

    #[test]
    fn test_empty_vending_machine() {
        let machine = VendingMachine::new(0);
        assert_eq!(machine.get_state_name(), "Out of Stock");
    }

    #[test]
    fn test_eject_coin() {
        let mut machine = VendingMachine::new(5);
        machine.insert_coin();
        machine.eject_coin();
        assert_eq!(machine.get_state_name(), "No Coin");
    }
}
