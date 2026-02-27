//! # Singleton Pattern
//!
//! The Singleton pattern ensures a class has only one instance and provides
//! a global point of access to it.
//!
//! ## Key Participants
//! - **Singleton**: Defines instance operation and unique instance
//!
//! ## Benefits
//! - Controlled access to sole instance
//! - Reduced namespace pollution
//! - Can be subclassed
//! - More flexible than class operations

use std::sync::{Arc, Mutex};

// Logger Singleton
pub struct Logger {
    logs: Vec<String>,
}

impl Logger {
    fn new() -> Self {
        Self {
            logs: Vec::new(),
        }
    }

    pub fn log(&mut self, message: &str) {
        let timestamp = chrono_timestamp();
        let entry = format!("[{}] {}", timestamp, message);
        println!("{}", entry);
        self.logs.push(entry);
    }

    pub fn get_logs(&self) -> &[String] {
        &self.logs
    }
}

// Simple timestamp simulation
fn chrono_timestamp() -> String {
    use std::time::{SystemTime, UNIX_EPOCH};
    let duration = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .unwrap();
    format!("{:?}", duration.as_secs())
}

// Singleton using once_cell (would typically use the crate)
// For this example, we'll use a simplified approach with std::sync::OnceLock (Rust 1.70+)
use std::sync::OnceLock;

pub fn get_logger() -> &'static Arc<Mutex<Logger>> {
    static LOGGER: OnceLock<Arc<Mutex<Logger>>> = OnceLock::new();
    LOGGER.get_or_init(|| Arc::new(Mutex::new(Logger::new())))
}

// Alternative: Using a struct with lazy initialization
pub struct SingletonManager {
    data: String,
}

impl SingletonManager {
    pub fn instance() -> &'static Arc<Mutex<SingletonManager>> {
        static INSTANCE: OnceLock<Arc<Mutex<SingletonManager>>> = OnceLock::new();
        INSTANCE.get_or_init(|| {
            Arc::new(Mutex::new(SingletonManager {
                data: "Singleton Data".to_string(),
            }))
        })
    }

    pub fn get_data(&self) -> String {
        self.data.clone()
    }

    pub fn set_data(&mut self, data: &str) {
        self.data = data.to_string();
    }
}

/// Example demonstrating Singleton pattern
pub fn example() {
    println!("\n--- Singleton Pattern Example ---\n");

    // Using the Logger singleton
    println!("1. Logger Singleton:");

    let logger1 = get_logger();
    logger1.lock().unwrap().log("Application started");

    let logger2 = get_logger();
    logger2.lock().unwrap().log("User logged in");

    // Verify both references point to the same instance
    let logger_guard = logger1.lock().unwrap();
    let logs = logger_guard.get_logs();
    println!("\nTotal log entries: {}", logs.len());
    drop(logger_guard); // Explicitly drop the guard

    // Using the SingletonManager
    println!("\n2. SingletonManager:");

    {
        let mut mgr = SingletonManager::instance().lock().unwrap();
        println!("Data: {}", mgr.get_data());
        mgr.set_data("Modified Data");
    }

    {
        let mgr = SingletonManager::instance().lock().unwrap();
        println!("Data after modification: {}", mgr.get_data());
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_logger_singleton() {
        let logger1 = get_logger();
        let logger2 = get_logger();

        // Both should point to the same instance
        let ptr1 = Arc::as_ptr(&logger1) as *const _ as usize;
        let ptr2 = Arc::as_ptr(&logger2) as *const _ as usize;
        assert_eq!(ptr1, ptr2);
    }

    #[test]
    fn test_logger_persists_messages() {
        let logger = get_logger();
        logger.lock().unwrap().log("Test message");

        let logger_guard = logger.lock().unwrap();
        let logs = logger_guard.get_logs();
        assert!(logs.len() > 0);
        assert!(logs.last().unwrap().contains("Test message"));
        drop(logger_guard);
    }

    #[test]
    fn test_singleton_manager_singleton() {
        let mgr1 = SingletonManager::instance();
        let mgr2 = SingletonManager::instance();

        // Both should point to the same instance
        let ptr1 = Arc::as_ptr(&mgr1) as *const _ as usize;
        let ptr2 = Arc::as_ptr(&mgr2) as *const _ as usize;
        assert_eq!(ptr1, ptr2);
    }

    #[test]
    fn test_singleton_manager_shared_state() {
        {
            let mut mgr = SingletonManager::instance().lock().unwrap();
            mgr.set_data("Shared State");
        }

        {
            let mgr = SingletonManager::instance().lock().unwrap();
            assert_eq!(mgr.get_data(), "Shared State");
        }
    }

    #[test]
    fn test_multiple_logger_references() {
        let logger1 = get_logger();
        let logger2 = get_logger();
        let logger3 = get_logger();

        // All three should be the same instance
        let ptr1 = Arc::as_ptr(&logger1) as *const _ as usize;
        let ptr2 = Arc::as_ptr(&logger2) as *const _ as usize;
        let ptr3 = Arc::as_ptr(&logger3) as *const _ as usize;

        assert_eq!(ptr1, ptr2);
        assert_eq!(ptr2, ptr3);
    }
}
