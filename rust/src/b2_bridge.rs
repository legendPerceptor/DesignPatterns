//! # Bridge Pattern
//!
//! The Bridge pattern decouples an abstraction from its implementation so
//! that the two can vary independently.
//!
//! ## Key Participants
//! - **Abstraction**: Defines abstract interface
//! - **RefinedAbstraction**: Extends Abstraction
//! - **Implementor**: Defines interface for implementation classes
//! - **ConcreteImplementor**: Implements Implementor interface
//!
//! ## Benefits
//! - Decouples abstraction from implementation
//! - Extensibility - can extend abstractions and implementations independently
//! - Hides implementation details from clients

// Implementor interface
pub trait Device {
    fn turn_on(&self);
    fn turn_off(&self);
    fn set_volume(&self, percent: u8);
    fn get_name(&self) -> &str;
}

// Concrete Implementors
pub struct TV {
    name: String,
    state: TVState,
}

struct TVState {
    on: bool,
    volume: u8,
}

impl TV {
    pub fn new(name: &str) -> Self {
        Self {
            name: name.to_string(),
            state: TVState { on: false, volume: 0 },
        }
    }
}

impl Device for TV {
    fn turn_on(&self) {
        println!("{} TV is ON", self.name);
    }

    fn turn_off(&self) {
        println!("{} TV is OFF", self.name);
    }

    fn set_volume(&self, percent: u8) {
        println!("{} TV volume set to {}%", self.name, percent);
    }

    fn get_name(&self) -> &str {
        &self.name
    }
}

pub struct Radio {
    name: String,
    state: RadioState,
}

struct RadioState {
    on: bool,
    volume: u8,
}

impl Radio {
    pub fn new(name: &str) -> Self {
        Self {
            name: name.to_string(),
            state: RadioState { on: false, volume: 0 },
        }
    }
}

impl Device for Radio {
    fn turn_on(&self) {
        println!("Radio ({}) is ON", self.name);
    }

    fn turn_off(&self) {
        println!("Radio ({}) is OFF", self.name);
    }

    fn set_volume(&self, percent: u8) {
        println!("Radio ({}) volume set to {}%", self.name, percent);
    }

    fn get_name(&self) -> &str {
        &self.name
    }
}

// Abstraction
pub struct RemoteControl {
    device: Option<Box<dyn Device>>,
    volume: u8,
}

impl RemoteControl {
    pub fn new() -> Self {
        Self {
            device: None,
            volume: 0,
        }
    }

    pub fn connect(&mut self, device: Box<dyn Device>) {
        println!("Remote connected to: {}", device.get_name());
        self.device = Some(device);
    }

    pub fn toggle_power(&self) {
        if let Some(ref device) = self.device {
            if self.volume > 0 {
                device.turn_off();
            } else {
                device.turn_on();
            }
        }
    }

    pub fn volume_up(&mut self) {
        self.volume = (self.volume + 10).min(100);
        if let Some(ref device) = self.device {
            device.set_volume(self.volume);
        }
    }

    pub fn volume_down(&mut self) {
        self.volume = self.volume.saturating_sub(10);
        if let Some(ref device) = self.device {
            device.set_volume(self.volume);
        }
    }
}

impl Default for RemoteControl {
    fn default() -> Self {
        Self::new()
    }
}

// Refined Abstraction
pub struct AdvancedRemoteControl {
    base: RemoteControl,
}

impl AdvancedRemoteControl {
    pub fn new() -> Self {
        Self {
            base: RemoteControl::new(),
        }
    }

    pub fn connect(&mut self, device: Box<dyn Device>) {
        self.base.connect(device);
    }

    pub fn toggle_power(&self) {
        self.base.toggle_power();
    }

    pub fn volume_up(&mut self) {
        self.base.volume_up();
    }

    pub fn mute(&mut self) {
        self.base.volume = 0;
        if let Some(ref device) = self.base.device {
            device.set_volume(0);
        }
        println!("Muted");
    }
}

impl Default for AdvancedRemoteControl {
    fn default() -> Self {
        Self::new()
    }
}

/// Example demonstrating Bridge pattern
pub fn example() {
    println!("\n--- Bridge Pattern Example ---\n");

    println!("1. Basic Remote with TV:");
    let mut remote = RemoteControl::new();
    remote.connect(Box::new(TV::new("Sony")));
    remote.toggle_power();
    remote.volume_up();
    remote.volume_up();
    remote.toggle_power();

    println!("\n2. Advanced Remote with Radio:");
    let mut adv_remote = AdvancedRemoteControl::new();
    adv_remote.connect(Box::new(Radio::new("98.5 FM")));
    adv_remote.toggle_power();
    adv_remote.volume_up();
    adv_remote.mute();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_tv_device() {
        let tv = TV::new("TestTV");
        assert_eq!(tv.get_name(), "TestTV");
        tv.turn_on();
        tv.set_volume(50);
        tv.turn_off();
    }

    #[test]
    fn test_radio_device() {
        let radio = Radio::new("TestRadio");
        assert_eq!(radio.get_name(), "TestRadio");
        radio.turn_on();
        radio.set_volume(75);
    }

    #[test]
    fn test_remote_control_volume_clamping() {
        let mut remote = RemoteControl::new();
        remote.connect(Box::new(TV::new("Test")));

        // Volume should saturate at 100, not overflow
        for _ in 0..20 {
            remote.volume_up();
        }
        assert!(remote.volume <= 100);
    }

    #[test]
    fn test_remote_control_volume_down() {
        let mut remote = RemoteControl::new();
        remote.connect(Box::new(TV::new("Test")));
        remote.volume_up();
        remote.volume_up();
        remote.volume_down();
        assert_eq!(remote.volume, 10);
    }

    #[test]
    fn test_advanced_remote_mute() {
        let mut adv_remote = AdvancedRemoteControl::new();
        adv_remote.connect(Box::new(TV::new("Test")));
        adv_remote.volume_up();
        adv_remote.volume_up();
        adv_remote.mute();
        assert_eq!(adv_remote.base.volume, 0);
    }
}
