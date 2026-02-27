//! # Memento Pattern
//!
//! The Memento pattern captures and externalizes an object's internal state
//! without violating encapsulation, so the object can be restored to this
//! state later.
//!
//! ## Key Participants
//! - **Memento**: Stores internal state of Originator
//! - **Originator**: Creates memento and uses it to restore state
//! - **Caretaker**: Responsible for memento's safekeeping
//!
//! ## Benefits
//! - Preserves encapsulation
//! - Simplifies Originator
//! - Provides rollback/undo functionality

// Memento
#[derive(Clone)]
pub struct DocumentMemento {
    pub content: String,
    pub cursor_position: usize,
    pub timestamp: String,
}

impl DocumentMemento {
    pub fn new(content: String, cursor: usize) -> Self {
        Self {
            content,
            cursor_position: cursor,
            timestamp: format!("{:?}", std::time::SystemTime::now()),
        }
    }
}

// Originator
pub struct TextDocument {
    content: String,
    cursor_position: usize,
}

impl TextDocument {
    pub fn new() -> Self {
        Self {
            content: String::new(),
            cursor_position: 0,
        }
    }

    pub fn write(&mut self, text: &str) {
        self.content.push_str(text);
        self.cursor_position += text.len();
        println!("[Document] Wrote: \"{}\"", text);
        println!("  Current content: \"{}\"", self.content);
    }

    pub fn delete_last(&mut self, count: usize) {
        if self.content.len() >= count {
            self.content.truncate(self.content.len() - count);
            self.cursor_position = self.cursor_position.saturating_sub(count);
            println!("[Document] Deleted last {} character(s)", count);
            println!("  Current content: \"{}\"", self.content);
        }
    }

    pub fn save(&self) -> DocumentMemento {
        println!("[Document] Saving state...");
        DocumentMemento::new(self.content.clone(), self.cursor_position)
    }

    pub fn restore(&mut self, memento: &DocumentMemento) {
        self.content = memento.content.clone();
        self.cursor_position = memento.cursor_position;
        println!("[Document] Restored to state from {:?}", memento.timestamp);
        println!("  Content: \"{}\"", self.content);
        println!("  Cursor position: {}", self.cursor_position);
    }

    pub fn get_content(&self) -> &str {
        &self.content
    }
}

impl Default for TextDocument {
    fn default() -> Self {
        Self::new()
    }
}

// Caretaker
pub struct History {
    undo_stack: Vec<DocumentMemento>,
    redo_stack: Vec<DocumentMemento>,
}

impl History {
    pub fn new() -> Self {
        Self {
            undo_stack: Vec::with_capacity(50),
            redo_stack: Vec::with_capacity(50),
        }
    }

    pub fn save_state(&mut self, memento: DocumentMemento) {
        self.undo_stack.push(memento);
        self.redo_stack.clear();
        println!("[History] State saved. Undo stack: {}\n", self.undo_stack.len());
    }

    pub fn undo(&mut self) -> Option<&DocumentMemento> {
        if self.undo_stack.len() <= 1 {
            println!("[History] Nothing to undo.");
            return None;
        }

        let current = self.undo_stack.pop()?;
        self.redo_stack.push(current);

        println!(
            "[History] Undo performed. Remaining: {}",
            self.undo_stack.len()
        );
        self.undo_stack.last()
    }

    pub fn redo(&mut self) -> Option<&DocumentMemento> {
        let memento = self.redo_stack.pop()?;
        self.undo_stack.push(memento.clone());
        println!("[History] Redo performed.");
        self.undo_stack.last()
    }

    pub fn set_initial(&mut self, memento: DocumentMemento) {
        self.undo_stack.push(memento);
    }
}

impl Default for History {
    fn default() -> Self {
        Self::new()
    }
}

// Game Character Example
#[derive(Clone)]
pub struct GameCharacterMemento {
    pub health: i32,
    pub x: i32,
    pub y: i32,
    pub state: String,
}

pub struct GameCharacter {
    name: String,
    health: i32,
    x: i32,
    y: i32,
    state: String,
}

