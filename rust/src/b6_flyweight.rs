//! # Flyweight Pattern
//!
//! The Flyweight pattern uses sharing to support large numbers of fine-grained
//! objects efficiently. It shares intrinsic state that is independent of the
//! extrinsic state.
//!
//! ## Key Participants
//! - **Flyweight**: Interface for intrinsic state
//! - **ConcreteFlyweight**: Implements Flyweight with shared intrinsic state
//! - **UnsharedConcreteFlyweight**: Not all Flyweights need to be shared
//! - **FlyweightFactory**: Creates and manages flyweight objects
//! - **Client**: Maintains reference to flyweights and computes extrinsic state
//!
//! ## Benefits
//! - Reduces memory usage by sharing objects
//! - Centralizes state management
//! - Allows large numbers of objects when most state can be made extrinsic

use std::collections::HashMap;
use std::rc::Rc;

// Flyweight trait
pub trait Character {
    fn display(&self, font_size: u8, color: &str);
    fn get_char(&self) -> char;
}

// Concrete Flyweight (intrinsic state: the character itself)
pub struct Letter {
    c: char, // Intrinsic state - shared
}

impl Letter {
    pub fn new(c: char) -> Self {
        Self { c }
    }
}

impl Character for Letter {
    fn display(&self, font_size: u8, color: &str) {
        println!("Letter '{}' (size={}, color={})", self.c, font_size, color);
    }

    fn get_char(&self) -> char {
        self.c
    }
}

// Unshared Concrete Flyweight
pub struct Space;

impl Character for Space {
    fn display(&self, font_size: u8, _color: &str) {
        println!("[Space] (size={})", font_size);
    }

    fn get_char(&self) -> char {
        ' '
    }
}

// Flyweight Factory
pub struct CharacterFactory {
    letters: HashMap<char, Rc<dyn Character>>,
    space: Option<Rc<dyn Character>>,
}

impl CharacterFactory {
    pub fn new() -> Self {
        Self {
            letters: HashMap::new(),
            space: None,
        }
    }

    pub fn get_character(&mut self, c: char) -> Rc<dyn Character> {
        if !self.letters.contains_key(&c) {
            println!("Creating new flyweight for '{}'", c);
            self.letters.insert(c, Rc::new(Letter::new(c)));
        }
        self.letters.get(&c).cloned().unwrap()
    }

    pub fn get_space(&mut self) -> Rc<dyn Character> {
        if self.space.is_none() {
            self.space = Some(Rc::new(Space));
        }
        self.space.as_ref().cloned().unwrap()
    }

    pub fn flyweight_count(&self) -> usize {
        self.letters.len()
    }
}

impl Default for CharacterFactory {
    fn default() -> Self {
        Self::new()
    }
}

// Context/Glyph (stores extrinsic state)
pub struct GlyphContext {
    character: Rc<dyn Character>,
    font_size: u8,   // Extrinsic state
    color: String,    // Extrinsic state
}

impl GlyphContext {
    pub fn new(character: Rc<dyn Character>, font_size: u8, color: String) -> Self {
        Self {
            character,
            font_size,
            color,
        }
    }

    pub fn display(&self) {
        self.character.display(self.font_size, &self.color);
    }
}

// Document using flyweights
pub struct Document {
    factory: CharacterFactory,
    glyphs: Vec<GlyphContext>,
}

impl Document {
    pub fn new(factory: CharacterFactory) -> Self {
        Self {
            factory,
            glyphs: Vec::new(),
        }
    }

    pub fn add_character(&mut self, c: char, font_size: u8, color: String) {
        let character = self.factory.get_character(c);
        self.glyphs.push(GlyphContext::new(character, font_size, color));
    }

    pub fn add_space(&mut self, font_size: u8) {
        let space = self.factory.get_space();
        self.glyphs.push(GlyphContext::new(space, font_size, "black".to_string()));
    }

    pub fn render(&self) {
        println!(
            "\nRendering document ({} glyphs, {} flyweights used):",
            self.glyphs.len(),
            self.factory.flyweight_count()
        );
        for glyph in &self.glyphs {
            glyph.display();
        }
    }
}

/// Example demonstrating Flyweight pattern
pub fn example() {
    println!("\n--- Flyweight Pattern Example ---\n");

    println!("1. Document Character Example:");

    let mut factory = CharacterFactory::new();
    let mut doc = Document::new(factory);

    // Add characters - reusing 'h', 'e', 'l', 'o' multiple times
    doc.add_character('h', 12, "black".to_string());
    doc.add_character('e', 12, "black".to_string());
    doc.add_character('l', 12, "black".to_string());
    doc.add_character('l', 12, "black".to_string());
    doc.add_character('o', 12, "black".to_string());
    doc.add_space(12);
    doc.add_character('w', 12, "black".to_string());
    doc.add_character('o', 12, "black".to_string());
    doc.add_character('r', 12, "black".to_string());
    doc.add_character('l', 12, "black".to_string());
    doc.add_character('d', 12, "black".to_string());

    doc.render();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_character_factory_shares_letters() {
        let mut factory = CharacterFactory::new();
        let c1 = factory.get_character('a');
        let c2 = factory.get_character('a');

        // Both should return the same underlying character
        assert_eq!(c1.get_char(), c2.get_char());
    }

    #[test]
    fn test_flyweight_count_increases() {
        let mut factory = CharacterFactory::new();
        factory.get_character('a');
        factory.get_character('b');
        factory.get_character('a');

        assert_eq!(factory.flyweight_count(), 2);
    }

    #[test]
    fn test_different_letters_different_objects() {
        let mut factory = CharacterFactory::new();
        let char_a = factory.get_character('a');
        let char_b = factory.get_character('b');

        assert_eq!(char_a.get_char(), 'a');
        assert_eq!(char_b.get_char(), 'b');
    }
}
