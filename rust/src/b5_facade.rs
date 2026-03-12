//! # Facade Pattern
//!
//! The Facade pattern provides a unified interface to a set of interfaces in a
//! subsystem. Facade defines a higher-level interface that makes the subsystem
//! easier to use.
//!
//! ## Key Participants
//! - **Facade**: Knows which subsystem classes are responsible and delegates requests
//! - **Subsystem classes**: Implement subsystem functionality
//! - **Client**: Communicates with the subsystem through the Facade
//!
//! ## Benefits
//! - Shields clients from subsystem components
//! - Promotes weak coupling between subsystem and clients
//! - Doesn't prevent clients from using subsystem if needed

// Subsystem components
pub struct Amplifier {
    volume: u8,
}

impl Amplifier {
    pub fn new() -> Self {
        Self { volume: 0 }
    }

    pub fn on(&self) {
        println!("Amplifier is ON");
    }

    pub fn off(&self) {
        println!("Amplifier is OFF");
    }

    pub fn set_dvd(&self) {
        println!("Amplifier: DVD player selected");
    }

    pub fn set_cd(&self) {
        println!("Amplifier: CD player selected");
    }

    pub fn set_volume(&mut self, level: u8) {
        self.volume = level.min(100);
        println!("Amplifier: volume set to {}", self.volume);
    }
}

impl Default for Amplifier {
    fn default() -> Self {
        Self::new()
    }
}

pub struct DvdPlayer;

impl DvdPlayer {
    pub fn new() -> Self {
        Self
    }

    pub fn on(&self) {
        println!("DVD Player: ON");
    }

    pub fn off(&self) {
        println!("DVD Player: OFF");
    }

    pub fn play(&self, movie: &str) {
        println!("DVD Player: playing \"{}\"", movie);
    }
}

impl Default for DvdPlayer {
    fn default() -> Self {
        Self::new()
    }
}

pub struct CdPlayer;

impl CdPlayer {
    pub fn new() -> Self {
        Self
    }

    pub fn on(&self) {
        println!("CD Player: ON");
    }

    pub fn off(&self) {
        println!("CD Player: OFF");
    }

    pub fn play(&self, album: &str) {
        println!("CD Player: playing \"{}\"", album);
    }
}

impl Default for CdPlayer {
    fn default() -> Self {
        Self::new()
    }
}

pub struct PopcornPopper;

impl PopcornPopper {
    pub fn new() -> Self {
        Self
    }

    pub fn on(&self) {
        println!("Popcorn Popper: ON");
    }

    pub fn off(&self) {
        println!("Popcorn Popper: OFF");
    }

    pub fn pop(&self) {
        println!("Popcorn Popper: Popping corn!");
    }
}

impl Default for PopcornPopper {
    fn default() -> Self {
        Self::new()
    }
}

pub struct Projector;

impl Projector {
    pub fn new() -> Self {
        Self
    }

    pub fn on(&self) {
        println!("Projector: ON");
    }

    pub fn off(&self) {
        println!("Projector: OFF");
    }

    pub fn wide_screen_mode(&self) {
        println!("Projector: widescreen mode");
    }
}

impl Default for Projector {
    fn default() -> Self {
        Self::new()
    }
}

pub struct TheaterLights;

impl TheaterLights {
    pub fn new() -> Self {
        Self
    }

    pub fn on(&self) {
        println!("Theater Lights: ON");
    }

    pub fn off(&self) {
        println!("Theater Lights: OFF");
    }

    pub fn dim(&self, percent: u8) {
        println!("Theater Lights: dimmed to {}%", percent);
    }
}

impl Default for TheaterLights {
    fn default() -> Self {
        Self::new()
    }
}

pub struct Screen;

impl Screen {
    pub fn new() -> Self {
        Self
    }

    pub fn up(&self) {
        println!("Screen: UP");
    }

    pub fn down(&self) {
        println!("Screen: DOWN");
    }
}

impl Default for Screen {
    fn default() -> Self {
        Self::new()
    }
}

pub struct Tuner {
    frequency: f64,
}

impl Tuner {
    pub fn new() -> Self {
        Self { frequency: 0.0 }
    }

    pub fn on(&self) {
        println!("Tuner: ON");
    }

    pub fn off(&self) {
        println!("Tuner: OFF");
    }

    pub fn set_frequency(&mut self, freq: f64) {
        self.frequency = freq;
        println!("Tuner: frequency set to {}", freq);
    }

    pub fn set_am(&self) {
        println!("Tuner: AM mode");
    }

    pub fn set_fm(&self) {
        println!("Tuner: FM mode");
    }
}

