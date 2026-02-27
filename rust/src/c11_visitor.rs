//! # Visitor Pattern
//!
//! The Visitor pattern represents an operation to be performed on elements of an
//! object structure. Visitor lets you define a new operation without changing
//! the classes of the elements on which it operates.
//!
//! ## Key Participants
//! - **Visitor**: Declares visit operation for each element type
//! - **ConcreteVisitor**: Implements each operation
//! - **Element**: Defines accept operation that takes visitor
//! - **ConcreteElement**: Implements accept operation
//! - **ObjectStructure**: Can enumerate its elements
//!
//! ## Benefits
//! - Makes adding new operations easy
//! - Gathers related operations
//! - Adds operations across classes

// Visitor trait
pub trait Visitor {
    fn visit_book(&self, book: &Book);
    fn visit_fruit(&self, fruit: &Fruit);
    fn visit_electronics(&self, electronics: &Electronics);
    fn get_name(&self) -> &str;
}

// Element trait
pub trait Item {
    fn accept(&self, visitor: &dyn Visitor);
    fn get_name(&self) -> &str;
    fn get_price(&self) -> f64;
}

// Concrete Elements
pub struct Book {
    pub title: String,
    pub author: String,
    pub price: f64,
}

impl Book {
    pub fn new(title: &str, author: &str, price: f64) -> Self {
        Self {
            title: title.to_string(),
            author: author.to_string(),
            price,
        }
    }
}

impl Item for Book {
    fn accept(&self, visitor: &dyn Visitor) {
        visitor.visit_book(self);
    }

    fn get_name(&self) -> &str {
        &self.title
    }

    fn get_price(&self) -> f64 {
        self.price
    }
}

pub struct Fruit {
    pub name: String,
    pub price_per_kg: f64,
    pub weight: f64,
}

impl Fruit {
    pub fn new(name: &str, price_per_kg: f64, weight: f64) -> Self {
        Self {
            name: name.to_string(),
            price_per_kg,
            weight,
        }
    }
}

impl Item for Fruit {
    fn accept(&self, visitor: &dyn Visitor) {
        visitor.visit_fruit(self);
    }

    fn get_name(&self) -> &str {
        &self.name
    }

    fn get_price(&self) -> f64 {
        self.price_per_kg * self.weight
    }
}

pub struct Electronics {
    pub model: String,
    pub price: f64,
    pub energy_efficient: bool,
}

impl Electronics {
    pub fn new(model: &str, price: f64, energy_efficient: bool) -> Self {
        Self {
            model: model.to_string(),
            price,
            energy_efficient,
        }
    }
}

impl Item for Electronics {
    fn accept(&self, visitor: &dyn Visitor) {
        visitor.visit_electronics(self);
    }

    fn get_name(&self) -> &str {
        &self.model
    }

    fn get_price(&self) -> f64 {
        self.price
    }
}

// Concrete Visitors
pub struct TaxVisitor {
    book_tax_rate: f64,
    food_tax_rate: f64,
    electronics_tax_rate: f64,
    eco_discount_rate: f64,
}

impl TaxVisitor {
    pub fn new() -> Self {
        Self {
            book_tax_rate: 0.05,
            food_tax_rate: 0.02,
            electronics_tax_rate: 0.15,
            eco_discount_rate: 0.03,
        }
    }
}

impl Visitor for TaxVisitor {
    fn visit_book(&self, book: &Book) {
        let tax = book.get_price() * self.book_tax_rate;
        println!("[TaxVisitor] Book: {}", book.get_name());
        println!("  Author: {}", book.author);
        println!("  Price: ${:.2}", book.get_price());
        println!("  Tax ({:.0}%): ${:.2}", self.book_tax_rate * 100.0, tax);
        println!(
            "  Total: ${:.2}\n",
            book.get_price() + tax
        );
    }

    fn visit_fruit(&self, fruit: &Fruit) {
        let tax = fruit.get_price() * self.food_tax_rate;
        println!(
            "[TaxVisitor] {} ({} kg @ ${}/kg)",
            fruit.get_name(),
            fruit.weight,
            fruit.price_per_kg
        );
        println!("  Price: ${:.2}", fruit.get_price());
        println!("  Tax ({:.0}%): ${:.2}", self.food_tax_rate * 100.0, tax);
        println!(
            "  Total: ${:.2}\n",
            fruit.get_price() + tax
        );
    }

    fn visit_electronics(&self, electronics: &Electronics) {
        let tax = electronics.get_price() * self.electronics_tax_rate;
        let eco_discount = if electronics.energy_efficient {
            electronics.get_price() * self.eco_discount_rate
        } else {
            0.0
        };
        let total_tax = tax - eco_discount;

        println!(
            "[TaxVisitor] Electronics: {}",
            electronics.get_name()
        );
        println!("  Price: ${:.2}", electronics.get_price());
        println!("  Tax ({:.0}%): ${:.2}", self.electronics_tax_rate * 100.0, tax);
        if electronics.energy_efficient {
            println!(
                "  Eco discount ({:.0}%): -${:.2}",
                self.eco_discount_rate * 100.0,
                eco_discount
            );
        }
        println!("  Total tax: ${:.2}", total_tax);
        println!(
            "  Total: ${:.2}\n",
            electronics.get_price() + total_tax
        );
    }

