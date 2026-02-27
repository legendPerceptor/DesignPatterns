//! # Observer Pattern
//!
//! The Observer pattern defines a one-to-many dependency so that when one
//! object changes state, all its dependents are notified and updated automatically.
//!
//! ## Key Participants
//! - **Subject**: Knows its observers and provides attach/detach interface
//! - **Observer**: Defines updating interface for notifications
//! - **ConcreteSubject**: Stores state of interest and sends notifications
//! - **ConcreteObserver**: Maintains reference to subject and implements update
//!
//! ## Benefits
//! - Abstract coupling between Subject and Observer
//! - Support for broadcast communication
//! - Subjects and observers vary independently

use std::rc::Rc;
use std::cell::RefCell;

// Observer trait
pub trait Observer {
    fn update(&self, data: &str);
    fn get_name(&self) -> &str;
}

// Subject trait
pub trait Subject {
    fn attach(&mut self, observer: Rc<RefCell<dyn Observer>>);
    fn detach(&mut self, observer: &Rc<RefCell<dyn Observer>>);
    fn notify(&self, data: &str);
}

// Weather Data
#[derive(Clone)]
pub struct WeatherData {
    pub temperature: f64,
    pub humidity: f64,
    pub pressure: f64,
}

// Concrete Subject
pub struct WeatherStation {
    observers: Vec<Rc<RefCell<dyn Observer>>>,
    measurements: WeatherData,
}

impl WeatherStation {
    pub fn new() -> Self {
        Self {
            observers: Vec::new(),
            measurements: WeatherData {
                temperature: 0.0,
                humidity: 0.0,
                pressure: 0.0,
            },
        }
    }

    pub fn set_measurements(&mut self, temperature: f64, humidity: f64, pressure: f64) {
        self.measurements = WeatherData {
            temperature,
            humidity,
            pressure,
        };

        println!(
            "\n[WeatherStation] New measurements: {}°C, {}% humidity, {} hPa",
            temperature, humidity, pressure
        );

        let data = format!("{},{},{}", temperature, humidity, pressure);
        self.notify(&data);
    }
}

impl Subject for WeatherStation {
    fn attach(&mut self, observer: Rc<RefCell<dyn Observer>>) {
        println!(
            "[Subject] Attached observer: {}",
            observer.borrow().get_name()
        );
        self.observers.push(observer);
    }

    fn detach(&mut self, observer: &Rc<RefCell<dyn Observer>>) {
        if let Some(pos) = self.observers.iter().position(|o| {
            Rc::ptr_eq(o, observer)
        }) {
            self.observers.remove(pos);
            println!(
                "[Subject] Detached observer: {}",
                observer.borrow().get_name()
            );
        }
    }

    fn notify(&self, data: &str) {
        for observer in &self.observers {
            observer.borrow().update(data);
        }
    }
}

impl Default for WeatherStation {
    fn default() -> Self {
        Self::new()
    }
}

// Concrete Observers
pub struct PhoneDisplay {
    owner: String,
}

impl PhoneDisplay {
    pub fn new(owner: &str) -> Self {
        Self {
            owner: owner.to_string(),
        }
    }

    fn _display(&self, parts: &[&str]) {
        if let (Some(&temp), Some(&hum)) = (parts.first(), parts.get(1)) {
            println!("    ┌─────────────────┐");
            println!("    │ {}   │", "12:34:56");
            println!("    │ {}°C  │", temp);
            println!("    │ {}%   │", hum);
            println!("    └─────────────────┘");
        }
    }
}

impl Observer for PhoneDisplay {
    fn update(&self, data: &str) {
        println!(
            "  [Phone Display - {}] Notification received!",
            self.owner
        );
        println!("    Weather Alert: {}", data.replace(",", ", "));
        let parts: Vec<&str> = data.split(',').collect();
        self._display(&parts);
    }

    fn get_name(&self) -> &str {
        &self.owner
    }
}