impl Default for Tuner {
    fn default() -> Self {
        Self::new()
    }
}

// Facade
pub struct HomeTheaterFacade {
    amp: Amplifier,
    dvd: DvdPlayer,
    cd: CdPlayer,
    popper: PopcornPopper,
    projector: Projector,
    lights: TheaterLights,
    screen: Screen,
    tuner: Tuner,
}

impl HomeTheaterFacade {
    pub fn new() -> Self {
        Self {
            amp: Amplifier::new(),
            dvd: DvdPlayer::new(),
            cd: CdPlayer::new(),
            popper: PopcornPopper::new(),
            projector: Projector::new(),
            lights: TheaterLights::new(),
            screen: Screen::new(),
            tuner: Tuner::new(),
        }
    }

    pub fn watch_movie(&mut self, movie: &str) {
        println!("\nGet ready to watch \"{}\"...", movie);

        self.popper.on();
        self.popper.pop();

        self.lights.dim(10);

        self.screen.down();
        self.projector.on();
        self.projector.wide_screen_mode();

        self.amp.on();
        self.amp.set_dvd();
        self.amp.set_volume(5);

        self.dvd.on();
        self.dvd.play(movie);
    }

    pub fn end_movie(&self) {
        println!("\nShutting down home theater...");

        self.popper.off();
        self.lights.on();
        self.screen.up();
        self.projector.off();
        self.amp.off();
        self.dvd.stop();
    }

    pub fn listen_to_cd(&mut self, album: &str) {
        println!("\nGet ready to listen to \"{}\"...", album);

        self.lights.on();
        self.amp.on();
        self.amp.set_cd();
        self.amp.set_volume(5);
        self.cd.on();
        self.cd.play(album);
    }

    pub fn listen_to_radio(&mut self, frequency: f64) {
        println!("\nGet ready to listen to radio at {}...", frequency);

        self.amp.on();
        self.amp.set_volume(5);
        self.tuner.on();
        self.tuner.set_frequency(frequency);
        self.tuner.set_fm();

        println!("Radio tuned to {}", frequency);
    }

    pub fn end_radio(&self) {
        println!("\nShutting down radio...");
        self.tuner.off();
        self.amp.off();
    }

    pub fn turn_off_all_lights(&self) {
        println!("\nTurning off all lights...");
        self.lights.off();
    }
}

impl DvdPlayer {
    pub fn stop(&self) {
        println!("DVD Player: STOP");
    }
}

impl Default for HomeTheaterFacade {
    fn default() -> Self {
        Self::new()
    }
}

/// Example demonstrating Facade pattern
pub fn example() {
    println!("\n--- Facade Pattern Example ---\n");

    println!("=== Demonstrating individual components ===");
    let mut amp = Amplifier::new();
    let dvd = DvdPlayer::new();
    let cd = CdPlayer::new();
    let mut tuner = Tuner::new();

    println!("\n--- Testing Amplifier ---");
    amp.on();
    amp.set_volume(50);
    amp.off();

    println!("\n--- Testing DVD Player ---");
    dvd.on();
    dvd.play("Movie");
    dvd.off();

    println!("\n--- Testing CD Player ---");
    cd.on();
    cd.play("Album");
    cd.off();

    println!("\n--- Testing Tuner ---");
    tuner.on();
    tuner.set_frequency(101.5);
    tuner.set_fm();
    tuner.set_am();
    tuner.off();

    println!("\n=== Using Facade ===");
    let mut facade = HomeTheaterFacade::new();

    println!("\n1. Watch Movie:");
    facade.watch_movie("The Matrix");

    println!("\n2. End Movie:");
    facade.end_movie();

    println!("\n3. Listen to CD:");
    facade.listen_to_cd("Pink Floyd - Dark Side of the Moon");

    println!("\n4. Listen to Radio:");
    facade.listen_to_radio(101.5);

    println!("\n5. End Radio:");
    facade.end_radio();

    println!("\n6. Turn off all lights:");
    facade.turn_off_all_lights();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_facade_creates_complete_system() {
        let mut facade = HomeTheaterFacade::new();
        // Should not panic - all components initialized
        facade.watch_movie("Test");
        facade.end_movie();
    }

    #[test]
    fn test_watch_movie_flow() {
        let mut facade = HomeTheaterFacade::new();
        facade.watch_movie("Test Movie");
        // Verify components are used (outputs would be visible in run)
    }

    #[test]
    fn test_listen_to_cd_flow() {
        let mut facade = HomeTheaterFacade::new();
        facade.listen_to_cd("Test Album");
        // Verify components are used
    }
}
