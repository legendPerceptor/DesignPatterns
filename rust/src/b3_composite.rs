//! # Composite Pattern
//!
//! The Composite pattern composes objects into tree structures to represent
//! part-whole hierarchies. It lets clients treat individual objects and
//! compositions uniformly.
//!
//! ## Key Participants
//! - **Component**: Declares interface for objects in composition
//! - **Leaf**: Represents leaf objects in composition
//! - **Composite**: Stores child components and implements child operations
//!
//! ## Benefits
//! - Defines class hierarchies of primitive and composite objects
//! - Simplifies client code - treats primitives and composites uniformly
//! - Makes adding new component types easy

// Component trait
pub trait FileSystemItem {
    fn get_name(&self) -> &str;
    fn size(&self) -> u64;
    fn display(&self, indent: usize);
}

// Leaf
pub struct File {
    name: String,
    size: u64,
}

impl File {
    pub fn new(name: &str, size: u64) -> Self {
        Self {
            name: name.to_string(),
            size,
        }
    }
}

impl FileSystemItem for File {
    fn get_name(&self) -> &str {
        &self.name
    }

    fn size(&self) -> u64 {
        self.size
    }

    fn display(&self, indent: usize) {
        let padding = " ".repeat(indent);
        println!("{}File: {} ({} bytes)", padding, self.name, self.size);
    }
}

// Composite
pub struct Directory {
    name: String,
    children: Vec<Box<dyn FileSystemItem>>,
}

impl Directory {
    pub fn new(name: &str) -> Self {
        Self {
            name: name.to_string(),
            children: Vec::new(),
        }
    }

    pub fn add(&mut self, item: Box<dyn FileSystemItem>) {
        self.children.push(item);
    }

    pub fn remove(&mut self, index: usize) {
        if index < self.children.len() {
            self.children.remove(index);
        }
    }

    pub fn child_count(&self) -> usize {
        self.children.len()
    }
}

impl FileSystemItem for Directory {
    fn get_name(&self) -> &str {
        &self.name
    }

    fn size(&self) -> u64 {
        self.children.iter().map(|child| child.size()).sum()
    }

    fn display(&self, indent: usize) {
        let padding = " ".repeat(indent);
        println!("{}Directory: {}/", padding, self.name);
        for child in &self.children {
            child.display(indent + 2);
        }
    }
}

/// Helper function to create a sample file system
pub fn create_file_system() -> Directory {
    // Create files
    let readme = Box::new(File::new("readme.txt", 1024));
    let main_cpp = Box::new(File::new("main.cpp", 4096));
    let utils_cpp = Box::new(File::new("utils.cpp", 2048));

    // Create src directory
    let mut src_dir = Directory::new("src");
    src_dir.add(main_cpp);
    src_dir.add(utils_cpp);

    // Create root directory
    let mut root_dir = Directory::new("project");
    root_dir.add(readme);
    root_dir.add(Box::new(src_dir));

    root_dir
}

/// Example demonstrating Composite pattern
pub fn example() {
    println!("\n--- Composite Pattern Example ---\n");

    let mut fs = create_file_system();
    fs.display(0);

    println!("\nTotal size: {} bytes", fs.size());
    println!("Child count: {}", fs.child_count());
    println!("Root name: {}", fs.get_name());

    // Demonstrate remove functionality
    println!("\nRemoving first item...");
    fs.remove(0);
    println!("New child count: {}", fs.child_count());
    println!("New total size: {} bytes", fs.size());
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_file_creation() {
        let file = File::new("test.txt", 1024);
        assert_eq!(file.get_name(), "test.txt");
        assert_eq!(file.size(), 1024);
    }

    #[test]
    fn test_directory_can_contain_files() {
        let mut dir = Directory::new("test_dir");
        dir.add(Box::new(File::new("file1.txt", 100)));
        dir.add(Box::new(File::new("file2.txt", 200)));
        assert_eq!(dir.child_count(), 2);
    }

    #[test]
    fn test_nested_directory_structure() {
        let mut outer = Directory::new("outer");
        let mut inner = Directory::new("inner");
        inner.add(Box::new(File::new("file1.txt", 100)));
        outer.add(Box::new(inner));
        outer.add(Box::new(File::new("file2.txt", 200)));

        assert_eq!(outer.child_count(), 2);
        assert_eq!(outer.size(), 300);
    }

    #[test]
    fn test_calculates_total_size() {
        let fs = create_file_system();
        // readme.txt (1024) + main.cpp (4096) + utils.cpp (2048)
        assert_eq!(fs.size(), 7168);
    }

    #[test]
    fn test_helper_function() {
        let fs = create_file_system();
        assert!(fs.child_count() > 0);
        assert!(fs.size() > 0);
    }
}
