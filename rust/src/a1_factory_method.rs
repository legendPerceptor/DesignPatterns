//! # Factory Method Pattern
//!
//! The Factory Method pattern defines an interface for creating an object,
//! but lets subclasses decide which class to instantiate.
//!
//! ## Key Participants
//! - **Product**: The interface for objects the factory method creates
//! - **ConcreteProduct**: Implements the Product interface
//! - **Creator**: Declares the factory method
//! - **ConcreteCreator**: Overrides the factory method to return ConcreteProduct
//!
//! ## Benefits
//! - Eliminates need to bind application classes to specific classes
//! - Code only deals with the Product interface
//! - Easy to extend for new products

// Product trait
#[allow(dead_code)]
pub trait Document {
    fn create(&self);
    fn get_type(&self) -> &str;
}

// Concrete Products
pub struct TextDocument {
    content: String,
}

#[allow(dead_code)]
impl TextDocument {
    pub fn new() -> Self {
        Self {
            content: String::new(),
        }
    }

    pub fn with_content(content: &str) -> Self {
        Self {
            content: content.to_string(),
        }
    }
}

impl Document for TextDocument {
    fn create(&self) {
        println!("TextDocument created");
        if !self.content.is_empty() {
            println!("  Content: {}", self.content);
        }
    }

    fn get_type(&self) -> &str {
        "TextDocument"
    }
}

pub struct SpreadsheetDocument {
    rows: u32,
    cols: u32,
}

impl SpreadsheetDocument {
    pub fn new() -> Self {
        Self {
            rows: 1000,
            cols: 26,
        }
    }
}

impl Document for SpreadsheetDocument {
    fn create(&self) {
        println!("SpreadsheetDocument created with {}x{} grid", self.rows, self.cols);
    }

    fn get_type(&self) -> &str {
        "SpreadsheetDocument"
    }
}

// Creator (Application)
pub trait Application {
    fn new_document(&self) -> Box<dyn Document>;
    fn create_document(&self) {
        let doc = self.new_document();
        doc.create();
    }
}

// Concrete Creators
pub struct TextEditorApplication;

impl Application for TextEditorApplication {
    fn new_document(&self) -> Box<dyn Document> {
        Box::new(TextDocument::with_content("Hello, World!"))
    }
}

pub struct SpreadsheetApplication;

impl Application for SpreadsheetApplication {
    fn new_document(&self) -> Box<dyn Document> {
        Box::new(SpreadsheetDocument::new())
    }
}

/// Example demonstrating Factory Method pattern
pub fn example() {
    println!("\n--- Factory Method Pattern Example ---\n");

    println!("1. Text Editor Application:");
    let text_app = TextEditorApplication;
    text_app.create_document();

    println!("\n2. Spreadsheet Application:");
    let sheet_app = SpreadsheetApplication;
    sheet_app.create_document();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_text_document_creation() {
        let doc = TextDocument::new();
        assert_eq!(doc.get_type(), "TextDocument");
    }

    #[test]
    fn test_spreadsheet_document_creation() {
        let doc = SpreadsheetDocument::new();
        assert_eq!(doc.get_type(), "SpreadsheetDocument");
        assert_eq!(doc.rows, 1000);
        assert_eq!(doc.cols, 26);
    }

    #[test]
    fn test_text_editor_creates_text_document() {
        let app = TextEditorApplication;
        let doc = app.new_document();
        assert_eq!(doc.get_type(), "TextDocument");
    }

    #[test]
    fn test_spreadsheet_app_creates_spreadsheet() {
        let app = SpreadsheetApplication;
        let doc = app.new_document();
        assert_eq!(doc.get_type(), "SpreadsheetDocument");
    }
}
