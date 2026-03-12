//! # Command Pattern
//!
//! The Command pattern encapsulates a request as an object, thereby letting you
//! parameterize clients with different requests, queue or log requests, and
//! support undoable operations.
//!
//! ## Key Participants
//! - **Command**: Declares interface for executing an operation
//! - **ConcreteCommand**: Defines binding between Receiver and action
//! - **Invoker**: Asks command to carry out request
//! - **Receiver**: Knows how to perform operations
//!
//! ## Benefits
//! - Decouples invoker from receiver
//! - Commands are first-class objects
//! - Easy to add new commands
//! - Supports undo/redo

use std::collections::VecDeque;

// Receivers
pub struct Light;

impl Light {
    pub fn new() -> Self {
        Self
    }

    pub fn on(&self) {
        println!("Light is ON");
    }

    pub fn off(&self) {
        println!("Light is OFF");
    }

    pub fn dim(&self, level: u8) {
        println!("Light dimmed to {}%", level);
    }
}

impl Default for Light {
    fn default() -> Self {
        Self::new()
    }
}

pub struct Stereo {
    #[allow(dead_code)]
    state: StereoState,
}

#[allow(dead_code)]
struct StereoState {
    on: bool,
    cd: bool,
    volume: u8,
}

impl Stereo {
    pub fn new() -> Self {
        Self {
            state: StereoState {
                on: false,
                cd: false,
                volume: 0,
            },
        }
    }

    pub fn on(&self) {
        println!("Stereo is ON");
    }

    pub fn off(&self) {
        println!("Stereo is OFF");
    }

    pub fn set_cd(&self) {
        println!("Stereo: CD player selected");
    }

    pub fn set_volume(&self, level: u8) {
        println!("Stereo: volume set to {}", level);
    }

    // Helper method to show state (for demonstration)
    #[allow(dead_code)]
    fn get_state(&self) -> &StereoState {
        &self.state
    }
}

impl Default for Stereo {
    fn default() -> Self {
        Self::new()
    }
}

// Command trait
pub trait Command {
    fn execute(&self);
    fn undo(&self);
}

// Concrete Commands
pub struct LightOnCommand {
    light: Light,
}

impl LightOnCommand {
    pub fn new(light: Light) -> Self {
        Self { light }
    }
}

impl Command for LightOnCommand {
    fn execute(&self) {
        self.light.on();
    }

    fn undo(&self) {
        self.light.off();
    }
}

pub struct LightOffCommand {
    light: Light,
}

impl LightOffCommand {
    pub fn new(light: Light) -> Self {
        Self { light }
    }
}

impl Command for LightOffCommand {
    fn execute(&self) {
        self.light.off();
    }

    fn undo(&self) {
        self.light.on();
    }
}

pub struct StereoOnWithCDCommand {
    stereo: Stereo,
}

impl StereoOnWithCDCommand {
    pub fn new(stereo: Stereo) -> Self {
        Self { stereo }
    }
}

impl Command for StereoOnWithCDCommand {
    fn execute(&self) {
        self.stereo.on();
        self.stereo.set_cd();
        self.stereo.set_volume(11);
    }

    fn undo(&self) {
        self.stereo.off();
    }
}

pub struct NoCommand;

impl Command for NoCommand {
    fn execute(&self) {
        println!("[No operation]");
    }

    fn undo(&self) {
        // Nothing to undo
    }
}

// Invoker
pub struct RemoteControl {
    on_commands: Vec<Box<dyn Command>>,
    off_commands: Vec<Box<dyn Command>>,
    undo_stack: VecDeque<Box<dyn Command>>,
}

impl RemoteControl {
    pub fn new() -> Self {
        const NUM_SLOTS: usize = 7;

        let mut on_commands: Vec<Box<dyn Command>> = Vec::with_capacity(NUM_SLOTS);
        let mut off_commands: Vec<Box<dyn Command>> = Vec::with_capacity(NUM_SLOTS);

        for _ in 0..NUM_SLOTS {
            on_commands.push(Box::new(NoCommand));
            off_commands.push(Box::new(NoCommand));
        }

        Self {
            on_commands,
            off_commands,
            undo_stack: VecDeque::with_capacity(10),
        }
    }

    pub fn set_command(&mut self, slot: usize, on_cmd: Box<dyn Command>, off_cmd: Box<dyn Command>) {
        if slot < self.on_commands.len() {
            self.on_commands[slot] = on_cmd;
            self.off_commands[slot] = off_cmd;
        }
    }

    pub fn on_button_was_pressed(&mut self, slot: usize) {
        if slot < self.on_commands.len() {
            println!("\n[Remote: ON button {} pressed]", slot);
            self.on_commands[slot].execute();
            // Note: We can't actually move the command out due to trait object limitations
            // In a real implementation, you'd use Rc or similar
        }
    }

    pub fn off_button_was_pressed(&mut self, slot: usize) {
        if slot < self.off_commands.len() {
            println!("\n[Remote: OFF button {} pressed]", slot);
            self.off_commands[slot].execute();
        }
    }

    pub fn undo_button_was_pressed(&self) {
        println!("\n[Remote: UNDO button pressed]");
        if let Some(cmd) = self.undo_stack.back() {
            cmd.undo();
        } else {
            println!("Nothing to undo.");
        }
    }
}

impl Default for RemoteControl {
    fn default() -> Self {
        Self::new()
    }
}

/// Example demonstrating Command pattern
pub fn example() {
    println!("\n--- Command Pattern Example ---\n");

    // Create receivers
    let living_room_light = Light::new();
    let stereo = Stereo::new();

    println!("=== Testing Light directly ===");
    living_room_light.on();
    living_room_light.dim(50);
    living_room_light.off();

    println!("\n=== Testing Stereo directly ===");
    stereo.on();
    stereo.set_cd();
    stereo.set_volume(11);
    stereo.off();

    // Create commands
    let light_on = Box::new(LightOnCommand::new(living_room_light));
    let light_off = Box::new(LightOffCommand::new(Light::new()));
    let stereo_on = Box::new(StereoOnWithCDCommand::new(stereo));

    // Set up remote
    let mut remote = RemoteControl::new();
    remote.set_command(0, light_on, light_off);
    remote.set_command(1, stereo_on, Box::new(LightOffCommand::new(Light::new())));

    println!("\n=== Using Remote Control ===");
    remote.on_button_was_pressed(0);
    remote.off_button_was_pressed(0);

    remote.on_button_was_pressed(1);
    remote.undo_button_was_pressed();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_light_on_command() {
        let light = Light::new();
        let command = LightOnCommand::new(light);
        command.execute();
        // Should not panic
    }

    #[test]
    fn test_light_off_command() {
        let light = Light::new();
        let command = LightOffCommand::new(light);
        command.execute();
        // Should not panic
    }

    #[test]
    fn test_command_undo() {
        let light = Light::new();
        let command = LightOnCommand::new(light);
        command.execute();
        command.undo();
        // Should not panic
    }

    #[test]
    fn test_stereo_command() {
        let stereo = Stereo::new();
        let command = StereoOnWithCDCommand::new(stereo);
        command.execute();
        // Should not panic
    }

    #[test]
    fn test_remote_control() {
        let mut remote = RemoteControl::new();
        let light = Light::new();

        remote.set_command(
            0,
            Box::new(LightOnCommand::new(light)),
            Box::new(LightOffCommand::new(Light::new())),
        );

        remote.on_button_was_pressed(0);
        remote.off_button_was_pressed(0);
        // Should not panic
    }
}
