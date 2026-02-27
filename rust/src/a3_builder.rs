//! # Builder Pattern
//!
//! The Builder pattern separates the construction of complex objects from
//! their representation, allowing the same construction process to create
//! different representations.
//!
//! ## Key Participants
//! - **Builder**: Specifies abstract interface for building parts
//! - **ConcreteBuilder**: Constructs and assembles parts
//! - **Director**: Constructs object using Builder interface
//! - **Product**: The object under construction
//!
//! ## Benefits
//! - Lets you vary product's internal representation
//! - Isolates construction code from representation code
//! - Gives finer control over construction process

#[derive(Debug, Clone)]
pub struct HttpRequest {
    pub method: String,
    pub url: String,
    pub headers: Vec<(String, String)>,
    pub body: Option<String>,
    pub timeout_ms: u64,
}

impl HttpRequest {
    pub fn send(&self) {
        println!("Sending HTTP Request:");
        println!("  {} {}", self.method, self.url);
        println!("  Headers: {} entries", self.headers.len());
        if let Some(body) = &self.body {
            println!("  Body: {} bytes", body.len());
        } else {
            println!("  Body: (none)");
        }
        println!("  Timeout: {}ms", self.timeout_ms);
    }
}

pub struct HttpRequestBuilder {
    method: Option<String>,
    url: Option<String>,
    headers: Vec<(String, String)>,
    body: Option<String>,
    timeout_ms: Option<u64>,
}

impl HttpRequestBuilder {
    pub fn new() -> Self {
        Self {
            method: None,
            url: None,
            headers: Vec::new(),
            body: None,
            timeout_ms: None,
        }
    }

    pub fn method(mut self, method: &str) -> Self {
        self.method = Some(method.to_string());
        self
    }

    pub fn url(mut self, url: &str) -> Self {
        self.url = Some(url.to_string());
        self
    }

    pub fn header(mut self, key: &str, value: &str) -> Self {
        self.headers.push((key.to_string(), value.to_string()));
        self
    }

    pub fn body(mut self, body: &str) -> Self {
        self.body = Some(body.to_string());
        self
    }

    pub fn timeout(mut self, timeout_ms: u64) -> Self {
        self.timeout_ms = Some(timeout_ms);
        self
    }

    pub fn build(self) -> Result<HttpRequest, String> {
        let method = self.method.ok_or("Method is required")?;
        let url = self.url.ok_or("URL is required")?;
        let timeout_ms = self.timeout_ms.unwrap_or(5000);

        Ok(HttpRequest {
            method,
            url,
            headers: self.headers,
            body: self.body,
            timeout_ms,
        })
    }
}

impl Default for HttpRequestBuilder {
    fn default() -> Self {
        Self::new()
    }
}

/// Example demonstrating Builder pattern
pub fn example() {
    println!("\n--- Builder Pattern Example ---\n");

    // Simple GET request
    println!("1. Simple GET Request:");
    let get_request = HttpRequestBuilder::new()
        .method("GET")
        .url("https://api.example.com/users")
        .timeout(3000)
        .build()
        .unwrap();
    get_request.send();

    // Complex POST request
    println!("\n2. Complex POST Request:");
    let post_request = HttpRequestBuilder::new()
        .method("POST")
        .url("https://api.example.com/data")
        .header("Content-Type", "application/json")
        .header("Authorization", "Bearer token123")
        .body(r#"{"id": 42, "name": "John Doe"}"#)
        .timeout(10000)
        .build()
        .unwrap();
    post_request.send();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_builder_creates_request() {
        let request = HttpRequestBuilder::new()
            .method("GET")
            .url("https://example.com")
            .build()
            .unwrap();

        assert_eq!(request.method, "GET");
        assert_eq!(request.url, "https://example.com");
    }

    #[test]
    fn test_builder_with_all_fields() {
        let request = HttpRequestBuilder::new()
            .method("POST")
            .url("https://example.com")
            .header("X-Custom", "value")
            .body("test body")
            .timeout(5000)
            .build()
            .unwrap();

        assert_eq!(request.method, "POST");
        assert_eq!(request.headers.len(), 1);
        assert_eq!(request.body, Some("test body".to_string()));
        assert_eq!(request.timeout_ms, 5000);
    }

    #[test]
    fn test_builder_requires_method() {
        let result = HttpRequestBuilder::new()
            .url("https://example.com")
            .build();

        assert!(result.is_err());
        assert_eq!(result.unwrap_err(), "Method is required");
    }

    #[test]
    fn test_builder_requires_url() {
        let result = HttpRequestBuilder::new()
            .method("GET")
            .build();

        assert!(result.is_err());
        assert_eq!(result.unwrap_err(), "URL is required");
    }

    #[test]
    fn test_builder_default_timeout() {
        let request = HttpRequestBuilder::new()
            .method("GET")
            .url("https://example.com")
            .build()
            .unwrap();

        assert_eq!(request.timeout_ms, 5000); // Default timeout
    }
}
