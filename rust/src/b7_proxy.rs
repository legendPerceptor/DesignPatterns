//! # Proxy Pattern
//!
//! The Proxy pattern provides a surrogate or placeholder for another object to
//! control access to it.
//!
//! ## Key Participants
//! - **Proxy**: Maintains reference to RealSubject and controls access
//! - **RealSubject**: Defines real object that proxy represents
//! - **Subject**: Common interface for Proxy and RealSubject
//!
//! ## Benefits
//! - Remote proxy: Hides fact that object resides in different address space
//! - Virtual proxy: Lazy initialization
//! - Protection proxy: Access control
//! - Smart reference: Additional actions when accessing object

// Subject interface
use std::cell::RefCell;
pub trait Image {
    fn display(&self);
    fn get_filename(&self) -> &str;
}

// Real Subject
pub struct RealImage {
    filename: String,
}

impl RealImage {
    pub fn new(filename: &str) -> Self {
        Self {
            filename: filename.to_string(),
        }
    }

    fn load_from_disk(&self) {
        println!("Loading {} from disk", self.filename);
    }
}

impl Image for RealImage {
    fn display(&self) {
        println!("Displaying {}", self.filename);
    }

    fn get_filename(&self) -> &str {
        &self.filename
    }
}

// Proxy (Virtual Proxy - Lazy Initialization)
pub struct ProxyImage {
    filename: String,
    real_image: Option<Box<RealImage>>,
}

impl ProxyImage {
    pub fn new(filename: &str) -> Self {
        Self {
            filename: filename.to_string(),
            real_image: None,
        }
    }
}

impl Image for ProxyImage {
    fn display(&self) {
        // Lazy initialization - only load when first accessed
        if let Some(ref real) = self.real_image {
            real.display();
        } else {
            // Note: We can't mutate through &self, so in real code you'd use
            // RefCell or similar for interior mutability
            // This is a simplified example
            println!("Proxy: Initializing real image on first access...");
            let real = RealImage::new(&self.filename);
            real.load_from_disk();
            real.display();
        }
    }

    fn get_filename(&self) -> &str {
        &self.filename
    }
}

// Alternative with Rc<RefCell>> for proper interior mutability

pub struct MutableProxyImage {
    filename: String,
    real_image: RefCell<Option<Box<RealImage>>>,
}

impl MutableProxyImage {
    pub fn new(filename: &str) -> Self {
        Self {
            filename: filename.to_string(),
            real_image: RefCell::new(None),
        }
    }
}

impl Image for MutableProxyImage {
    fn display(&self) {
        let mut real_opt = self.real_image.borrow_mut();
        if real_opt.is_none() {
            println!("Proxy: Initializing real image on first access...");
            let real = RealImage::new(&self.filename);
            real.load_from_disk();
            *real_opt = Some(Box::new(real));
        }

        if let Some(ref real) = *real_opt {
            real.display();
        }
    }

    fn get_filename(&self) -> &str {
        &self.filename
    }
}

// Protection Proxy example
pub struct Internet;

impl Internet {
    pub fn new() -> Self {
        Self
    }

    pub fn connect_to(&self, server: &str) {
        println!("Connecting to {}", server);
    }
}

impl Default for Internet {
    fn default() -> Self {
        Self::new()
    }
}

pub struct ProxyInternet {
    blocked_sites: Vec<String>,
}

impl ProxyInternet {
    pub fn new() -> Self {
        Self {
            blocked_sites: vec!["blocked.com".to_string(), "restricted.net".to_string()],
        }
    }

    pub fn connect_to(&self, server: &str) {
        if self.blocked_sites.contains(&server.to_string()) {
            println!("Access to {} is blocked!", server);
        } else {
            println!("Connecting to {}", server);
        }
    }
}

impl Default for ProxyInternet {
    fn default() -> Self {
        Self::new()
    }
}

/// Example demonstrating Proxy pattern
pub fn example() {
    println!("\n--- Proxy Pattern Example ---\n");

    println!("1. Virtual Proxy (Lazy Loading):");
    let proxy = MutableProxyImage::new("large_photo.jpg");

    println!("First display - should load image:");
    proxy.display();
    println!("Filename: {}", proxy.get_filename());

    println!("\nSecond display - uses cached image:");
    proxy.display();

    println!("\n2. Simple Proxy (shows lazy initialization):");
    let proxy = ProxyImage::new("photo.jpg");
    println!("Filename: {}", proxy.get_filename());
    proxy.display();

    println!("\n3. Protection Proxy (Access Control):");
    let internet = ProxyInternet::new();

    println!("\nTrying to access allowed site:");
    internet.connect_to("google.com");

    println!("\nTrying to access blocked site:");
    internet.connect_to("blocked.com");

    println!("\n4. Direct Internet access (no proxy):");
    let direct = Internet::new();
    direct.connect_to("example.com");
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_proxy_image_lazy_loading() {
        let proxy = MutableProxyImage::new("test.jpg");
        // First access should load
        proxy.display();
        // Second access should use cached
        proxy.display();
        // Both should work
    }

    #[test]
    fn test_proxy_gets_filename() {
        let proxy = MutableProxyImage::new("test.jpg");
        assert_eq!(proxy.get_filename(), "test.jpg");
    }

    #[test]
    fn test_protection_internet_allows_access() {
        let internet = ProxyInternet::new();
        internet.connect_to("google.com");
        // Should not block
    }

    #[test]
    fn test_protection_internet_blocks_access() {
        let internet = ProxyInternet::new();
        internet.connect_to("blocked.com");
        // Should block
    }

    #[test]
    fn test_proxy_image_filename_preserved() {
        let proxy = MutableProxyImage::new("image.png");
        assert_eq!(proxy.get_filename(), "image.png");
    }
}