    fn get_name(&self) -> &str {
        "Tax Calculator"
    }
}

impl Default for TaxVisitor {
    fn default() -> Self {
        Self::new()
    }
}

pub struct DiscountVisitor {
    book_discount: f64,
    electronics_discount: f64,
}

impl DiscountVisitor {
    pub fn new() -> Self {
        Self {
            book_discount: 0.10,
            electronics_discount: 0.20,
        }
    }
}

impl Visitor for DiscountVisitor {
    fn visit_book(&self, book: &Book) {
        let discount = book.get_price() * self.book_discount;
        println!("[DiscountVisitor] Book: {}", book.get_name());
        println!("  Original Price: ${:.2}", book.get_price());
        println!(
            "  Discount ({:.0}%): -${:.2}",
            self.book_discount * 100.0,
            discount
        );
        println!(
            "  Sale Price: ${:.2}\n",
            book.get_price() - discount
        );
    }

    fn visit_fruit(&self, fruit: &Fruit) {
        println!("[DiscountVisitor] {}", fruit.get_name());
        println!("  Price: ${:.2}", fruit.get_price());
        println!("  Discount: None");
        println!("  Sale Price: ${:.2}\n", fruit.get_price());
    }

    fn visit_electronics(&self, electronics: &Electronics) {
        let discount = electronics.get_price() * self.electronics_discount;
        println!(
            "[DiscountVisitor] Electronics: {}",
            electronics.get_name()
        );
        println!("  Original Price: ${:.2}", electronics.get_price());
        println!(
            "  Discount ({:.0}%): -${:.2}",
            self.electronics_discount * 100.0,
            discount
        );
        println!(
            "  Sale Price: ${:.2}\n",
            electronics.get_price() - discount
        );
    }

    fn get_name(&self) -> &str {
        "Discount Calculator"
    }
}

impl Default for DiscountVisitor {
    fn default() -> Self {
        Self::new()
    }
}

// Object Structure
pub struct ShoppingCart {
    items: Vec<Box<dyn Item>>,
}

impl ShoppingCart {
    pub fn new() -> Self {
        Self {
            items: Vec::new(),
        }
    }

    pub fn add_item(&mut self, item: Box<dyn Item>) {
        self.items.push(item);
    }

    pub fn accept(&self, visitor: &dyn Visitor) {
        println!(
            "\n[ShoppingCart] Processing cart with visitor: {}\n",
            visitor.get_name()
        );
        println!("[ShoppingCart] Items in cart: {}\n", self.items.len());
        for item in &self.items {
            item.accept(visitor);
        }
    }
}

impl Default for ShoppingCart {
    fn default() -> Self {
        Self::new()
    }
}

/// Example demonstrating Visitor pattern
pub fn example() {
    println!("\n--- Visitor Pattern Example ---\n");

    // Create shopping cart with items
    let mut cart = ShoppingCart::new();
    cart.add_item(Box::new(Book::new("Design Patterns", "Erich Gamma", 49.99)));
    cart.add_item(Box::new(Book::new("Clean Code", "Robert Martin", 39.99)));
    cart.add_item(Box::new(Fruit::new("Apple", 2.99, 1.5)));
    cart.add_item(Box::new(Electronics::new("Laptop Pro", 1299.99, true)));

    // Apply different visitors
    println!("--- Tax Calculation ---");
    cart.accept(&TaxVisitor::new());

    println!("--- Discount Calculation ---");
    cart.accept(&DiscountVisitor::new());
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_book() {
        let book = Book::new("Test", "Author", 29.99);
        assert_eq!(book.get_price(), 29.99);
    }

    #[test]
    fn test_fruit() {
        let fruit = Fruit::new("Orange", 1.99, 2.0);
        assert_eq!(fruit.get_price(), 3.98);
    }

    #[test]
    fn test_electronics() {
        let electronics = Electronics::new("Laptop", 999.99, true);
        assert_eq!(electronics.get_price(), 999.99);
    }

    #[test]
    fn test_shopping_cart() {
        let mut cart = ShoppingCart::new();
        cart.add_item(Box::new(Book::new("Test", "Author", 29.99)));
        assert_eq!(cart.items.len(), 1);
    }

    #[test]
    fn test_visitor() {
        let mut cart = ShoppingCart::new();
        cart.add_item(Box::new(Book::new("Test", "Author", 29.99)));
        cart.accept(&TaxVisitor::new());
        // Should not panic
    }
}