impl GameCharacter {
    pub fn new(name: &str) -> Self {
        Self {
            name: name.to_string(),
            health: 100,
            x: 0,
            y: 0,
            state: "Normal".to_string(),
        }
    }

    pub fn take_damage(&mut self, damage: i32) {
        self.health = (self.health - damage).max(0);
        self.state = if self.health > 70 {
            "Normal".to_string()
        } else if self.health > 30 {
            "Wounded".to_string()
        } else {
            "Critical".to_string()
        };
        println!("[{}] Took {} damage. Health: {} ({})",
                 self.name, damage, self.health, self.state);
    }

    pub fn move_to(&mut self, x: i32, y: i32) {
        self.x = x;
        self.y = y;
        println!("[{}] Moved to ({}, {})", self.name, self.x, self.y);
    }

    pub fn heal(&mut self, amount: i32) {
        self.health = (self.health + amount).min(100);
        self.state = if self.health > 70 {
            "Normal".to_string()
        } else if self.health > 30 {
            "Wounded".to_string()
        } else {
            "Critical".to_string()
        };
        println!("[{}] Healed {}. Health: {} ({})",
                 self.name, amount, self.health, self.state);
    }

    pub fn create_checkpoint(&self) -> GameCharacterMemento {
        println!("[{}] Checkpoint created!", self.name);
        GameCharacterMemento {
            health: self.health,
            x: self.x,
            y: self.y,
            state: self.state.clone(),
        }
    }

    pub fn restore_from_checkpoint(&mut self, memento: &GameCharacterMemento) {
        self.health = memento.health;
        self.x = memento.x;
        self.y = memento.y;
        self.state = memento.state.clone();
        println!("[{}] Restored from checkpoint!", self.name);
        println!("  Health: {}, Position: ({}, {}), State: {}",
                 self.health, self.x, self.y, self.state);
    }
}

/// Example demonstrating Memento pattern
pub fn example() {
    println!("\n--- Memento Pattern Example ---\n");

    println!("1. Text Editor with Undo/Redo");
    let mut doc = TextDocument::new();
    let mut history = History::new();

    history.set_initial(doc.save());

    doc.write("Hello");
    history.save_state(doc.save());

    doc.write(" World");
    history.save_state(doc.save());

    println!("\n--- Undo ---");
    if let Some(memento) = history.undo() {
        doc.restore(memento);
    }

    println!("\n--- Redo ---");
    if let Some(memento) = history.redo() {
        doc.restore(memento);
    }

    println!("\n2. Game Character Checkpoints");
    let mut hero = GameCharacter::new("Hero");

    hero.move_to(10, 20);
    let checkpoint1 = hero.create_checkpoint();

    hero.take_damage(30);
    let checkpoint2 = hero.create_checkpoint();

    hero.take_damage(50);
    println!("\nRestoring to checkpoint 2...");
    hero.restore_from_checkpoint(&checkpoint2);
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_document_write() {
        let mut doc = TextDocument::new();
        doc.write("Hello");
        assert_eq!(doc.get_content(), "Hello");
    }

    #[test]
    fn test_document_save_and_restore() {
        let mut doc = TextDocument::new();
        doc.write("Hello");
        let memento = doc.save();

        doc.write(" World");
        assert_eq!(doc.get_content(), "Hello World");

        doc.restore(&memento);
        assert_eq!(doc.get_content(), "Hello");
    }

    #[test]
    fn test_history_undo() {
        let mut doc = TextDocument::new();
        let mut history = History::new();

        history.set_initial(doc.save());
        doc.write("Hello");
        history.save_state(doc.save());

        doc.write(" World");
        history.save_state(doc.save());

        if let Some(memento) = history.undo() {
            doc.restore(memento);
        }
        assert_eq!(doc.get_content(), "Hello");
    }

    #[test]
    fn test_game_character_checkpoint() {
        let mut hero = GameCharacter::new("Hero");
        hero.move_to(10, 20);
        let checkpoint = hero.create_checkpoint();

        hero.take_damage(50);
        hero.restore_from_checkpoint(&checkpoint);
        assert_eq!(hero.health, 100);
    }
}