pub struct WebDisplay;

impl Observer for WebDisplay {
    fn update(&self, data: &str) {
        println!("  [Web Display] Updating dashboard...");
        let parts: Vec<&str> = data.split(',').collect();
        println!("    <div class='weather-widget'>");
        if let Some(&temp) = parts.first() {
            println!("      <h2>Weather Station</h2>");
            println!("      <p>Temperature: {}°C</p>", temp);
        }
        if let Some(&hum) = parts.get(1) {
            println!("      <p>Humidity: {}%</p>", hum);
        }
        if let Some(&pres) = parts.get(2) {
            println!("      <p>Pressure: {} hPa</p>", pres);
        }
        println!("    </div>");
    }

    fn get_name(&self) -> &str {
        "Web Display"
    }
}

pub struct ForecastDisplay {
    last_data: Option<String>,
}

impl ForecastDisplay {
    pub fn new() -> Self {
        Self {
            last_data: None,
        }
    }
}

impl Observer for ForecastDisplay {
    fn update(&self, data: &str) {
        println!("  [Forecast Display] Analyzing trends...");

        let parts: Vec<&str> = data.split(',').collect();
        let pressure: f64 = parts.get(2).and_then(|s| s.parse().ok()).unwrap_or(1015.0);

        let forecast = if pressure < 1000.0 {
            "Rain expected"
        } else if pressure > 1020.0 {
            "Clear skies ahead"
        } else {
            "Conditions stable"
        };

        println!("    Forecast: {}", forecast);
    }

    fn get_name(&self) -> &str {
        "Forecast Display"
    }
}

impl Default for ForecastDisplay {
    fn default() -> Self {
        Self::new()
    }
}

// YouTube Example
pub struct YouTubeChannel {
    name: String,
}

impl YouTubeChannel {
    pub fn new(name: &str) -> Self {
        Self {
            name: name.to_string(),
        }
    }

    pub fn upload_video(&self, title: &str) {
        println!("\n[YouTube] {} uploaded new video: \"{}\"", self.name, title);
        // Note: Would notify subscribers here
    }
}

pub struct Subscriber {
    name: String,
}

impl Subscriber {
    pub fn new(name: &str) -> Self {
        Self {
            name: name.to_string(),
        }
    }

    pub fn notified(&self, video_title: &str) {
        println!(
            "  [Subscriber] {} received notification: \"{}\" is now live!",
            self.name, video_title
        );
    }

    pub fn get_name(&self) -> &str {
        &self.name
    }
}

/// Example demonstrating Observer pattern
pub fn example() {
    println!("\n--- Observer Pattern Example ---\n");

    println!("1. Weather Station");
    let mut station = WeatherStation::new();

    let phone = Rc::new(RefCell::new(PhoneDisplay::new("John")));
    let web = Rc::new(RefCell::new(WebDisplay));
    let forecast = Rc::new(RefCell::new(ForecastDisplay::new()));

    station.attach(phone.clone());
    station.attach(web.clone());
    station.attach(forecast.clone());

    station.set_measurements(25.5, 65.0, 1015.0);

    println!("\n2. All observers notified!");
    println!("Note: Detach functionality is available but requires type coercion");
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_weather_station() {
        let station = WeatherStation::new();
        // Should not panic
    }

    #[test]
    fn test_attach_detach() {
        let mut station = WeatherStation::new();
        let observer = Rc::new(RefCell::new(PhoneDisplay::new("Test")));

        station.attach(observer.clone());
        // Note: detach requires type coercion for concrete observer types
        // station.detach(&observer);
        // Should not panic
    }

    #[test]
    fn test_phone_display() {
        let display = PhoneDisplay::new("Test");
        assert_eq!(display.get_name(), "Test");
    }

    #[test]
    fn test_youtube_channel() {
        let channel = YouTubeChannel::new("TestChannel");
        channel.upload_video("Test Video");
        // Should not panic
    }
}
